/*
 * leds.c
 *
 *  Created on: Oct 17, 2019
 *      Author: george
 */

#include <stdint.h>
#include <stdlib.h>

#include <msp430fr2512.h>
#include <driverlib.h>

#include "tlc5948a.h"

#include "leds.h"
#include "band_anims.h"

/*
 * The LED order is RIGHT TO LEFT. So, gs[0] is the rightmost LED,
 *  gs[3] is the leftmost, and gs[4] is the HEART!
 */

const rgbcolor16_t band_off[BAND_LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

rgbcolor16_t band_colors_curr[BAND_LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

rgbcolor16_t band_colors_next[BAND_LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

rgbdelta_t band_colors_step[BAND_LED_COUNT] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
};

rgbcolor_t heart_color_options[HEART_COLOR_COUNT] = {
    {0xff, 0x0, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x7f, 0x0}, {0x0, 0x40, 0x0},
    {0x0, 0x0, 0xff}, {0x80, 0x0, 0x80}, {0xff, 0x80, 0xff}, {0x32, 0x66, 0x32},
    {0x66, 0x66, 0xaa}, {0x4b, 0x0, 0x82}, {0xda, 0x52, 0x20}, {0xc5, 0x20, 0x4},
    {0xf0, 0x14, 0x3c}, {0x6a, 0x2d, 0xcd}, {0x0, 0x5f, 0xff}, {0xc7, 0xa, 0x85},
};

/// Does the heart LED need re-display to the LED driver?
uint8_t heart_dirty = 1;
/// Does the band display need re-sent to the LED driver?
uint8_t band_dirty = 1;

uint8_t band_animation_state = 0;

rgbcolor16_t heart_color_curr;
rgbcolor_t heart_color = {
    0xff, 0x0f, 0x0f
};
rgbdelta_t heart_color_step;
uint8_t heart_frame_index;

uint8_t heart_state;
uint8_t heart_is_boop;

uint8_t band_anim_frame;
uint8_t band_anim_id;
uint8_t band_saved_anim_id;
uint8_t band_is_ambient = 1;
uint8_t band_anim_looping;
uint8_t band_anim_length;
uint16_t band_hold_steps;
uint16_t band_hold_index;
uint16_t band_transition_steps;
uint16_t band_transition_index;
const band_animation_t *band_current_anim;

uint8_t current_ambient_correct;
uint8_t previous_ambient_correct;

uint8_t band_twinkle_bits = 0xea;
uint16_t band_anim_adjustment_index = 0;

/// Start a new frame, setting up current, source, dest, and step.
void band_load_colors() {
    // leg_colors_curr <- tentacle_current_anim[tentacle_anim_index]

    // Stage in the current color:
    for (uint8_t i=0; i<BAND_LED_COUNT; i++) {
        band_colors_curr[i].red = band_current_anim->colors[band_anim_frame][i].red * 5;
        band_colors_curr[i].green = band_current_anim->colors[band_anim_frame][i].green * 5;
        band_colors_curr[i].blue = band_current_anim->colors[band_anim_frame][i].blue * 5;


        // Stage in the next color:
        // If we're looping, it's modded. If not looping, back to black.
        if (band_anim_frame == band_current_anim->len-1 && !(band_anim_looping || band_is_ambient)) {
            band_colors_next[i].red = 0;
            band_colors_next[i].green = 0;
            band_colors_next[i].blue = 0;
        } else {
            uint8_t next_id = (band_anim_frame+1) % band_current_anim->len;
            band_colors_next[i].red = band_current_anim->colors[next_id][i].red * 5;
            band_colors_next[i].green = band_current_anim->colors[next_id][i].green * 5;
            band_colors_next[i].blue = band_current_anim->colors[next_id][i].blue * 5;
        }

        band_colors_step[i].red = ((int_fast32_t) band_colors_next[i].red - band_colors_curr[i].red) / band_transition_steps;
        band_colors_step[i].green = ((int_fast32_t) band_colors_next[i].green - band_colors_curr[i].green) / band_transition_steps;
        band_colors_step[i].blue = ((int_fast32_t) band_colors_next[i].blue - band_colors_curr[i].blue) / band_transition_steps;
    }
}

/// Do a fading step within a frame, stepping colors by their step values.
inline void band_fade_colors() {
    // If this is the very last transition step,
    if (band_transition_steps && band_transition_index == band_transition_steps-1) {
        // hit the destination:
        memcpy(band_colors_curr, band_colors_next,
               sizeof(rgbcolor_t) * BAND_LED_COUNT);
    } else {
        for (uint8_t i = 0; i < BAND_LED_COUNT; i++)
        {
            band_colors_curr[i].red += band_colors_step[i].red;
            band_colors_curr[i].green += band_colors_step[i].green;
            band_colors_curr[i].blue += band_colors_step[i].blue;
        }
    }
}

/// Actually apply the appropriate colors to our grayscale values.
/**
 ** This also handles special cases, like twinkling.
 **/
void set_band_gs(const rgbcolor16_t* band_colors) {
    static uint_fast32_t r = 0;
    static uint_fast32_t g = 0;
    static uint_fast32_t b = 0;

    for (uint8_t flower_index = 0; flower_index < BAND_LED_COUNT; flower_index++)
    {
        r = band_colors[flower_index].red << current_ambient_correct;
        g = band_colors[flower_index].green << current_ambient_correct;
        b = band_colors[flower_index].blue << current_ambient_correct;

        if (band_current_anim->anim_type != ANIM_TYPE_SOLID) {
            if (band_twinkle_bits & (1 << flower_index)) {
                r = r >> 2;
                g = g >> 2;
                b = b >> 2;
            }
        }

        if (r>UINT16_MAX) r=UINT16_MAX;
        if (g>UINT16_MAX) g=UINT16_MAX;
        if (b>UINT16_MAX) b=UINT16_MAX;

        tlc_gs[3+1+(3-flower_index)*3] = b;
        tlc_gs[3+2+(3-flower_index)*3] = g;
        tlc_gs[3+3+(3-flower_index)*3] = r;
    }
}

void band_set_steps_and_go() {
    band_hold_steps = band_current_anim->durations[band_anim_frame] / LEGS_DUR_STEP;
    band_hold_index = 0;
    band_transition_steps = band_current_anim->fade_durs[band_anim_frame] / LEGS_DUR_STEP;
    band_transition_index = 0;

    band_load_colors();
}

/// Launch a new headband animation, possibly as the new ambient one.
/**
 ** This should generally not be called directly by the application logic,
 **  but rather indirectly by invoking `band_start_anim_by_id`.
 */
void band_start_anim_by_struct(const band_animation_t *animation, uint8_t loop, uint8_t ambient) {
    // If ambient=1, then a bit of pre-work is needed.
    // If we've been asked to do an interrupting animation, remember what our ambient anim
    //  was so we can go back to it.
    if (!ambient && band_is_ambient) {
        band_saved_anim_id = band_anim_id;
    }

    band_is_ambient = ambient;

    band_current_anim = animation;

    band_anim_frame = 0; // This is our frame index in the animation.
    band_animation_state = 1; // animating

    band_anim_adjustment_index = 0;

    band_anim_looping = loop;
    band_anim_length = band_current_anim->len;

    band_set_steps_and_go();
    set_band_gs(band_colors_curr);
    band_dirty = 1;
}

void band_start_anim_by_id(uint8_t anim_id, uint8_t loop, uint8_t ambient) {
    if (ambient && !band_is_ambient) {
        // If we've been asked to switch our ambient animation, but we're currently in an
        //  interrupting animation, we need to change what we have saved so we go back to
        //  the new ambient animation.
        band_saved_anim_id = anim_id;
        return;
    }

    band_start_anim_by_struct(band_all_anims[anim_id], loop, ambient);

    band_anim_id = anim_id;
}

/// Load and display the next animation frame in the current animation.
void band_next_anim_frame() {
    band_anim_frame++; // This is our index in the animation.

    if (band_anim_frame >= band_anim_length) { // rolled over.
        if (band_is_ambient) { // we're ambient. (loop is dontcare)
            band_anim_frame = 0; // start over from beginning.
        } else if (band_anim_looping) { // not ambient; loops remaining
            band_anim_frame = 0;
            band_anim_looping--;
        } else { // not ambient, no loops remaining
            band_is_ambient = 1; // Now we're back to being ambient...
            current_ambient_correct = 1;
            band_start_anim_by_id(band_saved_anim_id, 0, 1);
            return; // skip the transitions_and_go because that's called in start_anim.
        }
    }

    band_set_steps_and_go();
}

/// Do a time step of the LED animation system.
void leds_timestep() {
    // If the ambient light correction has changed, mark everything dirty.
    if (current_ambient_correct != previous_ambient_correct) {
        previous_ambient_correct = current_ambient_correct;
        heart_dirty = 1;
        band_dirty = 1;
    }

    // Handle the heart.
    if (heart_state) {
        if (heart_frame_index == 0 || heart_frame_index == 24) {
            if (heart_is_boop) {
                heart_color_curr.red = heart_color.red << 6;
                heart_color_curr.green = heart_color.green << 6;
                heart_color_curr.blue = heart_color.blue << 6;
            } else {
                heart_color_curr.red = heart_color.red << (1 + heart_state / 16);
                heart_color_curr.green = heart_color.green << (1 + heart_state / 16);
                heart_color_curr.blue = heart_color.blue << (1 + heart_state / 16);
            }
            heart_frame_index++;
        } else if (heart_frame_index == 80) {
            heart_frame_index = 0;
            heart_state--;
            if (!heart_state) {
                heart_is_boop = 0;
            }
        } else {
            heart_color_curr.red >>= 1;
            heart_color_curr.green >>= 1;
            heart_color_curr.blue >>= 1;
            heart_frame_index++;
        }

        heart_dirty = 1;
    }

    if (heart_dirty) {
        tlc_gs[3] = heart_color_curr.red;
        tlc_gs[2] = heart_color_curr.green;
        tlc_gs[1] = heart_color_curr.blue;
    }

    // Handle the band:
    //  Apply our current delta animation timestep.
    if (band_current_anim->anim_type != ANIM_TYPE_SOLID) {
        uint16_t target_index;
        if (band_current_anim->anim_type == ANIM_TYPE_FASTTWINKLE)
            target_index = BAND_TWINKLE_STEPS_FAST/LEGS_DUR_STEP;
        else
            target_index = BAND_TWINKLE_STEPS_SLOW/LEGS_DUR_STEP;

        if (band_anim_adjustment_index == target_index) {
            band_twinkle_bits = rand() % 256;
            band_anim_adjustment_index = 0;
            band_dirty = 1;
        } else {
            band_anim_adjustment_index++;
        }
    }

    if (band_animation_state) {
        if (band_hold_index < band_hold_steps) {
            // Hold the same color.
            band_hold_index++;
        } else {
            // We're transitioning:
            band_transition_index++;
            if (band_transition_index >= band_transition_steps) {
                // next frame plx.
                band_next_anim_frame();
                band_dirty = 1;
            } else {
                band_fade_colors();
                band_dirty = 1;
            }
        }
    }

    if (band_dirty) {
        set_band_gs(band_colors_curr);
    }

    // If either the heart or the band was updated,
    if (band_dirty || heart_dirty) {
        // then re-send to the LED driver.
        tlc_set_gs();
        band_dirty = heart_dirty = 0;
    }

}
