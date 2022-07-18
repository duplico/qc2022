/*
 * badge.c
 *
 *  Created on: Jul 18, 2022
 *      Author: george
 */

#include <stdint.h>

#include "badge.h"

#include "band_anims.h"

badge_conf_t badge_conf = (badge_conf_t){
    .badge_id = BADGE_ID_UNASSIGNED,
    .initialized = 0,
    .clock_authority = 0,
    .badges_seen = {0,},
    .current_anim_id = 4,
    .badges_seen_count = 0,
    .ubers_seen_count = 0,
};

uint8_t anim_unlocked(uint8_t id) {
    // TODO
    return 1;
}

void next_animation() {
    uint8_t candidate = badge_conf.current_anim_id;

    do {
        candidate +=1;
        if (candidate == HEAD_ANIM_COUNT)
            candidate = 0;
    } while (!anim_unlocked(candidate));

    badge_conf.current_anim_id = candidate;
    leds_start_anim_by_id(candidate, 0, 1);
}

/// Mark a badge as seen, returning 1 if it's a new badge or 2 if a new uber.
uint8_t set_badge_seen(uint8_t id) {
    if (id >= 8*BADGES_SEEN_BUFFER_LEN_BYTES) {
        return 0; // Invalid ID.
    }

    uint8_t seen = check_id_buf(id, badge_conf.badges_seen);

    if (seen) {
        return 0;
    }

    // New badge.
    set_id_buf(id, badge_conf.badges_seen);

    if (is_uber(id)) {
        badge_conf.ubers_seen_count++;
    }
    badge_conf.badges_seen_count++;

    return is_uber(id)? 2: 1;
}

void badge_set_id(uint8_t id) {
    badge_conf.badge_id = id;
}
