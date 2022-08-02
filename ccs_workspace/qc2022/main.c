/**
 * \brief   2021 Allhallowtide badge main.
 * \brief   Main entry point for 2021 Allhallowtide Party electronic badge.
 * \author  George Louthan <duplico@dupli.co>
 * \date    2021
 * \copyright MIT License.
 */

// MSP430 main header
#include <msp430fr2633.h>

// C headers
#include <stdint.h>

// Accessory headers
#include <driverlib.h>

// CapTIvate
#include "captivate.h"
#include "CAPT_App.h"

// Local
#include "tlc5948a.h"
#include "leds.h"
#include "rtc.h"
#include "serial.h"
#include "badge.h"
#include "animations.h"

// Global state
uint8_t button_state;

// Interrupt flags
volatile uint8_t f_time_loop;
volatile uint8_t f_long_press;
volatile uint8_t f_bling;

/// Initialize clock signals and the three system clocks.
/**
 ** We'll take the DCO to 16 MHz, and divide it by 2 for MCLK.
 ** Then we'll divide MCLK by 1 to get 8 MHz SMCLK.
 **
 ** Our available clock sources are:
 **  VLO:     10kHz very low power low-freq
 **  REFO:    32.768kHz (typ) reference oscillator
 **  DCO:     Digitally controlled oscillator (1MHz default)
 **           Specifically, 1048576 Hz typical.
 **
 ** At startup, our clocks are as follows:
 **  MCLK:  Sourced by the DCO
 **         (Available: DCO, REFO, VLO)
 **  SMCLK: Sourced from MCLK, with no divider
 **         (Available dividers: {1,2,4,8})
 **  ACLK: Sourced from REFO
 **         (the only available internal source)
 */
void init_clocks() {
    ///// DCO  (Digitally-controlled oscillator)

    // Configure FRAM wait state (set to 1 to support 16MHz MCLK)
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Recommended procedure for setting up these clocks is located in the
    //  user's guide, SLAU445i, page 106, and the formula for DCOCLK is at
    //  page 104.
    // Set DCOCLKDIV to 16 MHz.
    __bis_SR_register(SCG0);                // disable FLL
    CSCTL3 |= SELREF__REFOCLK;              // Set REFO as FLL reference source
    CSCTL0 = 0;                             // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                 // Clear DCO frequency select bits
    CSCTL1 |= DCORSEL_5;                    // Set DCO = 16MHz range
    // CSCTL feedback loop:
    CSCTL2 = FLLD__1 + 487;                  // DCODIV = /1

    // f_DCOCLK = 2^FLLD x (FLLN + 1) x (f_FLLREFCLK / FLLREFDIV)
    // f_DCOCLKDIV =       (FLLN + 1) x (f_FLLREFCLK / FLLREFDIV)
    //    15990784 =          488     x (   32768    /    1     )

    __delay_cycles(3);
    __bic_SR_register(SCG0);                // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked

    // SYSTEM CLOCKS
    // =============

    // CSCTL5 has a reset value of 0x0000; no need to clear it.

    // MCLK (8 MHz)
    //  All sources but MODOSC are available at up to /128
    //  Set to DCOCLKDIV/2 = 8 MHz
    // DIVM__2;

    // SMCLK (8 MHz)
    //  Derived from MCLK with divider up to /8
    //  Set to MCLK/1 = 8 MHz
    // DIVS__1;

    // ACLK (32k) configured to REFO per default

    CSCTL5 = VLOAUTOOFF | DIVS__1 | DIVM__2;
}

/// Apply the initial configuration of the GPIO and peripheral pins.
/**
 **
 */
void init_io() {
    // Per datasheet S4.6, p20, unused pins should be switched to outputs.

    // IO:
    // P1.0     Unused      (SEL 00; DIR 1)
    // P1.1     TA0.1       (SEL 10; DIR 1)
    // P1.2     Unused      (SEL 00; DIR 1)
    // P1.3     Unused      (SEL 00; DIR 1)
    // P1.4     UCA0 SIMO   (SEL 01; DIR 1)
    // P1.5     UCA0 SOMI   (SEL 01; DIR 0)
    // P1.6     UCA0 SCLK   (SEL 01; DIR 1)
    // P1.7     GPIO LAT    (SEL 00; DIR 1)

    // P2.0     Unused      (SEL 00; DIR 1)
    // P2.1     Unused      (SEL 00; DIR 1)
    // P2.2     Unused      (SEL 00; DIR 1)
    // P2.3     Unused      (SEL 00; DIR 1)
    // P2.4     GPIO O_HAI  (SEL 00; DIR 0; REN 1; OUT 1)
    // P2.5     UCA1 RX     (SEL 01; DIR 0)
    // P2.6     UCA1 TX     (SEL 01; DIR 1)
    // P2.7     Unused      (SEL 00; DIR 1)

    // P3.0 is DONTCARE for CAP0.0
    // P3.1     Unused      (SEL 00; DIR 1)
    // P3.2     Unused      (SEL 00; DIR 1)

    // P1
    P1DIR =     0b11011111;
    P1SEL0 =    0b01110000; // LSB
    P1SEL1 =    0b00000010; // MSB
    P1REN =     0x00;
    P1OUT =     0x00;

    // P2
    P2DIR =     0b11001111;
    P2SEL0 =    0b01100000; // LSB
    P2SEL1 =    0b00000000; // MSB
    P2REN =     0b00010000;
    P2OUT =     0b00010000;


    // Init P3 as unused
    P3DIR = 0xFF;
    P3SEL0 = 0x00;
    P3SEL1 = 0x00;
    P3OUT = 0x00;

    // Unlock the pins from high-impedance mode:
    // (AKA the MSP430FR magic make-it-work command)
    PM5CTL0 &= ~LOCKLPM5;
}

void button_cb(tSensor *pSensor) {

    if((pSensor->bSensorTouch == true) && (pSensor->bSensorPrevTouch == false))
    {
        // Button press
        button_state = 1;
        rtc_button_csecs = rtc_centiseconds;
    }

    if((pSensor->bSensorTouch == false) && (pSensor->bSensorPrevTouch == true))
    {
        // Button release
        if (button_state == 1) {
            badge_button_press_short();
        }
        button_state = 0;
    }
}

/// Make snafucated.
int main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Hold WDT.

    // Configure board basics:
    init_clocks();
    init_io();

    // Enable interrupts.
    __bis_SR_register(GIE);

    // Configure mid-level drivers.
    rtc_init();
    tlc_init();
    serial_init();

    // Initialize badge data and game.
    badge_init();

    MAP_CAPT_initUI(&g_uiApp);
    MAP_CAPT_calibrateUI(&g_uiApp);
    MAP_CAPT_registerCallback(&BTN00, &button_cb);

    MAP_CAPT_stopTimer();
    MAP_CAPT_clearTimer();
    MAP_CAPT_selectTimerSource(CAPT_TIMER_SRC_ACLK);
    MAP_CAPT_selectTimerSourceDivider(CAPT_TIMER_CLKDIV__1);
    MAP_CAPT_writeTimerCompRegister(CAPT_MS_TO_CYCLES(g_uiApp.ui16ActiveModeScanPeriod));
    MAP_CAPT_startTimer();
    MAP_CAPT_enableISR(CAPT_TIMER_INTERRUPT);

    WDTCTL = WDTPW | WDTSSEL__ACLK | WDTIS__32K | WDTCNTCL; // 1 second WDT

    while(1)
    {
        // Check whether the time loop flag has been set; this is our
        //  main animation and debouncing loop.
        if (f_time_loop) {
            // First off, pat the dog.
            WDTCTL = WDTPW | WDTSSEL__ACLK | WDTIS__32K | WDTCNTCL; // 1 second WDT

            // Service the LED animation timestep.
            leds_timestep();
            serial_tick();

            f_time_loop = 0;
        }

        if (f_serial_phy) {
            serial_phy_handle_rx();
            f_serial_phy = 0;
        }

        if (f_long_press) {
            f_long_press = 0;
            button_state = 2;
            badge_button_press_long();
        }

        if (f_bling) {
            badge_bling();
            f_bling = 0;
        }

        // Check whether CapTIvate needs to be serviced.
        if (g_bConvTimerFlag)
        {
            CAPT_updateUI(&g_uiApp);
            g_bConvTimerFlag = false;
        }

        __bis_SR_register(LPM0_bits);
    } // End background loop
}
