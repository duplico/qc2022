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

#include "badge.h"
#include "leds.h"
#include "animations.h"

#define LED_OFFSET 1
#define RGB_OFFSET 1
#define RED_OFFSET 2
#define GRN_OFFSET 1
#define BLU_OFFSET 0

// With our offset configuration, LED index 0 is the top one,
//  by the lanyard hole, they then go clockwise from there.

/// Colors for the current animation frame.
rgbcolor16_t leds_colors_curr[LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

/// Colors for the next animation frame.
rgbcolor16_t leds_colors_next[LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

/// The per-tick adjustment to colors between this frame and next for fading.
rgbdelta_t leds_colors_step[LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

uint8_t leds_anim_queue_ids[LEDS_QUEUE_MAXLEN]; // Initialized in code.
uint8_t leds_anim_queue_loops[LEDS_QUEUE_MAXLEN]; // Initialized in code.

/// Does the display need re-sent to the LED driver?
uint8_t leds_dirty = 1;

uint8_t led_animation_state = 0;

uint8_t leds_anim_frame;
uint8_t leds_anim_id;
uint8_t leds_ambient_anim_id;
uint8_t leds_is_ambient = 1;
uint8_t leds_anim_looping;
uint8_t leds_anim_length;
uint8_t leds_force_twinkle;
uint16_t leds_hold_steps;
uint16_t leds_hold_index;
uint16_t leds_transition_steps;
uint16_t leds_transition_index;
const leds_animation_t *leds_current_anim;

uint8_t leds_twinkle_bits = 0xea;
uint16_t leds_anim_adjustment_index = 0;

/// Start a new frame, setting up current, source, dest, and step.
void leds_load_colors() {
    // Stage in the current color:
    for (uint8_t i=0; i<LED_COUNT; i++) {
        leds_colors_curr[i].red = leds_current_anim->colors[leds_anim_frame][i].red << 8;
        leds_colors_curr[i].green = leds_current_anim->colors[leds_anim_frame][i].green << 8;
        leds_colors_curr[i].blue = leds_current_anim->colors[leds_anim_frame][i].blue << 8;

        // Stage in the next color. There's a few options here to pick the next frame to
        //  fade to.
        if (leds_current_anim == all_anims[ANIM_META_CONNECTS] && leds_anim_frame == leds_anim_length-1) {
            // The very special case where we are showing our connection count
            //  animation and have reached the point where we need to modify the
            //  animation to freeze it.

            // Override the hold time and fade time to match the last visible frame
            //  in the animation.
            leds_hold_steps = leds_current_anim->durations[leds_current_anim->len-1] / TICKS_PER_LED_ANIM_DUR;
            leds_transition_steps =  leds_current_anim->fade_durs[leds_current_anim->len-1] / TICKS_PER_LED_ANIM_DUR;

            // Also, twinkle it.
            leds_force_twinkle = 1;

            // We fade to black after this one.
            leds_colors_next[i].red = 0;
            leds_colors_next[i].green = 0;
            leds_colors_next[i].blue = 0;
        } else if (leds_anim_frame < leds_current_anim->len-1 || leds_anim_looping || leds_is_ambient) {
            // The base case is that this is not the end of the animation; or, that we're
            //  ambient or otherwise still looping
            uint8_t next_id = (leds_anim_frame+1) % leds_current_anim->len;
            leds_colors_next[i].red = leds_current_anim->colors[next_id][i].red << 8;
            leds_colors_next[i].green = leds_current_anim->colors[next_id][i].green << 8;
            leds_colors_next[i].blue = leds_current_anim->colors[next_id][i].blue << 8;
        } else if (leds_anim_queue_ids[0] != LEDS_ID_NO_ANIM) {
            // Or, we could be about to transition to the next non-ambient animation
            //  in the queue.
            leds_colors_next[i].red = all_anims[leds_anim_queue_ids[0]]->colors[0][i].red << 8;
            leds_colors_next[i].green = all_anims[leds_anim_queue_ids[0]]->colors[0][i].green << 8;
            leds_colors_next[i].blue = all_anims[leds_anim_queue_ids[0]]->colors[0][i].blue << 8;
        } else {
            // Or, we're going back to ambient after this.
            leds_colors_next[i].red = all_anims[leds_ambient_anim_id]->colors[0][i].red << 8;
            leds_colors_next[i].green = all_anims[leds_ambient_anim_id]->colors[0][i].green << 8;
            leds_colors_next[i].blue = all_anims[leds_ambient_anim_id]->colors[0][i].blue << 8;
        }

        leds_colors_step[i].red = ((int_fast32_t) leds_colors_next[i].red - leds_colors_curr[i].red) / leds_transition_steps;
        leds_colors_step[i].green = ((int_fast32_t) leds_colors_next[i].green - leds_colors_curr[i].green) / leds_transition_steps;
        leds_colors_step[i].blue = ((int_fast32_t) leds_colors_next[i].blue - leds_colors_curr[i].blue) / leds_transition_steps;
    }
}

/// Do a fading step within a frame, stepping colors by their step values.
inline void leds_fade_colors() {
    // If this is the very last transition step,
    if (leds_transition_steps && leds_transition_index == leds_transition_steps-1) {
        // hit the destination:
        memcpy(leds_colors_curr, leds_colors_next,
               sizeof(rgbcolor_t) * LED_COUNT);
    } else {
        for (uint8_t i = 0; i < LED_COUNT; i++)
        {
            leds_colors_curr[i].red += leds_colors_step[i].red;
            leds_colors_curr[i].green += leds_colors_step[i].green;
            leds_colors_curr[i].blue += leds_colors_step[i].blue;
        }
    }
}

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
        r = colors[stoplight_index].red;
        g = colors[stoplight_index].green;
        b = colors[stoplight_index].blue;

        if (leds_force_twinkle || leds_current_anim->anim_type != ANIM_TYPE_SOLID) {
            if (leds_twinkle_bits & (1 << stoplight_index)) {
                r = r >> 2;
                g = g >> 2;
                b = b >> 2;
            }
        }

        if (r>UINT16_MAX) r=UINT16_MAX;
        if (g>UINT16_MAX) g=UINT16_MAX;
        if (b>UINT16_MAX) b=UINT16_MAX;

        tlc_gs_data[LED_OFFSET + (((RGB_OFFSET + stoplight_index) * 3) % (LED_COUNT * 3)) + RED_OFFSET] = r;
        tlc_gs_data[LED_OFFSET + (((RGB_OFFSET + stoplight_index) * 3) % (LED_COUNT * 3)) + GRN_OFFSET] = g;
        tlc_gs_data[LED_OFFSET + (((RGB_OFFSET + stoplight_index) * 3) % (LED_COUNT * 3)) + BLU_OFFSET] = b;
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
        leds_ambient_anim_id = leds_anim_id;
    }

    leds_is_ambient = ambient;
    leds_force_twinkle = 0;
    leds_current_anim = animation;

    leds_anim_frame = 0; // This is our frame index in the animation.
    led_animation_state = 1; // animating

    leds_anim_adjustment_index = 0;

    leds_anim_looping = loop;
    if (animation == all_anims[ANIM_META_CONNECTS]) {
        leds_anim_length = leds_current_anim->len - badge_count_lights() + 1;
    } else {
        leds_anim_length = leds_current_anim->len;
    }

    leds_set_steps_and_go();
    leds_set_gs(leds_colors_curr);
    leds_dirty = 1;
}

/// Launch a new animation, using its ID, possibly as the new ambient one.
/**
 ** NB: Must call with clearqueue=1 the first time.
 */
void leds_start_anim_by_id(uint8_t anim_id, uint8_t loop, uint8_t ambient, uint8_t clearqueue) {
    if (ambient && !leds_is_ambient) {
        // If we've been asked to switch our ambient animation, but we're currently in an
        //  interrupting animation, we need to change what we have saved so we go back to
        //  the new ambient animation.
        leds_ambient_anim_id = anim_id;
        return;
    }

    if (clearqueue) {
        for (uint8_t i=0; i<LEDS_QUEUE_MAXLEN; i++) {
            leds_anim_queue_ids[i] = LEDS_ID_NO_ANIM;
        }
    }

    // If there's nothing in the queue, and the current animation is ambient, we can
    //  start this animation directly.
    if (leds_is_ambient && leds_anim_queue_ids[0] == LEDS_ID_NO_ANIM) {
        leds_start_anim_by_struct(all_anims[anim_id], loop, ambient);
        leds_anim_id = anim_id;
    } else {
        // Otherwise, enqueue it.
        for (uint8_t i=0; i<LEDS_QUEUE_MAXLEN; i++) {
            if (leds_anim_queue_ids[i] == LEDS_ID_NO_ANIM) {
                leds_anim_queue_ids[i] = anim_id;
                leds_anim_queue_loops[i] = loop;
                break;
            }
        }
    }
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
            // Is there anything in the queue?
            if (leds_anim_queue_ids[0] != LEDS_ID_NO_ANIM) {
                // If so, dequeue it.
                leds_anim_id = leds_anim_queue_ids[0];
                leds_start_anim_by_struct(all_anims[leds_anim_queue_ids[0]], leds_anim_queue_loops[0], 0);

                // Move everything else up.
                for (uint8_t i=0; i<LEDS_QUEUE_MAXLEN-1; i++) {
                    leds_anim_queue_ids[i] = leds_anim_queue_ids[i+1];
                    leds_anim_queue_loops[i] = leds_anim_queue_loops[i+1];
                }
                leds_anim_queue_ids[LEDS_QUEUE_MAXLEN-1] = LEDS_ID_NO_ANIM;
            } else {
            // If not, go back to ambient:
                leds_is_ambient = 1; // Now we're back to being ambient...
                leds_start_anim_by_id(leds_ambient_anim_id, 0, 1, 0);
            }
            return; // skip the transitions_and_go because that's called in start_anim.
        }
    }

    leds_set_steps_and_go();
}

/// Do a time step of the LED animation system.
void leds_timestep() {
    //  Apply our current delta animation timestep.
    if (leds_force_twinkle || leds_current_anim->anim_type != ANIM_TYPE_SOLID) {
        uint16_t target_index;
        if (leds_force_twinkle || leds_current_anim->anim_type == ANIM_TYPE_FASTTWINKLE)
            target_index = LEDS_TWINKLE_STEPS_FAST/TICKS_PER_LED_ANIM_DUR;
        else
            target_index = LEDS_TWINKLE_STEPS_SLOW/TICKS_PER_LED_ANIM_DUR;

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
        leds_set_gs(leds_colors_curr);
    }

    // If the LEDs were updated...
    if (leds_dirty) {
        // then re-send to the LED driver.
        tlc_set_gs();
        leds_dirty = 0;
    }

}
