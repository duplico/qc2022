#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include "leds.h"

extern const rgbcolor_t h00_frames[][5];
extern const uint16_t h00_durations[];
extern const uint16_t h00_fade_durs[];
extern const leds_animation_t h00;
extern const rgbcolor_t h01_frames[][5];
extern const uint16_t h01_durations[];
extern const uint16_t h01_fade_durs[];
extern const leds_animation_t h01;
extern const rgbcolor_t h02_frames[][5];
extern const uint16_t h02_durations[];
extern const uint16_t h02_fade_durs[];
extern const leds_animation_t h02;
extern const rgbcolor_t meta_newpair_frames[][5];
extern const uint16_t meta_newpair_durations[];
extern const uint16_t meta_newpair_fade_durs[];
extern const leds_animation_t meta_newpair;
extern const rgbcolor_t meta_pair_frames[][5];
extern const uint16_t meta_pair_durations[];
extern const uint16_t meta_pair_fade_durs[];
extern const leds_animation_t meta_pair;
extern const rgbcolor_t meta_z_brightness0_frames[][5];
extern const uint16_t meta_z_brightness0_durations[];
extern const uint16_t meta_z_brightness0_fade_durs[];
extern const leds_animation_t meta_z_brightness0;
extern const rgbcolor_t meta_z_brightness1_frames[][5];
extern const uint16_t meta_z_brightness1_durations[];
extern const uint16_t meta_z_brightness1_fade_durs[];
extern const leds_animation_t meta_z_brightness1;
extern const rgbcolor_t meta_z_brightness2_frames[][5];
extern const uint16_t meta_z_brightness2_durations[];
extern const uint16_t meta_z_brightness2_fade_durs[];
extern const leds_animation_t meta_z_brightness2;
#define ANIM_COUNT 3
#define ANIM_COUNT_INCL_META 8
#define ANIM_H00 0
#define ANIM_H01 1
#define ANIM_H02 2
#define ANIM_META_NEWPAIR 3
#define ANIM_META_PAIR 4
#define ANIM_META_Z_BRIGHTNESS0 5
#define ANIM_META_Z_BRIGHTNESS1 6
#define ANIM_META_Z_BRIGHTNESS2 7
#define ANIM_TYPE_SOLID 0
#define ANIM_TYPE_FASTTWINKLE 1
extern const leds_animation_t *all_anims[8];
#endif // _H_
