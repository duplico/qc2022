/// Serial communication module for the arm contacts.
/**
 ** The serial system involves a PHY layer and a link-layer.
 ** The PHY layer operates based on interrupts to decode individual
 ** messages. Once a message is received, a flag is raised to the
 ** main module, the CPU is woken, and link-layer functions are called
 ** on an event basis. These also include time-based functions such as
 ** timeoutes, and the assertion or deassertion of the badge-detect line,
 ** also called o_hai.
 **
 ** \file serial.c
 ** \author George Louthan
 ** \date   2022
 ** \copyright (c) 2022 George Louthan @duplico. MIT License.
 */

#include <stdint.h>
#include <string.h>

#include <msp430fr2633.h>

#include "badge.h"
#include "rtc.h"
#include "serial.h"

volatile serial_message_t serial_message_in;
serial_message_t serial_message_out;
volatile uint8_t serial_phy_state_rx;
volatile uint8_t serial_phy_state_tx;
volatile uint8_t serial_phy_index_rx;
volatile uint8_t serial_phy_index_tx;

volatile uint8_t f_serial_phy = 0;

uint8_t serial_plugged = 0;
uint16_t serial_ll_timeout_ticks;

uint8_t serial_ll_tx_seq = 0;
uint32_t serial_ll_rx_seq = 0;

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

/// Calculate and populate a CRC16 to a serial message.
void crc16_apply(serial_message_t *message) {
    message->crc16 = crc16_buf(
        (uint8_t *) message,
        sizeof(serial_message_t) - sizeof(message->crc16)
    );
}

/// Validate a serial message.
uint8_t validate_message(serial_message_t *message) {
    // Check CRC.
    if (crc16_buf((uint8_t *) message,
                    sizeof(serial_message_t)
                  - sizeof(message->crc16)) != message->crc16) {
        // Bad header CRC.
        return 0;
    }

    // Opcode-specific validation:
    switch(message->opcode) {
    case SERIAL_OPCODE_HELO:
    case SERIAL_OPCODE_ACK:
        if (message->from_id > BADGE_ID_UNASSIGNED) {
            // (highest valid badge ID is BADGE_ID_UNASSIGNED)
            // A controller shouldn't send HELO or ACK.
            return 0;
        }
        break;
    case SERIAL_OPCODE_SETID:
        // Validate our incoming new ID.
        if (message->payload > BADGE_ID_UNASSIGNED) {
            return 0;
        }
    // Fall through...
    case SERIAL_OPCODE_STATQ:
        // Admin commands should only come from a controller.
        if (message->from_id != CONTROLLER_ID) {
            return 0;
        }
        break;
    default:
        return 0; // Don't accept any other opcodes.
    }
    return 1;
}

/// Send a message, applying the payload, len, crc, and from-ID.
void serial_send_start(uint8_t opcode) {
    // Block until the PHY is idle.
    while (serial_phy_state_tx != SERIAL_PHY_STATE_IDLE);

    // Set up the opcode and payload based on the opcode:
    serial_message_out.opcode = opcode;
    switch(opcode) {
    case SERIAL_OPCODE_ACK:
        serial_message_out.payload = serial_ll_rx_seq;
        break;
    case SERIAL_OPCODE_HELO:
        serial_message_out.payload = serial_ll_tx_seq;
        break;
    default:
        serial_message_out.payload = 0;
    }

    // Set the rest of the parameters:
    serial_message_out.last_clock = rtc_seconds;
    serial_message_out.clock_is_set = badge_clock_authority;
    serial_message_out.from_id = badge_conf.badge_id;
    crc16_apply(&serial_message_out);
    serial_phy_state_tx = SERIAL_PHY_STATE_IDLE;
    UCA1TXBUF = SERIAL_PHY_SYNC_WORD;
    // The interrupts will take it from here.
}

/// Link-layer serial receive event, called after validating the received message.
void serial_ll_handle_rx() {
    // NB: All structural and opcode-specific (but not state-specific)
    //     validation has already been done. Any ACK has already been
    //     checked against the expected sequence number.

    // Deal with clock setting first and foremost.
    // We accept another badge's clock under the following conditions:
    if (!badge_clock_authority && serial_message_in.clock_is_set) {
        //  * Our clock is not authoritative, and the other clock is.
        //    (in this case, our clock is then set to authoritative)
        badge_set_time(serial_message_in.last_clock, 1);
    } else if (badge_clock_authority == serial_message_in.clock_is_set &&
               serial_message_in.last_clock > badge_conf.clock + BADGE_CLOCK_DRIFT_ALLOWED_SECONDS) {
        //  * Both clocks' authoritative-ness is the same (i.e. both are
        //     authoritative, or both are non-authoritative), but the other
        //     clock is at least BADGE_CLOCK_DRIFT_ALLOWED_SECONDS in the future.
        //     (in this case, our authoritative-ness is unchanged)
        badge_set_time(serial_message_in.last_clock, badge_clock_authority);
    }

    // Now do other needed stuff. If these generate animations, they'll replace any
    //  that the clocks set off.
    switch(serial_message_in.opcode) {
    case SERIAL_OPCODE_SETID:
        badge_set_id((uint8_t) (0xff & serial_message_in.payload));
        serial_ll_rx_seq = badge_conf.badge_id;
        serial_send_start(SERIAL_OPCODE_ACK);
        break;
    case SERIAL_OPCODE_STATQ:
        serial_ll_rx_seq = ((uint32_t) badge_conf.badges_seen_count) | ((uint32_t) badge_conf.ubers_seen_count << 8);
        if (badge_conf.heat_unlocked) {
            serial_ll_rx_seq |= ((uint32_t) 0b1 << 31);
        }
        if (badge_conf.cold_unlocked) {
            serial_ll_rx_seq |= ((uint32_t) 0b1 << 30);
        }
        if (badge_conf.clock >= BADGE_UNLOCK_SECS_S02) {
            serial_ll_rx_seq |= ((uint32_t) 0b1 << 29);
        }
        serial_send_start(SERIAL_OPCODE_ACK);
        break;
    case SERIAL_OPCODE_HELO:
        serial_ll_rx_seq = serial_message_in.payload;
        serial_send_start(SERIAL_OPCODE_ACK);
        // Fall through...
    case SERIAL_OPCODE_ACK:
        badge_set_seen(serial_message_in.from_id);
        break;
    }
}

/// Called when a link-layer timeout has passed without being reset.
void serial_ll_timeout() {
    if (serial_plugged) {
        serial_send_start(SERIAL_OPCODE_HELO);
    }

    serial_ll_timeout_ticks = SERIAL_TIMEOUT_TICKS;
}

/// Handle the physical connection of two badges.
void serial_phy_connect() {
    // We got an O_HAI signal.
    serial_plugged = 1;
    // After we return, the timeout will send for us.
    serial_ll_timeout_ticks = 1;
    serial_ll_tx_seq++;
}

/// Handle the physical disconnection of two badges.
void serial_phy_disconnect() {
    // PHY TX will handle its own business of shouting into space.
    // PHY RX needs to be set to idle.
    serial_phy_state_rx = SERIAL_PHY_STATE_IDLE;
    serial_plugged = 0;
}

/// The event for receiving a complete but not yet validated serial message.
void serial_phy_handle_rx() {
    // We just got a complete serial message. Validate it.
    if (!validate_message((serial_message_t *) &serial_message_in)) {
        return; // Invalid; ignore.
    }

    // It's valid. So, hand the message to the link-layer.
    serial_ll_handle_rx();
}

/// Call this every system tick to service time-based actions.
void serial_tick() {
    // First, debounce the connection detection.
    static uint8_t ohai_last = 1;
    static uint8_t ohai_curr = 1;

    volatile uint8_t ohai_read = (P2IN & BIT4) ? 1 : 0;
    if (ohai_read == ohai_last && ohai_curr != ohai_last) {
        ohai_curr = ohai_read;

        // Status change on the connection detect line.
        if (ohai_curr) {
            // HIGH: disconnected.
            serial_phy_disconnect();
        } else {
            // LOW: connected.
            serial_phy_connect();
        }
    }
    ohai_last = ohai_read;

    // Next, handle link-layer timeouts.
    serial_ll_timeout_ticks--;

    if (!serial_ll_timeout_ticks) {
        serial_ll_timeout();
    }
}

/// Initialize the serial module.
void serial_init() {
    // We'll be using UCA1 here.

    // Pause the UART peripheral:
    UCA1CTLW0 |= UCSWRST;
    // Configure the baud rate to 230400.
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

    // PHY and link-layer state:
    serial_phy_state_rx = SERIAL_PHY_STATE_IDLE;
    serial_phy_state_tx = SERIAL_PHY_STATE_IDLE;

    serial_ll_timeout_ticks = SERIAL_TIMEOUT_TICKS;

    // Enable interrupts for TX and RX:
    UCA1IE |= UCTXIE | UCRXIE;
}

/// Interrupt service routine for receiving or sending a byte over the serial UART.
#pragma vector=USCI_A1_VECTOR
__interrupt void serial_isr() {
    switch(__even_in_range(UCA1IV, USCI_UART_UCTXIFG)) {
    case USCI_UART_UCRXIFG:
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
                f_serial_phy = 1;
                LPM0_EXIT;
            }
            break; // case SERIAL_PHY_STATE_RX
        }
        break; // case UCRXIFG
    case USCI_UART_UCTXIFG:
        // Transmit buffer full, ready to load another byte to send.
        switch(serial_phy_state_tx) {
        case SERIAL_PHY_STATE_IDLE:
            // We just sent a sync byte. Time to send the message:
            serial_phy_state_tx = SERIAL_PHY_STATE_TX;
            serial_phy_index_tx = 0;
            // fall through...
        case SERIAL_PHY_STATE_TX:
            // This works slightly differently than the receiving version.
            //  It's time to TRANSMIT serial_message_out[serial_phy_index]
            //  because we just sent serial_message_out[serial_phy_index-1]
            //  (or, if serial_phy_index_rx == 0, it was the syncbyte)

            if (serial_phy_index_tx < sizeof(serial_message_out)) {
                // Need to send another.
                UCA1TXBUF = ((uint8_t *) (&serial_message_out))[serial_phy_index_tx];
                serial_phy_index_tx++;
            } else {
                // Done sending the message.
                serial_phy_state_tx = SERIAL_PHY_STATE_IDLE;
                serial_phy_index_tx = 0;
                LPM3_EXIT;
            }
            break;
        }
        break; // case UCTXIFG
    }

}
