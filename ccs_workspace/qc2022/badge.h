/// Queercon 2022 main badge header file.
/**
 ** This header contains the core configuration and functionality declarations
 ** and definitions for the Queercon 2022 badge.
 **
 ** \file badge.h
 ** \author George Louthan
 ** \date   2022
 ** \copyright (c) 2022 George Louthan @duplico. MIT License.
 */

#ifndef BADGE_H_
#define BADGE_H_

// Configuration defines.

/// Valid badge ID but indicating it hasn't been assigned by a controller.
#define BADGE_ID_UNASSIGNED 250
/// The ID used only by a controller device to set IDs and times.
#define CONTROLLER_ID 251

/// The lowest badge ID that is an uber/awesome badge.
#define BADGE_ID_UBER_START 201
/// The maximum number of uber/awesome badges.
#define BADGE_ID_UBER_COUNT 20

/// The number of unique badges to add a dot to the badge count display.
#define BADGES_SEEN_PER_DISP 10
/// The number of badges required to fill the badge count display.
#define BADGES_SEEN_MAX_DISP (BADGES_SEEN_PER_DISP * 14)

/// Number of seconds to prevent pairing activity after a successful pair.
#define BADGE_PAIR_COOLDOWN 8
/// Seconds a remote badge needs to be in the future to accept its clock.
#define BADGE_CLOCK_DRIFT_ALLOWED_SECONDS 300 // 5 minutes

/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H00 0
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H01 0
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H02 0
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H03 2
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H04 5
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H05 10
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H06 20
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H07 21
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H08 22
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H09 23
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H10 35
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H11 45
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H12 55
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H13 65
/// Number of unique pairings required to unlock the animation with this ID.
#define BADGE_UNLOCK_COUNT_H14 99

/// Number of unique pairings with uber badges to unlock this animation.
#define BADGE_UNLOCK_COUNT_U00 1
/// Number of unique pairings with uber badges to unlock this animation.
#define BADGE_UNLOCK_COUNT_U01 5

/// Number of seconds that must have passed to unlock this animation.
#define BADGE_UNLOCK_SECS_S02 212400

/// Number of seconds that must pass to unlock ALL animations.
#define BADGE_UNLOCK_SECS_ALL 302400

/// Temperature in Fahrenheit over which to unlock this animation.
#define BADGE_UNLOCK_TEMP_OVER_S00 95
/// Temperature in Fahrenheit under which to unlock this animation.
#define BADGE_UNLOCK_TEMP_UNDER_S01 55

/// Bling animation interval in seconds. preferably a power of 2.
#define BADGE_BLING_SECS 64
/// Badge clock write interval in seconds. preferably a power of 2.
#define BADGE_CLOCK_WRITE_INTERVAL 16

/// MCLK rate in MHZ.
#define MCLK_FREQ_MHZ 8
/// SMCLK rate in Hz.
#define SMCLK_RATE_HZ 8000000

/// Baseline multiplier for dot correction for red LEDs.
#define TLC_DC_RED 0x03
/// Baseline multiplier for dot correction for green LEDs.
#define TLC_DC_GRN 0x01
/// Baseline multiplier for dot correction for blue LEDs.
#define TLC_DC_BLU 0x02

/// Default global brightness correct for LEDs.
#define TLC_BC 0x00 // 25%

// System values, not really config.

/// Number of bytes in the bitfield of all badge IDs.
#define BADGES_SEEN_BUFFER_LEN_BYTES 32

/// Badge config struct definition
typedef struct {
    /// The badge's ID, between 0 and BADGE_ID_UNASSIGNED, inclusive.
    uint16_t badge_id;
    /// This badge's time clock, in seconds since Wednesday at noon Vegas time.
    uint32_t clock;
    /// Bitfield tracking badge IDs seen.
    uint8_t badges_seen[BADGES_SEEN_BUFFER_LEN_BYTES];
    /// Counter of badges seen generally, including ubers.
    uint8_t badges_seen_count;
    /// Counter of ubers seen.
    uint8_t ubers_seen_count;
    /// ID of the currently selected animation.
    uint8_t current_anim_id;
    /// Status of the heat unlock.
    uint8_t heat_unlocked;
    /// Status of the cold unlock.
    uint8_t cold_unlocked;
} badge_conf_t;

// Global-ish variables:
extern volatile badge_conf_t badge_conf;
extern uint8_t badge_clock_authority;
extern uint8_t badge_bling_button_pressed;
extern uint8_t badge_brightness_level;
extern volatile uint8_t f_time_loop;
extern volatile uint8_t f_long_press;
extern volatile uint8_t f_second;
extern uint8_t button_state;

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
void badge_temp_unlock(uint8_t hot);
void badge_button_press_long();
void badge_button_press_short();
void badge_bling();
uint8_t badge_count_lights();
void badge_init();

#endif /* BADGE_H_ */
