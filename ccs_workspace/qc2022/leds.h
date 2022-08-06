/// Header for the high-level LED animation driver.
/**
 **
 ** \file leds.h
 ** \author George Louthan
 ** \date   2022
 ** \copyright (c) 2015-2022 George Louthan @duplico. MIT License.
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>

/// Number of RGB LEDs in the system
#define LED_COUNT 5
/// Conversion factor from animation duration unit to system ticks.
/**
 ** This is used by dividing the animation duration by this value. So,
 ** if animation duration units are in milliseconds, then set this to
 ** 10.
 */
#define ANIM_DUR_UNIT_PER_SYSTEM_TICK 10

/// System ticks between fast twinkles.
#define LEDS_TWINKLE_STEPS_FAST 30
/// System ticks between slow twinkles.
#define LEDS_TWINKLE_STEPS_SLOW 125

/// Maximum number of non-ambient animations that can be queued.
#define LEDS_QUEUE_MAXLEN 5
/// Value for an animation ID indicating there is no animation selected.
#define LEDS_ID_NO_ANIM 255

/// Collection of RGB colors of 16 unsigned bits per channel.
typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} rgbcolor16_t;

/// Collection of RGB colors of 8 unsigned bits per channel.
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgbcolor_t;

/// Collection of signed 32-bit RGB channels used for color differences/steps.
typedef struct {
    int_fast32_t red;
    int_fast32_t green;
    int_fast32_t blue;
} rgbdelta_t;

/// An LED animation!
typedef struct {
    const rgbcolor_t (*colors)[LED_COUNT];
    const uint16_t *durations;
    const uint16_t *fade_durs;
    uint8_t len;
    uint8_t anim_type;
} leds_animation_t;

extern uint8_t leds_is_ambient;

void leds_timestep();
void leds_start_anim_by_id(uint8_t anim_id, uint8_t loop, uint8_t ambient, uint8_t clearqueue);
void leds_start_anim_by_struct(const leds_animation_t *animation, uint8_t loop, uint8_t ambient);

#endif /* LEDS_H_ */
