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
volatile uint8_t f_second;
volatile uint8_t f_hot;
volatile uint8_t f_cold;

/// Perform the TI-recommended software trim of the DCO per TI demo code.
void dco_software_trim()
{
    unsigned int oldDcoTap = 0xffff;
    unsigned int newDcoTap = 0xffff;
    unsigned int newDcoDelta = 0xffff;
    unsigned int bestDcoDelta = 0xffff;
    unsigned int csCtl0Copy = 0;
    unsigned int csCtl1Copy = 0;
    unsigned int csCtl0Read = 0;
    unsigned int csCtl1Read = 0;
    unsigned int dcoFreqTrim = 3;
    unsigned char endLoop = 0;

    do
    {
        CSCTL0 = 0x100;                         // DCO Tap = 256
        do
        {
            CSCTL7 &= ~DCOFFG;                  // Clear DCO fault flag
        }while (CSCTL7 & DCOFFG);               // Test DCO fault flag

        __delay_cycles((unsigned int)3000 * MCLK_FREQ_MHZ);// Wait FLL lock status (FLLUNLOCK) to be stable
                                                           // Suggest to wait 24 cycles of divided FLL reference clock
        while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) && ((CSCTL7 & DCOFFG) == 0));

        csCtl0Read = CSCTL0;                   // Read CSCTL0
        csCtl1Read = CSCTL1;                   // Read CSCTL1

        oldDcoTap = newDcoTap;                 // Record DCOTAP value of last time
        newDcoTap = csCtl0Read & 0x01ff;       // Get DCOTAP value of this time
        dcoFreqTrim = (csCtl1Read & 0x0070)>>4;// Get DCOFTRIM value

        if(newDcoTap < 256)                    // DCOTAP < 256
        {
            newDcoDelta = 256 - newDcoTap;     // Delta value between DCPTAP and 256
            if((oldDcoTap != 0xffff) && (oldDcoTap >= 256)) // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim--;
                CSCTL1 = (csCtl1Read & (~(DCOFTRIM0+DCOFTRIM1+DCOFTRIM2))) | (dcoFreqTrim<<4);
            }
        }
        else                                   // DCOTAP >= 256
        {
            newDcoDelta = newDcoTap - 256;     // Delta value between DCPTAP and 256
            if(oldDcoTap < 256)                // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim++;
                CSCTL1 = (csCtl1Read & (~(DCOFTRIM0+DCOFTRIM1+DCOFTRIM2))) | (dcoFreqTrim<<4);
            }
        }

        if(newDcoDelta < bestDcoDelta)         // Record DCOTAP closest to 256
        {
            csCtl0Copy = csCtl0Read;
            csCtl1Copy = csCtl1Read;
            bestDcoDelta = newDcoDelta;
        }

    }while(endLoop == 0);                      // Poll until endLoop == 1

    CSCTL0 = csCtl0Copy;                       // Reload locked DCOTAP
    CSCTL1 = csCtl1Copy;                       // Reload locked DCOFTRIM
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked
}

/// Initialize clock signals and the three system clocks.
/**
 ** We'll take the DCO to 8 MHz, and divide it by 1 for MCLK.
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
 **
 ** So the only change we need to make is to the DCO and MCLK.
 **
 */
void init_clocks() {

    __bis_SR_register(SCG0);                // disable FLL
    CSCTL3 |= SELREF__REFOCLK;              // Set REFO as FLL reference source
    CSCTL1 = DCOFTRIMEN | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_3;// DCOFTRIM=3, DCO Range = 8MHz
    CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                // enable FLL
    dco_software_trim();                        // Software Trim to get the best DCOFTRIM value

    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz

    // default DCODIV as MCLK and SMCLK source; no need to modify CSCTL5.
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

void init_adc() {
    // Configure ADC - Pulse sample mode; ADCSC trigger
    ADCCTL0 |= ADCSHT_8 | ADCON;                                  // ADC ON,temperature sample period>30us
    ADCCTL1 |= ADCSHP;                                            // s/w trig, single ch/conv, MODOSC
    ADCCTL2 |= ADCRES;                                            // 10-bit conversion results
    ADCMCTL0 |= ADCSREF_1 | ADCINCH_12;                           // ADC input ch A12 => temp sense
    ADCIE |=ADCIE0;                                               // Enable the Interrupt request for a completed ADC_B conversion

    // Configure reference
    PMMCTL0_H = PMMPW_H;                                          // Unlock the PMM registers
    PMMCTL2 |= INTREFEN | TSENSOREN;                              // Enable internal reference and temperature sensor
    __delay_cycles(400);                                          // Delay for reference settling

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
    init_adc();

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

        if (f_second) {
            rtc_seconds++;

            if (!(rtc_seconds % BADGE_CLOCK_WRITE_INTERVAL)) {
                // Every BADGE_CLOCK_WRITE_INTERVAL seconds, write our time to the config.
                badge_set_time(rtc_seconds, badge_clock_authority);
            }

            if (!(rtc_seconds % BADGE_BLING_SECS)) {
                badge_bling();
            }

            f_second = 0;
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

        if (f_hot) {
            badge_temp_unlock(1);
            f_hot = 0;
        }

        if (f_cold) {
            badge_temp_unlock(0);
            f_cold = 0;
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

#define CALADC_15V_30C  *((unsigned int *)0x1A1A)                 // Temperature Sensor Calibration-30 C
                                                                  // See device datasheet for TLV table memory mapping
#define CALADC_15V_85C  *((unsigned int *)0x1A1C)                 // Temperature Sensor Calibration-85 C

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    volatile int32_t temp;
    volatile int32_t degF;
    volatile int32_t degC;

    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            temp = ADCMEM0;
            // Temperature in Celsius
            // The temperature (Temp, C)=
            degC = (temp-CALADC_15V_30C)*(85-30)/(CALADC_15V_85C-CALADC_15V_30C)+30;

            // Temperature in Fahrenheit
            // Tf = (9/5)*Tc | 32
            degF = 9*degC/5+32;

            if (degF <= BADGE_UNLOCK_TEMP_UNDER_S01) {
                f_cold = 1;
            } else if (degF >= BADGE_UNLOCK_TEMP_OVER_S00) {
                f_hot = 1;
            } else {
                break;
            }

            LPM0_EXIT;
            break;
        default:
            break;
    }
}

