/*
 * tlc5948a.c
 * (c) 2016 George Louthan
 * 3-clause BSD license; see license.md.
 */

#include "tlc5948a.h"

#include <stdint.h>
#include <string.h>
#include <driverlib.h>

#include "badge.h"

/*
 *   LED controller (TLC5948A)
 *        eUSCI_A0 - LEDs
 *        (write on rise, change on fall,
 *         clock inactive low, MSB first)
 *        somi, miso, clk (3-wire)
 *        GSCLK     P1.2 (timer TA1.1)
 *        LAT       P1.4
 *
 *   This file's job is to keep the display going. Application logic will go
 *   elsewhere - this is strictly a driver.
 *
 */

#define TLC_THISISGS    0x00
#define TLC_THISISFUN   0x01

// Current TLC sending state:
uint8_t tlc_send_type = TLC_SEND_IDLE;
uint8_t tlc_tx_index = 0;   // Index of currently sending buffer

uint8_t tlc_loopback_data_out = 0x00;
volatile uint8_t tlc_loopback_data_in = 0x00;

uint16_t tlc_gs_data[16] = { 0x0000, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, };

// This is the basic set of function data.
// A few of them can be edited.
uint8_t fun_base[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ...reserved...
        // B136 / PSM(D2)       0
        // B135 / PSM(D1)       0
        // B134 / PSM(D0)       0
        // B133 / OLDENA        0
        // B132 / IDMCUR(D1)    0
        // B131 / IDMCUR(D0)    0
        // B130 / IDMRPT(D0)    0
        // B129 / IDMENA        0
        // B128 / LATTMG(D1)    1: (byte 15)
        0x01,
        // B127 / LATTMG(D0)    1
        // B126 / LSDVLT(D1)    0
        // B125 / LSDVLT(D0)    0
        // B124 / LODVLT(D1)    0
        // B123 / LODVLT(D0)    0
        // B122 / ESPWM         1
        // B121 / TMGRST        0
        // B120 / DSPRPT        1: (byte 16)
        0b10000101,
        // B119 / BLANK
        // and 7 bits of global brightness control: (byte 17)
        TLC_BC,
        // HERE WE SWITCH TO 7-BIT SPI.
        // The following index is 18: // TODO: dot correct here:
        0x7F,
        TLC_DC_BLU, TLC_DC_GRN, TLC_DC_RED,
        TLC_DC_BLU, TLC_DC_GRN, TLC_DC_RED,
        TLC_DC_BLU, TLC_DC_GRN, TLC_DC_RED,
        TLC_DC_BLU, TLC_DC_GRN, TLC_DC_RED,
        TLC_DC_BLU, TLC_DC_GRN, TLC_DC_RED,
};

void tlc_set_gs() {
    while (tlc_send_type != TLC_SEND_IDLE);
    tlc_send_type = TLC_SEND_TYPE_GS;
    tlc_tx_index = 0;
    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, TLC_THISISGS);
}

void tlc_set_fun() {
    while (tlc_send_type != TLC_SEND_IDLE);
    tlc_send_type = TLC_SEND_TYPE_FUN;
    tlc_tx_index = 0;
    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, TLC_THISISFUN);
}

// Stage the blank bit:
void tlc_stage_blank(uint8_t blank) {
    if (blank) {
        fun_base[17] |= BIT7;
        fun_base[16] &= ~BIT1;
    } else {
        fun_base[17] &= ~BIT7;
        fun_base[16] |= BIT1;
    }
}

// Test the TLC chip with a shift-register loopback.
// Returns 0 for success and 1 for failure.
uint8_t tlc_test_loopback(uint8_t test_pattern) {
    // Send the test pattern 34 times, and expect to receive it shifted
    // a bit.
    tlc_loopback_data_out = test_pattern;
    while (tlc_send_type != TLC_SEND_IDLE);

    EUSCI_A_SPI_clearInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_RECEIVE_INTERRUPT);
    EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_RECEIVE_INTERRUPT);

    tlc_send_type = TLC_SEND_TYPE_LB;
    tlc_tx_index = 0;
    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, test_pattern);
    // Spin while we send and receive:
    while (tlc_send_type != TLC_SEND_IDLE);

    EUSCI_A_SPI_disableInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_RECEIVE_INTERRUPT);

    return tlc_loopback_data_in != (uint8_t) ((test_pattern << 7) | (test_pattern >> 1));
}

// Stage global brightness if different from default:
void tlc_stage_bc(uint8_t bc) {
    bc = bc & 0b01111111; // Mask out BLANK just in case.
    fun_base[17] &= 0b10000000;
    fun_base[17] |= bc;
}

void tlc_init() {
    // We're assuming that the GPIO/peripheral selection has already been configured elsewhere.
    // However, we need to make sure LAT starts out low:
    LAT_POUT &= ~LAT_PBIT;

    // Configure our SPI peripheral to talk to the LED controller.
    // TODO: Redo USCI_A0 with registers, not driverlib:
    UCA0CTLW0 |= UCSWRST;  // Shut down USCI_A0,

    // And USCI_A0 peripheral:
    EUSCI_A_SPI_initMasterParam ini = {0};
    ini.clockPhase = EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    ini.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    ini.clockSourceFrequency = SMCLK_RATE_HZ;
    ini.desiredSpiClock = 1000000;
    ini.msbFirst = EUSCI_A_SPI_MSB_FIRST;
    ini.selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK;
    ini.spiMode = EUSCI_A_SPI_3PIN;
    EUSCI_A_SPI_initMaster(EUSCI_A0_BASE, &ini);

    UCA0CTLW0 &= ~UC7BIT;  //  put it in 8-bit mode out of caution.
    UCA0CTLW0 &= ~UCSWRST; //  and enable it.

    EUSCI_A_SPI_clearInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
    EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);

    // TODO: do something with this:
//    volatile uint8_t loopback_test = 0;
//    loopback_test = tlc_test_loopback(0b11011000);
//    __no_operation();

    // Stage an un-blank configuration to the function data:
    tlc_stage_blank(0);

    // Send our initial function data:
    tlc_set_fun();
    // And our initial grayscale data:
    tlc_set_gs();


    // Finally, we're going to configure the timer that outputs GSCLK.
    //  We want this to go as fast as possible. (Meaning as fast as we can, as
    //  its max, 33 MHz, is faster than our fastest possible source)
    // We're going to use T0A3 for this, sourced from an undivided SMCLK.
    //  (this should be our fastest source, since it can't use MCLK.)

    TA0CTL = MC__STOP; // Make sure the timer is stopped.
    TA0CCR0 = 1; // Reset timer at 1
    TA0CCTL1 = OUTMOD_7; // Reset/Set mode. TODO: Is this the right mode? (See slau445i p376)
    TA0CCR1 = 1; // Toggle our output at 1
    TA0CTL = TASSEL__SMCLK|MC_1; // Start it in up mode based on an undivided SMCLK.
}

#pragma vector=USCI_A0_VECTOR
__interrupt void EUSCI_A0_ISR(void)
{
    switch (__even_in_range(UCA0IV, 4)) {
    //Vector 2 - RXIFG
    case 2:
        // We received some garbage sent to us while we were sending.
        if (tlc_send_type == TLC_SEND_TYPE_LB) {
            // We're only interested in it if we're doing a loopback test.
            tlc_loopback_data_in = EUSCI_B_SPI_receiveData(EUSCI_A0_BASE);
            __no_operation();
        } else {
            EUSCI_B_SPI_receiveData(EUSCI_A0_BASE); // Throw it away.
        }
        break; // End of RXIFG ///////////////////////////////////////////////////////

    case 4: // Vector 4 - TXIFG : I just sent a byte.
        if (tlc_send_type == TLC_SEND_TYPE_GS) {
            if (tlc_tx_index == 32) { // done
                LAT_POUT |= LAT_PBIT; LAT_POUT &= ~LAT_PBIT; // Pulse LAT
                tlc_send_type = TLC_SEND_IDLE;
                break;
            } else { // gs - MSB first; this starts with 0.
                volatile static uint16_t channel_gs = 0;
                channel_gs = tlc_gs_data[tlc_tx_index/2];
                __no_operation();
                if (tlc_tx_index & 0x01) { // odd; less significant byte
                    UCA0TXBUF = (channel_gs & 0xff);
                } else { // even; more significant byte
                    UCA0TXBUF = (channel_gs >> 8) & 0xff;
                }
            }
            tlc_tx_index++;
        } else if (tlc_send_type == TLC_SEND_TYPE_FUN) {
            if (tlc_tx_index == 18) { // after 18 we have to switch to 7-bit mode.
                UCA0CTLW0 |= UCSWRST;
                UCA0CTLW0 |= UC7BIT;
                UCA0CTLW0 &= ~UCSWRST;
                EUSCI_A_SPI_clearInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
                EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
            } else if (tlc_tx_index == 34) {
                LAT_POUT |= LAT_PBIT; LAT_POUT &= ~LAT_PBIT; // Pulse LAT
                UCA0CTLW0 |= UCSWRST;
                UCA0CTLW0 &= ~UC7BIT;
                UCA0CTLW0 &= ~UCSWRST;
                EUSCI_A_SPI_clearInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
                EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
                tlc_send_type = TLC_SEND_IDLE;
                break;
            }
            EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, fun_base[tlc_tx_index]);
            tlc_tx_index++;
        } else if (tlc_send_type == TLC_SEND_TYPE_LB) { // Loopback for POST
            if (tlc_tx_index == 33) {
                tlc_send_type = TLC_SEND_IDLE;
                break;
            }
            EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, tlc_loopback_data_out);
            tlc_tx_index++;
        } else {
            tlc_send_type = TLC_SEND_IDLE; // probably shouldn't reach.
        }
        break; // End of TXIFG /////////////////////////////////////////////////////
    default: break;
    } // End of ISR flag switch ////////////////////////////////////////////////////
}
