/*
 * badge.h
 *
 *  Created on: Jun 26, 2022
 *      Author: george
 */

#ifndef BADGE_H_
#define BADGE_H_

#define BADGES_IN_SYSTEM 170
#define BADGE_ID_UBER_START 151
#define BADGE_ID_UBER_COUNT 20
#define BADGE_ID_UNASSIGNED 150
#define CONTROLLER_ID 251
#define BADGES_SEEN_BUFFER_LEN_BYTES 32

#define BADGES_SEEN_PER_DISP 10
#define BADGES_SEEN_MAX_DISP (BADGES_SEEN_PER_DISP * 15)

#define BADGE_PAIR_COOLDOWN 8
#define BADGE_CLOCK_DRIFT_ALLOWED_SECONDS 300 // 5 minutes

#define BADGE_UNLOCK_COUNT_H00 0
#define BADGE_UNLOCK_COUNT_H01 0
#define BADGE_UNLOCK_COUNT_H02 0
#define BADGE_UNLOCK_COUNT_H03 2
#define BADGE_UNLOCK_COUNT_H04 5
#define BADGE_UNLOCK_COUNT_H05 10
#define BADGE_UNLOCK_COUNT_H06 20
#define BADGE_UNLOCK_COUNT_H07 21
#define BADGE_UNLOCK_COUNT_H08 22
#define BADGE_UNLOCK_COUNT_H09 23
#define BADGE_UNLOCK_COUNT_H10 35
#define BADGE_UNLOCK_COUNT_H11 50
#define BADGE_UNLOCK_COUNT_H12 60
#define BADGE_UNLOCK_COUNT_H13 80
#define BADGE_UNLOCK_COUNT_H14 BADGES_SEEN_MAX_DISP

#define BADGE_UNLOCK_COUNT_U00 1
#define BADGE_UNLOCK_COUNT_U01 5

// NB: This is modded, so a power of 2 is preferred.
#define BADGE_BLING_SECS 64

// SMCLK rate in Hz:
#define SMCLK_RATE_HZ 8000000

// Dot correction configuration for the three colors:
#define TLC_DC_RED 0x03
#define TLC_DC_GRN 0x01
#define TLC_DC_BLU 0x02

// Global brightness correct:
#define TLC_BC 0x00 // 25%

/// Badge config struct definition
typedef struct {
    /// The badge's ID, between CBADGE_ID_START and CBADGE_ID_MAX_UNASSIGNED
    uint16_t badge_id;
    /// Whether the badge has a config created for it.
    uint8_t initialized;
    /// Whether this badge thinks it has an authoritative clock.
    uint8_t clock_authority;
    /// This badge's time clock, in seconds since Wednesday at 8 PM Vegas time.
    uint32_t clock;
    /// Bitfield tracking badge IDs seen.
    uint8_t badges_seen[BADGES_SEEN_BUFFER_LEN_BYTES];
    /// Counter of badges seen generally, including ubers.
    uint8_t badges_seen_count;
    /// Counter of ubers seen.
    uint8_t ubers_seen_count;
    /// ID of the currently selected animation.
    uint8_t current_anim_id;
} badge_conf_t;

// Global-ish variables:
extern badge_conf_t badge_conf;
extern uint8_t badge_bling_button_pressed;
extern volatile uint8_t f_time_loop;
extern volatile uint8_t f_long_press;
extern volatile uint8_t f_bling;

// Utility functions:
uint8_t check_id_buf(uint8_t id, uint8_t *buf);
void set_id_buf(uint8_t id, uint8_t value, uint8_t *buf);
uint8_t byte_rank(uint8_t v);
uint16_t buffer_rank(uint8_t *buf, uint8_t len);
uint8_t is_uber(uint8_t id);
uint8_t anim_unlocked(uint8_t id);
void next_animation();
void badge_set_id(uint8_t id);
inline void badge_set_time(uint32_t clock, uint8_t authority);
void badge_set_seen(uint8_t id);
void badge_button_press_long();
void badge_button_press_short();
void badge_bling();
uint8_t badge_count_lights();
void badge_init();

#endif /* BADGE_H_ */
