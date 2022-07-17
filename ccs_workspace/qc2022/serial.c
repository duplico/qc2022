/*
 * serial.c
 *
 *  Created on: Jul 17, 2022
 *      Author: george
 */

#include <stdint.h>
#include <string.h>

#include <msp430fr2633.h>

#include "badge.h"
#include "rtc.h"
#include "serial.h"

volatile serial_message_t serial_message_in; // Needs no initialization.
serial_message_t serial_message_out; // Needs no initialization.
volatile uint8_t serial_phy_state_rx; // 0-init is fine.
volatile uint8_t serial_phy_state_tx; // 0-init is fine.
volatile uint8_t serial_phy_index_rx; // Initialized in ISR.
volatile uint8_t serial_phy_index_tx; // Initialized in ISR.
volatile uint8_t serial_phy_timeout_counter; // 0-init is fine.

volatile uint8_t f_serial_phy = 0;

uint16_t connected_badge_id; // initialized in code
uint8_t serial_ll_state; // initialized in code
uint16_t serial_ll_timeout_ticks; // initialized in code

uint8_t serial_ll_tx_seq = 0;
uint8_t serial_ll_rx_seq = 0;

/// Calculate a 16-bit cyclic redundancy check on buffer sbuf of length len.
uint16_t crc16_buf(volatile uint8_t *sbuf, uint8_t len) {
    uint16_t crc=CRC_SEED;

    while(len){
        crc=(uint8_t)(crc >> 8) | (crc << 8);
        crc^=(uint8_t) *sbuf;
        crc^=(uint8_t)(crc & 0xff) >> 4;
        crc^=(crc << 8) << 4;
        crc^=((crc & 0xff) << 4) << 1;
        len--;
        sbuf++;
    }
    return crc;
}

uint16_t crc_build(uint8_t data, uint8_t start_over) {
    static uint16_t crc = CRC_SEED;
    if (start_over) {
        crc = CRC_SEED;
    }
    crc=(uint8_t)(crc >> 8) | (crc << 8);
    crc^=data;
    crc^=(uint8_t)(crc & 0xff) >> 4;
    crc^=(crc << 8) << 4;
    crc^=((crc & 0xff) << 4) << 1;
    return crc;
}

void crc16_apply(serial_message_t *message) {
    message->crc16 = crc16_buf(
        (uint8_t *) message,
        sizeof(serial_message_t) - sizeof(message->crc16)
    );
}

uint8_t validate_message(serial_message_t *message) {
    // Check CRC.
    if (crc16_buf((uint8_t *) message,
                    sizeof(serial_message_t)
                  - sizeof(message->crc16)) != message->crc16) {
        // Bad header CRC.
        return 0;
    }

    // TODO: Check from address is valid

    // Opcode-specific validation:
    switch(message->opcode) {
    case SERIAL_OPCODE_HELO:
        // TODO: Is there anything to check here?
        break;
    case SERIAL_OPCODE_ACK:
        if (message->payload != serial_ll_tx_seq) {
            // This is not ACKing the message we sent.
            return 0;
        }
        break;
    case SERIAL_OPCODE_SETID:
        if (message->payload > BADGES_IN_SYSTEM || message->from_id != CONTROLLER_ID) {
            return 0;
        }
        break;
    }
    return 1;
}

/// Send a message, applying the payload, len, crc, and from-ID.
void serial_send_start(uint8_t opcode, uint8_t payload_len) {
    // Block until the PHY is idle.
    while (serial_phy_state_tx != SERIAL_PHY_STATE_IDLE);
    serial_message_out.opcode = opcode;
//    serial_message_out.clock_is_set; // TODO
    serial_message_out.last_clock = rtc_seconds;
    serial_message_out.from_id = badge_conf.badge_id;
    crc16_apply(&serial_message_out);
    serial_phy_state_tx = SERIAL_PHY_STATE_IDLE;
    UCA1TXBUF = SERIAL_PHY_SYNC_WORD;
    serial_phy_timeout_counter = SERIAL_PHY_TIMEOUT_TICKS;
    // The interrupts will take it from here.
}

void serial_ll_timeout() {
//    switch(serial_ll_state) {
//    case SERIAL_LL_STATE_NC_PRX:
//        if (!badge_active) {
//            serial_ll_timeout_ms = PRX_TIME_MS;
//            break; // If we're not active, we never leave PRX.
//        }
//
//        // Pin us in PRX mode if the PTX input is asserted.
//        if (SERIAL_DIO_IN & SERIAL_DIO1_PTX) {
//            serial_ll_timeout_ms = PRX_TIME_MS;
//            break;
//        }
//
//        serial_enter_ptx();
//
//        serial_ll_state = SERIAL_LL_STATE_NC_PTX;
//        break;
//    case SERIAL_LL_STATE_NC_PTX:
//        // Pin us in PTX mode if the PRX input is asserted.
//        if (SERIAL_DIO_IN & SERIAL_DIO2_PRX) {
//            serial_ll_timeout_ms = PTX_TIME_MS;
//
//            serial_send_start(SERIAL_OPCODE_HELO, 0);
//            break;
//        }
//
//        serial_enter_prx();
//
//        serial_ll_state = SERIAL_LL_STATE_NC_PRX;
//        break;
//    default:
//        serial_ll_timeout_ms = SERIAL_C_DIO_POLL_MS;
//        if (
//                (serial_phy_mode_ptx && !(SERIAL_DIO_IN & SERIAL_DIO2_PRX))
//                || (!serial_phy_mode_ptx && !(SERIAL_DIO_IN & SERIAL_DIO1_PTX))
//        ) {
//            // We read connection-sense LOW, so we're unplugged.
//            serial_enter_prx();
//            serial_ll_state = SERIAL_LL_STATE_NC_PRX;
//            s_disconnected = 1;
//        }
//        break;
//    }
}

/// Call this every system tick
void serial_ll_tick() {
    serial_ll_timeout_ticks--;

    if (!serial_ll_timeout_ticks) {
        serial_ll_timeout();
    }
}

void serial_ll_handle_rx() {
//    // Note: the proper operation of this function depends on an alternating
//    //       RX/TX mode of operation; that is, there's a danger that if the
//    //       remote badge sends multiple messages in a row, the header and
//    //       payload could get clobbered.
//    switch(serial_ll_state) {
//    case SERIAL_LL_STATE_NC_PRX:
//        // Expecting a HELO.
//        if (serial_message_in.opcode == SERIAL_OPCODE_HELO) {
//            // Need to send an ACK.
//            serial_send_start(SERIAL_OPCODE_ACK, 0);
//            // Once that completes, we'll be connected.
//            connected_badge_id = serial_message_in.from_id;
//            serial_ll_state = SERIAL_LL_STATE_C_IDLE;
//            serial_ll_timeout_ms = SERIAL_C_DIO_POLL_MS;
//            s_connected = 1;
//        }
//        break;
//    case SERIAL_LL_STATE_NC_PTX:
//        // We sent a HELO when we entered this state, so we need an ACK.
//        if (serial_message_in.opcode == SERIAL_OPCODE_ACK) {
//            connected_badge_id = serial_message_in.from_id;
//            serial_ll_state = SERIAL_LL_STATE_C_IDLE;
//            serial_ll_timeout_ms = SERIAL_C_DIO_POLL_MS;
//            s_connected = 1;
//        }
//        break;
//    case SERIAL_LL_STATE_C_IDLE:
//        // cbadges must implement the following commands from idle:
//        // * STAT1Q - send a copy of badge_conf.stats
//        // * STAT2Q - send a pairing message (but not pair)
//        // * SETID  - ONLY FROM CONTROLLER - set ID and respond with ACK.
//        // * SETNAME - Set my handle
//        // * DUMPQ - Reply with a DUMPA
//        // * DISCON - Simulate a physical disconnection.
//        // * SETTYPE - Promote myself to uber or handler, respond with ACK. (CONTROLLER ONLY)
//        // * PAIR - Begin pairing.
//        serial_ll_timeout_ms = SERIAL_C_DIO_POLL_MS;
//
//        if (serial_message_in.opcode == SERIAL_OPCODE_STAT1Q) {
//            serial_send_stats();
//
//        } else if (serial_message_in.opcode == SERIAL_OPCODE_STAT2Q) {
//            serial_pair();
//
//        } else if (serial_message_in.opcode == SERIAL_OPCODE_SETID) {
//            memcpy(&badge_conf.badge_id, (uint8_t *) serial_buffer_in, sizeof(badge_conf.badge_id));
//            write_conf();
//            set_display_type(DISPLAY_OFF);
//            serial_send_start(SERIAL_OPCODE_ACK, 0);
//        } else if (serial_message_in.opcode == SERIAL_OPCODE_DUMPQ) {
//            uint8_t pillar_id = serial_buffer_in[0];
//            if (pillar_id > 3) {
//                // Invalid pillar, do nothing.
//                break;
//            }
//            serial_dump_answer(pillar_id);
//
//        } else if (serial_message_in.opcode == SERIAL_OPCODE_DISCON) {
//            serial_enter_prx();
//            serial_ll_state = SERIAL_LL_STATE_NC_PRX;
//            s_disconnected = 1;
//
//        } else if (serial_message_in.opcode == SERIAL_OPCODE_SETTYPE) {
//            badge_conf.badge_type = serial_buffer_in[0] & 0b11000000;
//            write_conf();
//            serial_send_start(SERIAL_OPCODE_ACK, 0);
//
//        } else if (serial_message_in.opcode == SERIAL_OPCODE_PAIR) {
//            serial_ll_state = SERIAL_LL_STATE_C_PAIRED;
//            s_paired = 1;
//            badge_conf.element_selected = ELEMENT_COUNT_NONE;
//            serial_pair();
//        }
//        break;
//    case SERIAL_LL_STATE_C_PAIRING:
//        if (serial_message_in.opcode == SERIAL_OPCODE_PAIR) {
//            serial_ll_state = SERIAL_LL_STATE_C_PAIRED;
//            s_paired = 1;
//        }
//        break;
//    case SERIAL_LL_STATE_C_PAIRED:
//        // The element selection buttons are ignored.
//        // Color-picking buttons are ignored.
//        // But, mission-doing is a thing!
//        if (serial_message_in.opcode == SERIAL_OPCODE_GOMISSION) {
//            mission_t *mission = (mission_t *) &serial_buffer_in[1];
//            complete_mission(mission);
//        }
//        break;
//    }
//
//    // General purpose ones:
//    if (serial_message_in.opcode == SERIAL_OPCODE_SETNAME) {
//        memcpy(&badge_conf.handle, (uint8_t *) serial_buffer_in, QC16_BADGE_NAME_LEN);
//        // Guarantee null term:
//        badge_conf.handle[QC16_BADGE_NAME_LEN] = 0x00;
//        write_conf();
//        // Don't ACK, but rather send a pairing update with our new handle:
//        serial_pair();
//    }
}

void serial_phy_handle_rx() {
    // We just got a complete serial message. Validate it.
    if (!validate_message((serial_message_t *) &serial_message_in)) {
        return;
    }

    // So, clear out our timeout:
    serial_phy_timeout_counter = SERIAL_PHY_TIMEOUT_TICKS;
    // Handle the message at the link-layer.
    serial_ll_handle_rx();
}

void init_serial() {
    // We'll be using UCA1 here.

    // Pause the UART peripheral:
    UCA1CTLW0 |= UCSWRST;
    // Configure the baud rate to 230400. // TODO: is that okay?
    //  (See page 589 in the family user's guide, SLAU445I)
    // The below is for 8.00 MHz SMCLK:
    UCA1BRW = 2;
    UCA1MCTLW = 0xBB00 | UCOS16 | UCBRF_2;

    // Source the baud rate generation from SMCLK (8 MHz)
    // 8N1 (8 data bits, no parity bits, 1 stop bit), LSB first,
    //  and activate the UART.
    UCA1CTLW0 = UCSSEL__SMCLK;

    // The TX interrupt flag (UCTXIFG) gets set upon enabling the UART.
    //  But, we'd prefer that interrupt not to fire, so we'll clear it
    //  now:
    UCA1IFG &= ~UCTXIFG;

    // Our initial config is in PRX mode.
//    serial_ll_state = SERIAL_LL_STATE_NC_PRX; // TODO

    // PHY and link-layer state:
    serial_phy_mode_ptx=0;
    serial_ll_timeout_ticks = SERIAL_PHY_TIMEOUT_TICKS;

    // Enable interrupts for TX and RX:
    UCA1IE |= UCTXIE | UCRXIE;
}

#pragma vector=USCI_A1_VECTOR
__interrupt void serial_isr() {
    switch(__even_in_range(UCA1IV, UCTXIFG)) {
    case UCRXIFG:
        // Receive buffer full; a byte is ready to read.
        switch(serial_phy_state_rx) {
        case SERIAL_PHY_STATE_IDLE:
            // Initiation of a reception. This needs to be the SYNC WORD.
            if (UCA1RXBUF == SERIAL_PHY_SYNC_WORD) {
                serial_phy_state_rx = SERIAL_PHY_STATE_RX;
                serial_phy_index_rx = 0;
            }
            break; // case SERIAL_PHY_STATE_IDLE
        case SERIAL_PHY_STATE_RX:
            ((uint8_t *) (&serial_message_in))[serial_phy_index_rx] = UCA1RXBUF;
            serial_phy_index_rx++;
            if (serial_phy_index_rx == sizeof(serial_message_in)) {
                // Message completely received.
                serial_phy_state_rx = SERIAL_PHY_STATE_IDLE;
                f_serial_phy = SERIAL_RX_DONE;
                LPM0_EXIT;
            }
            break; // case SERIAL_PHY_STATE_RX
        }
        break; // case UCRXIFG
    case UCTXIFG:
        // Transmit buffer full, ready to load another byte to send.
        switch(serial_phy_state_tx) {
        case SERIAL_PHY_STATE_IDLE:
            // We just sent a sync byte. Time to send the message:
            serial_phy_state_tx = SERIAL_PHY_STATE_TX;
            serial_phy_index_rx = 0;
            // fall through...
        case SERIAL_PHY_STATE_TX:
            // This works slightly differently than the receiving version.
            //  It's time to TRANSMIT serial_message_out[serial_phy_index]
            //  because we just sent serial_message_out[serial_phy_index-1]
            //  (or, if serial_phy_index_rx == 0, it was the syncbyte)

            if (serial_phy_index_rx < sizeof(serial_message_out)) {
                // Need to send another.
                UCA1TXBUF = ((uint8_t *) (&serial_message_out))[serial_phy_index_rx];
                serial_phy_index_tx++;
            } else {
                // Done sending the message.
                serial_phy_state_tx = SERIAL_PHY_STATE_IDLE;
                serial_phy_index_tx = 0;
                LPM3_EXIT; // TODO: this?
            }
            break;
        }
        break; // case UCTXIFG
    }

}
