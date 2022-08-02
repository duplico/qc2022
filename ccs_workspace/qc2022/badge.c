/*
 * badge.c
 *
 *  Created on: Jul 18, 2022
 *      Author: george
 */

#include <stdint.h>
#include <stdlib.h>

#include <msp430fr2633.h>

#include "badge.h"
#include "rtc.h"
#include "animations.h"
#include "tlc5948a.h"

#pragma PERSISTENT(badge_conf)
badge_conf_t badge_conf = (badge_conf_t){
    .badge_id = BADGE_ID_UNASSIGNED,
    .initialized = 0,
    .clock_authority = 0,
    .badges_seen = {0,},
    .current_anim_id = 0,
    .badges_seen_count = 0,
    .ubers_seen_count = 0,
};

uint8_t anim_unlocked(uint8_t id) {
    return 1;

    // Check unlockable animations:
    switch(id) {
    // Standard ambient animations:
    case ANIM_H00:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H00;
    case ANIM_H01:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H01;
    case ANIM_H02:
        return badge_conf.badges_seen_count >= BADGE_UNLOCK_COUNT_H02;
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
    // Uber ambient animations:
    case ANIM_U00:
        return badge_conf.ubers_seen_count >= BADGE_UNLOCK_COUNT_U00;
    case ANIM_U01:
        return badge_conf.ubers_seen_count >= BADGE_UNLOCK_COUNT_U01;
    // Special ambient animations:
    case ANIM_S00:
        // Hot
        return 1;
    case ANIM_S01:
        // Cold
        return 1;
    case ANIM_S02:
        // Party
        return 1;
    }

    // Otherwise, it's not something we can unlock.
    return 0;
}

/// Prepare to write to FRAM by disabling interrupts and unlocking write access to INFOA.
inline void fram_unlock() {
    __bic_SR_register(GIE);
    SYSCFG0 = FRWPPW | PFWP;
}

/// Finish writing to FRAM by locking write access to INFOA and enabling interrupts.
inline void fram_lock() {
    SYSCFG0 = FRWPPW | DFWP | PFWP;
    __bis_SR_register(GIE);
}

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

        return;
    }

    // New badge!
    fram_unlock();

    set_id_buf(id, 2, badge_conf.badges_seen);

    badge_conf.badges_seen_count++;

    if (is_uber(id)) {
        badge_conf.ubers_seen_count++;
    }

    fram_lock();

    if (is_uber(id)) {
        leds_start_anim_by_id(ANIM_META_NEWUBER, 0, 0, 1);
    } else {
        leds_start_anim_by_id(ANIM_META_NEWPAIR, 0, 0, 1);
    }
}

/// Set badge ID in the configuration.
void badge_set_id(uint8_t id) {
    uint8_t old_id = badge_conf.badge_id;

    fram_unlock();
    badge_conf.badge_id = id;
    set_id_buf(old_id, 0, badge_conf.badges_seen);
    set_id_buf(badge_conf.badge_id, 1, badge_conf.badges_seen);
    fram_lock();

    leds_start_anim_by_id(ANIM_META_Z_BRIGHTNESS1, 0, 0, 1);
}

/// Set the current time.
inline void badge_set_time(uint32_t clock, uint8_t authority) {
    if (authority != badge_conf.clock_authority) {
        // If our authority is changing, acknowledge it.
        leds_start_anim_by_id(ANIM_META_Z_BRIGHTNESS2, 0, 0, 1);
    }
    fram_unlock();
    badge_conf.clock_authority = authority;
    badge_conf.clock = clock;
    fram_lock();
}

void badge_button_press_long() {
    static uint8_t brightness_level = 0;
    uint8_t brightness_levels[3] = {
        0,
        64,
        255
    };

    brightness_level = (brightness_level+1) % 3;
    tlc_stage_bc(brightness_levels[brightness_level]);
    tlc_stage_dc_mult(brightness_level+1);
    tlc_set_fun();

    leds_start_anim_by_id(ANIM_META_Z_BRIGHTNESS0 + brightness_level, 1, 0, 1);
}

void badge_button_press_short() {
    if (leds_is_ambient) {
        next_animation();
    }
}

void badge_bling() {
    // Don't do it if we're not ambient.
    if (!leds_is_ambient) {
        return;
    }

    // Choose which bling
    // Animate the bling
    // TODO: Handle if number of blings changes
    leds_start_anim_by_id(ANIM_B00 + rand() % (1 + ANIM_B06 - ANIM_B00), 3, 0, 0);
}

uint8_t badge_count_lights() {
    if (badge_conf.badges_seen_count >= BADGES_SEEN_MAX_DISP) {
        return 15;
    }
    return 1 + badge_conf.badges_seen_count/BADGES_SEEN_PER_DISP;
}

/// Initialize the badge.
void badge_init() {
    if (badge_conf.initialized) {
        // We've had a config set before. Just check some things.
    } else {
        // This is our first time turning on.
    }

    srand(badge_conf.badge_id);

//    leds_start_anim_by_id(ANIM_META_STARTUP_FADE, 0, 0, 1);
//    leds_start_anim_by_id(ANIM_META_STARTUP_SPIN, 5, 0, 0);
//    leds_start_anim_by_id(ANIM_META_CONNECTS, 0, 0, 0);
    leds_start_anim_by_id(badge_conf.current_anim_id, 0, 1, 1); // TODO
}
