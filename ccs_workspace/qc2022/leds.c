/// High-level animation driver for the Queercon 2022 badge.
/**
 ** Unlike the low-level TLC5948A driver, this is a mostly hardware-agnostic
 ** driver for displaying LED animations. It assumes some number of RGB LEDs
 ** and supports ambient looping background animations, interrupting
 ** non-ambient animations, a configurable queueing length for playing multiple
 ** non-ambient animations in sequence, temporary looping of non-ambient
 ** animations, and two speeds of "twinkle" overlay.
 **
 ** Animations are specified in generated `animations.c` source file and
 ** `animations.h` header file. They are generated from a set of basic
 ** animation specifications interpreted by a Python script.
 **
 ** It also has two very specific QC2022 special cases:
 **
 ** The first is a badges seen counter, which detects when the animation
 ** `ANIM_META_CONNECTS` is being played and sets the ending frame correctly
 ** based on the number of badges this badge has connected to.
 **
 ** The second is a special case ambient animation called `U00`, which shows
 ** a binary counter using all the LEDs, whose value persists across plays of
 ** other animations, but not a power cycle.
 **
 ** \file leds.c
 ** \author George Louthan
 ** \date   2022
 ** \copyright (c) 2015-2022 George Louthan @duplico. MIT License.
 */

#include <stdint.h>
#include <stdlib.h>

#include <msp430fr2633.h>
#include <driverlib.h>

#include "tlc5948a.h"

#include "badge.h"
#include "leds.h"
#include "animations.h"

/// Number of RGB LEDs to shift the color channels when writing the GS buffer.
#define LED_OFFSET 1
/// Number of single channels to shift when writing the GS buffer.
#define RGB_OFFSET 1
/// Select the order of the red channels within the RGB LEDs.
#define RED_OFFSET 2
/// Select the order of the green channels within the RGB LEDs.
#define GRN_OFFSET 1
/// Select the order of the blue channels within the RGB LEDs.
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

/// Queue of animation IDs to play after the current animation.
uint8_t leds_anim_queue_ids[LEDS_QUEUE_MAXLEN]; // Initialize to LEDS_ID_NO_ANIM.
/// Loop counts of currently queued animations.
uint8_t leds_anim_queue_loops[LEDS_QUEUE_MAXLEN];

/// Does the display need re-sent to the LED driver?
uint8_t leds_dirty = 1;

/// Is the LED system playing anything right now?
uint8_t led_animation_state = 0;

/// QC2022: The running binary counter for the U00 animation.
uint16_t leds_anim_frame_quaternary_saved;
/// The current frame number in the current animation.
uint16_t leds_anim_frame;
/// The current animation ID, if known.
uint8_t leds_anim_id;
/// The ID of the configured ambient animation.
uint8_t leds_ambient_anim_id;
/// Whether the LEDs are displaying an ambient animation.
uint8_t leds_is_ambient = 1;
/// The number of lops remaining.
uint8_t leds_anim_looping;
/// The length of the current animation.
uint16_t leds_anim_length;
/// Whether to force the LEDs to twinkle regardless of animation setting.
uint8_t leds_force_twinkle;
/// Number of steps to hold the current frame before beginning to fade.
uint16_t leds_hold_steps;
/// Number of system ticks that have elapsed while holding the current frame.
uint16_t leds_hold_index;
/// Number of intermediate fading steps between this frame and the next.
uint16_t leds_transition_steps;
/// Number of system ticks elapsed while fading to the next frame.
uint16_t leds_transition_index;

/// Pointer to the current animation.
const leds_animation_t *leds_current_anim;

/// Bitmask to use for dimming or brightening LEDs for the twinkle effect.
uint8_t leds_twinkle_bits = 0xea;
/// System tick counter used only for twinkling.
uint16_t leds_anim_adjustment_index = 0;

/// Pre-baked expression for the shift from 8-bit color to 16-bit output color.
#define BRIGHTNESS_SHIFT (6 + badge_brightness_level)

/// Start a new frame, setting up current, source, dest, and step.
void leds_load_colors() {
    // Stage in the current color:
    for (uint8_t i=0; i<LED_COUNT; i++) {
        leds_colors_curr[i].red = leds_current_anim->colors[leds_anim_frame][i].red << BRIGHTNESS_SHIFT;
        leds_colors_curr[i].green = leds_current_anim->colors[leds_anim_frame][i].green << BRIGHTNESS_SHIFT;
        leds_colors_curr[i].blue = leds_current_anim->colors[leds_anim_frame][i].blue << BRIGHTNESS_SHIFT;

        // Stage in the next color. There's a few options here to pick the next frame to
        //  fade to.
        if (leds_current_anim == all_anims[ANIM_META_CONNECTS] && leds_anim_frame == leds_anim_length-1) {
            // The very special case where we are showing our connection count
            //  animation and have reached the point where we need to modify the
            //  animation to freeze it.

            // Override the hold time and fade time to match the last visible frame
            //  in the animation.
            leds_hold_steps = leds_current_anim->durations[leds_current_anim->len-1] / ANIM_DUR_UNIT_PER_SYSTEM_TICK;
            leds_transition_steps =  leds_current_anim->fade_durs[leds_current_anim->len-1] / ANIM_DUR_UNIT_PER_SYSTEM_TICK;

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
            leds_colors_next[i].red = leds_current_anim->colors[next_id][i].red << BRIGHTNESS_SHIFT;
            leds_colors_next[i].green = leds_current_anim->colors[next_id][i].green << BRIGHTNESS_SHIFT;
            leds_colors_next[i].blue = leds_current_anim->colors[next_id][i].blue << BRIGHTNESS_SHIFT;
        } else if (leds_anim_queue_ids[0] != LEDS_ID_NO_ANIM) {
            // Or, we could be about to transition to the next non-ambient animation
            //  in the queue.
            leds_colors_next[i].red = all_anims[leds_anim_queue_ids[0]]->colors[0][i].red << BRIGHTNESS_SHIFT;
            leds_colors_next[i].green = all_anims[leds_anim_queue_ids[0]]->colors[0][i].green << BRIGHTNESS_SHIFT;
            leds_colors_next[i].blue = all_anims[leds_anim_queue_ids[0]]->colors[0][i].blue << BRIGHTNESS_SHIFT;
        } else {
            // Or, we're going back to ambient after this.
            leds_colors_next[i].red = all_anims[leds_ambient_anim_id]->colors[0][i].red << BRIGHTNESS_SHIFT;
            leds_colors_next[i].green = all_anims[leds_ambient_anim_id]->colors[0][i].green << BRIGHTNESS_SHIFT;
            leds_colors_next[i].blue = all_anims[leds_ambient_anim_id]->colors[0][i].blue << BRIGHTNESS_SHIFT;
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
        if (leds_current_anim == all_anims[ANIM_U00]) {
            // If this is the quaternary counter animation, do something special.
            r = (leds_anim_frame & (BIT0 << (stoplight_index*3 + 0))) ? 255 << BRIGHTNESS_SHIFT : 0;
            g = (leds_anim_frame & (BIT0 << (stoplight_index*3 + 1))) ? 255 << BRIGHTNESS_SHIFT : 0;
            b = (leds_anim_frame & (BIT0 << (stoplight_index*3 + 2))) ? 255 << BRIGHTNESS_SHIFT : 0;
        } else {
            r = colors[stoplight_index].red;
            g = colors[stoplight_index].green;
            b = colors[stoplight_index].blue;
        }

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

/// Set up the hold and transition steps for the current frame, and load the colors.
void leds_set_steps_and_go() {
    if (leds_current_anim == all_anims[ANIM_U00]) {
        // Super special case.
        leds_hold_steps = leds_current_anim->durations[0] / ANIM_DUR_UNIT_PER_SYSTEM_TICK;
        leds_hold_index = 0;
        leds_transition_steps = leds_current_anim->fade_durs[0] / ANIM_DUR_UNIT_PER_SYSTEM_TICK;
        leds_transition_index = 0;
        leds_anim_frame_quaternary_saved = leds_anim_frame;
        return;
    }
    leds_hold_steps = leds_current_anim->durations[leds_anim_frame] / ANIM_DUR_UNIT_PER_SYSTEM_TICK;
    leds_hold_index = 0;
    leds_transition_steps = leds_current_anim->fade_durs[leds_anim_frame] / ANIM_DUR_UNIT_PER_SYSTEM_TICK;
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
        leds_anim_length = leds_current_anim->len + 1 - badge_count_lights();
    } else if (animation == all_anims[ANIM_U00]) {
        leds_anim_length = 32768;
        leds_anim_frame = leds_anim_frame_quaternary_saved;
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
    // Alternatively, if clearqueue is set, we clobber the current animation anyway.
    if (clearqueue || leds_is_ambient) {
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
            target_index = LEDS_TWINKLE_STEPS_FAST/ANIM_DUR_UNIT_PER_SYSTEM_TICK;
        else
            target_index = LEDS_TWINKLE_STEPS_SLOW/ANIM_DUR_UNIT_PER_SYSTEM_TICK;

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
