/*
 * leds.h
 *
 *  Created on: Oct 17, 2019
 *      Author: george
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>

#define LED_COUNT 5
#define TICKS_PER_LED_ANIM_DUR 10 // Number of system ticks per unit of duration in animations.

#define LEDS_TWINKLE_STEPS_FAST 30
#define LEDS_TWINKLE_STEPS_SLOW 125
#define LEDS_TWINKLE_STEPS_HARD 14

typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} rgbcolor16_t;


typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgbcolor_t;

typedef struct {
    int_fast32_t red;
    int_fast32_t green;
    int_fast32_t blue;
} rgbdelta_t;

typedef struct {
    const rgbcolor_t (*colors)[LED_COUNT];
    const uint16_t *durations;
    const uint16_t *fade_durs;
    uint8_t len;
    uint8_t anim_type;
} leds_animation_t;

extern uint8_t leds_is_ambient;

void leds_timestep();
void leds_start_anim_by_id(uint8_t anim_id, uint8_t loop, uint8_t ambient);
void leds_start_anim_by_struct(const leds_animation_t *animation, uint8_t loop, uint8_t ambient);

#endif /* LEDS_H_ */
