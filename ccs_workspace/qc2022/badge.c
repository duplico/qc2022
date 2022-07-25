/*
 * badge.c
 *
 *  Created on: Jul 18, 2022
 *      Author: george
 */

#include <stdint.h>

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
    leds_start_anim_by_id(badge_conf.current_anim_id, 0, 1);
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
        leds_start_anim_by_id(ANIM_META_NEWPAIR, 0, 0);
        return;
    }

    // New badge!
    fram_unlock();

    set_id_buf(id, 2, badge_conf.badges_seen);

    badge_conf.badges_seen_count++;
    // TODO: badge count unlocks.

    if (is_uber(id)) {
        badge_conf.ubers_seen_count++;
        // TODO: uber count unlocks.
    }

    fram_lock();

    // TODO:
    if (is_uber(id)) {
        leds_start_anim_by_id(ANIM_META_PAIR, 0, 0);
    } else {
        leds_start_anim_by_id(ANIM_META_PAIR, 0, 0);
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
}

/// Set the current time.
inline void badge_set_time(uint32_t clock, uint8_t authority) {
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
    tlc_set_fun();

    leds_start_anim_by_id(ANIM_META_Z_BRIGHTNESS0 + brightness_level, 1, 0);
}

void badge_button_press_short() {
    next_animation();
}

/// Initialize the badge.
void badge_init() {
    if (badge_conf.initialized) {
        // We've had a config set before. Just check some things.
    } else {
        // This is our first time turning on.
    }

    leds_start_anim_by_id(badge_conf.current_anim_id, 0, 1);
}
