/*
 * leds.c
 *
 *  Created on: Oct 17, 2019
 *      Author: george
 */

#include <stdint.h>
#include <stdlib.h>

#include <msp430fr2633.h>
#include <driverlib.h>

#include "tlc5948a.h"

#include "leds.h"
#include "animations.h"

rgbcolor16_t band_colors_curr[LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

rgbcolor16_t band_colors_next[LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

rgbdelta_t band_colors_step[LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

/// Does the display need re-sent to the LED driver?
uint8_t leds_dirty = 1;

uint8_t led_animation_state = 0;

uint8_t leds_anim_frame;
uint8_t leds_anim_id;
uint8_t leds_saved_anim_id;
uint8_t leds_is_ambient = 1;
uint8_t leds_anim_looping;
uint8_t leds_anim_length;
uint16_t leds_hold_steps;
uint16_t leds_hold_index;
uint16_t leds_transition_steps;
uint16_t leds_transition_index;
const leds_animation_t *leds_current_anim;

uint8_t current_ambient_correct;
uint8_t previous_ambient_correct;

uint8_t leds_twinkle_bits = 0xea;
uint16_t leds_anim_adjustment_index = 0;

/// Start a new frame, setting up current, source, dest, and step.
void leds_load_colors() {
    // leg_colors_curr <- tentacle_current_anim[tentacle_anim_index]

    // Stage in the current color:
    for (uint8_t i=0; i<LED_COUNT; i++) {
        band_colors_curr[i].red = leds_current_anim->colors[leds_anim_frame][i].red * 5;
        band_colors_curr[i].green = leds_current_anim->colors[leds_anim_frame][i].green * 5;
        band_colors_curr[i].blue = leds_current_anim->colors[leds_anim_frame][i].blue * 5;

        // Stage in the next color:
        // If we're looping, it's modded. If not looping, back to black.
        if (leds_anim_frame == leds_current_anim->len-1 && !(leds_anim_looping || leds_is_ambient)) {
            band_colors_next[i].red = 0;
            band_colors_next[i].green = 0;
            band_colors_next[i].blue = 0;
        } else {
            uint8_t next_id = (leds_anim_frame+1) % leds_current_anim->len;
            band_colors_next[i].red = leds_current_anim->colors[next_id][i].red * 5;
            band_colors_next[i].green = leds_current_anim->colors[next_id][i].green * 5;
            band_colors_next[i].blue = leds_current_anim->colors[next_id][i].blue * 5;
        }

        band_colors_step[i].red = ((int_fast32_t) band_colors_next[i].red - band_colors_curr[i].red) / leds_transition_steps;
        band_colors_step[i].green = ((int_fast32_t) band_colors_next[i].green - band_colors_curr[i].green) / leds_transition_steps;
        band_colors_step[i].blue = ((int_fast32_t) band_colors_next[i].blue - band_colors_curr[i].blue) / leds_transition_steps;
    }
}

/// Do a fading step within a frame, stepping colors by their step values.
inline void leds_fade_colors() {
    // If this is the very last transition step,
    if (leds_transition_steps && leds_transition_index == leds_transition_steps-1) {
        // hit the destination:
        memcpy(band_colors_curr, band_colors_next,
               sizeof(rgbcolor_t) * LED_COUNT);
    } else {
        for (uint8_t i = 0; i < LED_COUNT; i++)
        {
            band_colors_curr[i].red += band_colors_step[i].red;
            band_colors_curr[i].green += band_colors_step[i].green;
            band_colors_curr[i].blue += band_colors_step[i].blue;
        }
    }
}

#define LED_OFFSET 1
#define RGB_OFFSET 0

/// Actually apply the appropriate colors to our grayscale values.
/**
 ** This also handles special cases, like twinkling.
 **/
void leds_set_gs(const rgbcolor16_t* colors) {
    static uint_fast32_t r = 0;
    static uint_fast32_t g = 0;
    static uint_fast32_t b = 0;

    for (uint8_t stoplight_index = 0; stoplight_index < LED_COUNT; stoplight_index++)
    {
        r = colors[stoplight_index].red << current_ambient_correct;
        g = colors[stoplight_index].green << current_ambient_correct;
        b = colors[stoplight_index].blue << current_ambient_correct;

        if (leds_current_anim->anim_type != ANIM_TYPE_SOLID) {
            if (leds_twinkle_bits & (1 << stoplight_index)) {
                r = r >> 2;
                g = g >> 2;
                b = b >> 2;
            }
        }

        if (r>UINT16_MAX) r=UINT16_MAX;
        if (g>UINT16_MAX) g=UINT16_MAX;
        if (b>UINT16_MAX) b=UINT16_MAX;

        tlc_gs_data[LED_OFFSET + ((RGB_OFFSET + stoplight_index * 3) % (LED_COUNT * 3)) + 0] = b;
        tlc_gs_data[LED_OFFSET + ((RGB_OFFSET + stoplight_index * 3) % (LED_COUNT * 3)) + 1] = g;
        tlc_gs_data[LED_OFFSET + ((RGB_OFFSET + stoplight_index * 3) % (LED_COUNT * 3)) + 2] = r;
    }
}

void leds_set_steps_and_go() {
    leds_hold_steps = leds_current_anim->durations[leds_anim_frame] / TICKS_PER_LED_ANIM_DUR;
    leds_hold_index = 0;
    leds_transition_steps = leds_current_anim->fade_durs[leds_anim_frame] / TICKS_PER_LED_ANIM_DUR;
    leds_transition_index = 0;

    leds_load_colors();
}

/// Launch a new animation, possibly as the new ambient one.
/**
 ** This should generally not be called directly by the application logic,
 **  but rather indirectly by invoking `leds_start_anim_by_id`.
 */
void leds_start_anim_by_struct(const leds_animation_t *animation, uint8_t loop, uint8_t ambient) {
    // If ambient=1, then a bit of pre-work is needed.
    // If we've been asked to do an interrupting animation, remember what our ambient anim
    //  was so we can go back to it.
    if (!ambient && leds_is_ambient) {
        leds_saved_anim_id = leds_anim_id;
    }

    leds_is_ambient = ambient;

    leds_current_anim = animation;

    leds_anim_frame = 0; // This is our frame index in the animation.
    led_animation_state = 1; // animating

    leds_anim_adjustment_index = 0;

    leds_anim_looping = loop;
    leds_anim_length = leds_current_anim->len;

    leds_set_steps_and_go();
    leds_set_gs(band_colors_curr);
    leds_dirty = 1;
}

void leds_start_anim_by_id(uint8_t anim_id, uint8_t loop, uint8_t ambient) {
    if (ambient && !leds_is_ambient) {
        // If we've been asked to switch our ambient animation, but we're currently in an
        //  interrupting animation, we need to change what we have saved so we go back to
        //  the new ambient animation.
        leds_saved_anim_id = anim_id;
        return;
    }

    leds_start_anim_by_struct(all_anims[anim_id], loop, ambient);

    leds_anim_id = anim_id;
}

/// Load and display the next animation frame in the current animation.
void leds_next_anim_frame() {
    leds_anim_frame++; // This is our index in the animation.

    if (leds_anim_frame >= leds_anim_length) { // rolled over.
        if (leds_is_ambient) { // we're ambient. (loop is dontcare)
            leds_anim_frame = 0; // start over from beginning.
        } else if (leds_anim_looping) { // not ambient; loops remaining
            leds_anim_frame = 0;
            leds_anim_looping--;
        } else { // not ambient, no loops remaining
            leds_is_ambient = 1; // Now we're back to being ambient...
            current_ambient_correct = 1;
            leds_start_anim_by_id(leds_saved_anim_id, 0, 1);
            return; // skip the transitions_and_go because that's called in start_anim.
        }
    }

    leds_set_steps_and_go();
}

/// Do a time step of the LED animation system.
void leds_timestep() {
    // If the ambient light correction has changed, mark everything dirty.
    if (current_ambient_correct != previous_ambient_correct) {
        previous_ambient_correct = current_ambient_correct;
        leds_dirty = 1;
    }

    // Handle the band:
    //  Apply our current delta animation timestep.
    if (leds_current_anim->anim_type != ANIM_TYPE_SOLID) {
        uint16_t target_index;
        if (leds_current_anim->anim_type == ANIM_TYPE_FASTTWINKLE)
            target_index = BAND_TWINKLE_STEPS_FAST/TICKS_PER_LED_ANIM_DUR;
        else
            target_index = BAND_TWINKLE_STEPS_SLOW/TICKS_PER_LED_ANIM_DUR;

        if (leds_anim_adjustment_index == target_index) {
            leds_twinkle_bits = rand() % 256;
            leds_anim_adjustment_index = 0;
            leds_dirty = 1;
        } else {
            leds_anim_adjustment_index++;
        }
    }

    if (led_animation_state) {
        if (leds_hold_index < leds_hold_steps) {
            // Hold the same color.
            leds_hold_index++;
        } else {
            // We're transitioning:
            leds_transition_index++;
            if (leds_transition_index >= leds_transition_steps) {
                // next frame plx.
                leds_next_anim_frame();
                leds_dirty = 1;
            } else {
                leds_fade_colors();
                leds_dirty = 1;
            }
        }
    }

    if (leds_dirty) {
        leds_set_gs(band_colors_curr);
    }

    // If the LEDs were updated...
    if (leds_dirty) {
        // then re-send to the LED driver.
        tlc_set_gs();
        leds_dirty = 0;
    }

}
