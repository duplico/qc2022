/// Queercon 2022 badge primary high-level event-driven driver.
/**
 ** The badge source module controls the high-level behavior of the badge
 ** application. It handles the persistent configuration, any decisions
 ** about animation unlocking, connection tracking, button behavior, and when
 ** to do the random "bling" animations.
 **
 ** The main control loop is based in the lower-level main.c module, which
 ** calls the functions in badge.c from the appropriate context.
 **
 ** The basic split in responsibility between the badge.c and main.c modules
 ** is that main.c detects, prioritizes, and clears flags set from
 ** interrupts; it then calls the appropriate function in badge.c so that
 ** badge.c can behave in a more event-driven way, with the underlying MSP430
 ** hardware and registers abstracted away by main.c for the most part.
 **
 ** \file badge.c
 ** \author George Louthan
 ** \date   2022
 ** \copyright (c) 2022 George Louthan @duplico. MIT License.
 */

#include <stdint.h>
#include <stdlib.h>

#include <msp430fr2633.h>

#include "badge.h"
#include "rtc.h"
#include "animations.h"
#include "tlc5948a.h"

#pragma PERSISTENT(badge_conf)
/// The main persistent badge configuration.
volatile badge_conf_t badge_conf = (badge_conf_t){
    .badge_id = BADGE_ID_UNASSIGNED,
    .badges_seen = {0,},
    .current_anim_id = ANIM_H00,
    .badges_seen_count = 1, // I've seen myself.
    .ubers_seen_count = 0,
};

/// Whether this badge thinks it has an authoritative clock.
uint8_t badge_clock_authority = 0; // Intentionally clears on power cycle.

/// Whether the next bling anomation should be skipped.
uint8_t badge_bling_button_pressed = 1; // Skip the first bling after startup.

/// The global badge brightness level.
uint8_t badge_brightness_level;

/// Returns whether an animation with the selected id is unlocked.
uint8_t anim_unlocked(uint8_t id) {
    if (id < ANIM_H00) {
        return 0;
    }

    if (((id >= ANIM_H00 && id <= ANIM_H14) || (id >= ANIM_U00 || id <= ANIM_U01)) && badge_conf.clock > BADGE_UNLOCK_SECS_ALL) {
        return 1;
    }

    // Check unlockable animations:
    switch(id) {
    // Standard ambient animations:
    case ANIM_H00:
        return 1;
    case ANIM_H01:
        return 1;
    case ANIM_H02:
        return 1;
    case ANIM_H03:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H03;
    case ANIM_H04:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H04;
    case ANIM_H05:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H05;
    case ANIM_H06:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H06;
    case ANIM_H07:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H07;
    case ANIM_H08:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H08;
    case ANIM_H09:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H09;
    case ANIM_H10:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H10;
    case ANIM_H11:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H11;
    case ANIM_H12:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H12;
    case ANIM_H13:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H13;
    case ANIM_H14:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H14;
    // Uber ambient animations:
    case ANIM_U00:
        return badge_conf.ubers_seen_count >= BADGE_UNLOCK_COUNT_U00;
    case ANIM_U01:
        return badge_conf.ubers_seen_count >= BADGE_UNLOCK_COUNT_U01;
    // Special ambient animations:
    case ANIM_S00:
        // Hot
        return badge_conf.heat_unlocked;
    case ANIM_S01:
        // Cold
        return badge_conf.cold_unlocked;
    case ANIM_S02:
        // Party
        return badge_conf.clock >= BADGE_UNLOCK_SECS_S02;
    }

    // Otherwise, it's not something we can unlock.
    return 0;
}

/// Prepare to write to FRAM by disabling interrupts and unlocking write access to INFOA.
inline volatile void fram_unlock() {
    __bic_SR_register(GIE);
    SYSCFG0 = FRWPPW | PFWP;
}

/// Finish writing to FRAM by locking write access to INFOA and enabling interrupts.
inline volatile void fram_lock() {
    SYSCFG0 = FRWPPW | DFWP | PFWP;
    __bis_SR_register(GIE);
}

/// Switch to the next unlocked animation.
void next_animation() {
    uint8_t candidate = badge_conf.current_anim_id;

    do {
        candidate +=1;
        if (candidate >= ANIM_COUNT)
            candidate = 0;
    } while (!anim_unlocked(candidate));

    fram_unlock();
    badge_conf.current_anim_id = candidate;
    fram_lock();
    // Below, clearqueue is DONTCARE, but I don't have time to test it a change.
    leds_start_anim_by_id(badge_conf.current_anim_id, 0, 1, 1);
}

/// Mark a badge as seen, returning 1 if it's a new badge or 2 if a new uber.
void badge_set_seen(uint8_t id) {
    static uint8_t last_badge_seen = 255;
    static uint32_t last_badge_seen_time = 0;

    if (id >= 8*BADGES_SEEN_BUFFER_LEN_BYTES) {
        return; // Invalid ID.
    }

    if (
            last_badge_seen_time && // If we've set the last seen time...
            rtc_seconds < last_badge_seen_time + BADGE_PAIR_COOLDOWN  && // And it's before our cooldown period...
            id == last_badge_seen // And we're seeing the same badge
    ) {
        return; // Ignore it.
    }

    last_badge_seen = id;
    last_badge_seen_time = rtc_seconds;

    uint8_t seen = check_id_buf(id, badge_conf.badges_seen);

    if (seen) {
        if (is_uber(id)) {
            leds_start_anim_by_id(ANIM_META_PAIRUBER, 0, 0, 1);
        } else {
            leds_start_anim_by_id(ANIM_META_PAIR, 10, 0, 1);
        }

        // Queue our connection count.
        leds_start_anim_by_id(ANIM_META_CONNECTS, 0, 0, 0);

        return;
    }

    // New badge!
    fram_unlock();

    set_id_buf(id, 2, badge_conf.badges_seen);

    if (badge_conf.badges_seen_count < UINT8_MAX) {
        badge_conf.badges_seen_count++;
    }

    if (is_uber(id) && badge_conf.ubers_seen_count < UINT8_MAX) {
        badge_conf.ubers_seen_count++;
    }

    fram_lock();

    uint8_t new_anim_id = LEDS_ID_NO_ANIM;

    // Check to see if we hit a badge seen count that unlocks a new animation.
    // This is up here, before we check to see if it's uber, so that the uber
    //  new animation will be prioritized by overwriting this one.

    switch(badge_conf.badges_seen_count) {
    // The first three are starting animations and can be skipped.
    case BADGE_UNLOCK_COUNT_H03:
        new_anim_id = ANIM_H03;
        break;
    case BADGE_UNLOCK_COUNT_H04:
        new_anim_id = ANIM_H04;
        break;
    case BADGE_UNLOCK_COUNT_H05:
        new_anim_id = ANIM_H05;
        break;
    case BADGE_UNLOCK_COUNT_H06:
        new_anim_id = ANIM_H06;
        break;
    case BADGE_UNLOCK_COUNT_H07:
        new_anim_id = ANIM_H07;
        break;
    case BADGE_UNLOCK_COUNT_H08:
        new_anim_id = ANIM_H08;
        break;
    case BADGE_UNLOCK_COUNT_H09:
        new_anim_id = ANIM_H09;
        break;
    case BADGE_UNLOCK_COUNT_H10:
        new_anim_id = ANIM_H10;
        break;
    case BADGE_UNLOCK_COUNT_H11:
        new_anim_id = ANIM_H11;
        break;
    case BADGE_UNLOCK_COUNT_H12:
        new_anim_id = ANIM_H12;
        break;
    case BADGE_UNLOCK_COUNT_H13:
        new_anim_id = ANIM_H13;
        break;
    case BADGE_UNLOCK_COUNT_H14:
        new_anim_id = ANIM_H14;
        break;
    }

    if (is_uber(id)) {
        leds_start_anim_by_id(ANIM_META_NEWUBER, 0, 0, 1);

        // Determine whether we've unlocked a new animation:
        switch(badge_conf.ubers_seen_count) {
        case BADGE_UNLOCK_COUNT_U00:
            new_anim_id = ANIM_U00;
            break;
        case BADGE_UNLOCK_COUNT_U01:
            new_anim_id = ANIM_U01;
            break;
        }
    } else {
        leds_start_anim_by_id(ANIM_META_NEWPAIR, 0, 0, 1);
    }

    if (new_anim_id != LEDS_ID_NO_ANIM) {
        fram_unlock();
        badge_conf.current_anim_id = new_anim_id;
        fram_lock();
        leds_start_anim_by_id(new_anim_id, 0, 1, 0);
    }

    // Queue our connection count.
    leds_start_anim_by_id(ANIM_META_CONNECTS, 0, 0, 0);
}

/// Set badge ID in the configuration.
void badge_set_id(uint8_t id) {
    uint8_t old_id = badge_conf.badge_id;

    if (id != badge_conf.badge_id) {
        fram_unlock();
        badge_conf.badge_id = id;
        set_id_buf(old_id, 0, badge_conf.badges_seen);
        set_id_buf(badge_conf.badge_id, 1, badge_conf.badges_seen);
        if (!is_uber(old_id) && is_uber(id)) {
            badge_conf.ubers_seen_count++;
        } else if (is_uber(old_id) && !is_uber(id)) {
            badge_conf.ubers_seen_count--;
        }
        fram_lock();

        // Re-seed PRNG.
        srand(badge_conf.badge_id);
    }

    leds_start_anim_by_id(ANIM_META_Z_BRIGHTNESS2, 5, 0, 1);
}

/// Set the current time.
inline void badge_set_time(uint32_t clock, uint8_t authority) {
    rtc_seconds = clock;

    // If it used to be too early to unlock the party, and now the party
    //  is unlocked, go ahead and show it.
    if (badge_conf.clock < BADGE_UNLOCK_SECS_S02 && clock >= BADGE_UNLOCK_SECS_S02) {
        leds_start_anim_by_id(ANIM_S02, 0, 1, 0);
        fram_unlock();
        badge_conf.current_anim_id = ANIM_S02;
        fram_lock();
    }

    fram_unlock();
    badge_clock_authority = authority;
    badge_conf.clock = clock;
    fram_lock();

    // This is called periodically, so we'll kick off ADC samples from here.
    //  But only if needed.
    if (!badge_conf.heat_unlocked || !badge_conf.cold_unlocked) {
        ADCCTL0 |= ADCENC | ADCSC;
    }
}

/// Callback for its being very hot or very cold for the first time.
void badge_temp_unlock(uint8_t hot) {
    if (hot && !badge_conf.heat_unlocked) {
        leds_start_anim_by_id(ANIM_S00, 0, 1, 0);
        fram_unlock();
        badge_conf.heat_unlocked = 1;
        badge_conf.current_anim_id = ANIM_S00;
        fram_lock();
    } else if (!hot && !badge_conf.cold_unlocked) {
        leds_start_anim_by_id(ANIM_S01, 0, 1, 0);
        fram_unlock();
        badge_conf.cold_unlocked = 1;
        badge_conf.current_anim_id = ANIM_S01;
        fram_lock();
    }
}

/// Callback for a long button press.
void badge_button_press_long() {
    badge_bling_button_pressed = 1;

    uint8_t brightness_levels[3] = {
        0,
        64,
        255
    };

    badge_brightness_level = (badge_brightness_level+1) % 3;
    tlc_stage_bc(brightness_levels[badge_brightness_level]);
    tlc_stage_dc_mult(badge_brightness_level+1);
    tlc_set_fun();

    leds_start_anim_by_id(ANIM_META_Z_BRIGHTNESS0 + badge_brightness_level, 1, 0, 1);
}

/// Callback for a short button press.
void badge_button_press_short() {
    badge_bling_button_pressed = 1;

    if (leds_is_ambient) {
        next_animation();
    }
}

/// Call this when it's time for the badge to bling, even if you're not sure it should.
void badge_bling() {
    // Don't do it if we're not ambient or have recently pressed a button.
    if (badge_bling_button_pressed || !leds_is_ambient) {
        badge_bling_button_pressed = 0;
        return;
    }

    // Choose which bling
    // Animate the bling
    leds_start_anim_by_id(ANIM_B00 + rand() % (1 + ANIM_B06 - ANIM_B00), 3, 0, 0);
}

/// Returns the number of lights that should be lit to represent the current badge_seen_count.
uint8_t badge_count_lights() {
    if (badge_conf.badges_seen_count >= BADGES_SEEN_MAX_DISP) {
        return 15;
    }

    return 1 + badge_conf.badges_seen_count/BADGES_SEEN_PER_DISP;
}

/// Initialize the badge application behavior.
void badge_init() {
    // Different PRNG seed per badge.
    srand(badge_conf.badge_id);

    leds_start_anim_by_id(ANIM_META_STARTUP_FADE, 0, 0, 1);
    leds_start_anim_by_id(ANIM_META_CONNECTS, 0, 0, 0);
    leds_start_anim_by_id(badge_conf.current_anim_id, 0, 1, 1);
}
