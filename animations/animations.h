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
extern const rgbcolor_t meta_newuber_frames[][5];
extern const uint16_t meta_newuber_durations[];
extern const uint16_t meta_newuber_fade_durs[];
extern const leds_animation_t meta_newuber;
extern const rgbcolor_t meta_pair_frames[][5];
extern const uint16_t meta_pair_durations[];
extern const uint16_t meta_pair_fade_durs[];
extern const leds_animation_t meta_pair;
extern const rgbcolor_t meta_pairuber_frames[][5];
extern const uint16_t meta_pairuber_durations[];
extern const uint16_t meta_pairuber_fade_durs[];
extern const leds_animation_t meta_pairuber;
extern const rgbcolor_t meta_startup_fade_frames[][5];
extern const uint16_t meta_startup_fade_durations[];
extern const uint16_t meta_startup_fade_fade_durs[];
extern const leds_animation_t meta_startup_fade;
extern const rgbcolor_t meta_startup_spin_frames[][5];
extern const uint16_t meta_startup_spin_durations[];
extern const uint16_t meta_startup_spin_fade_durs[];
extern const leds_animation_t meta_startup_spin;
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
#define ANIM_COUNT_INCL_META 12
#define ANIM_H00 0
#define ANIM_H01 1
#define ANIM_H02 2
#define ANIM_META_NEWPAIR 3
#define ANIM_META_NEWUBER 4
#define ANIM_META_PAIR 5
#define ANIM_META_PAIRUBER 6
#define ANIM_META_STARTUP_FADE 7
#define ANIM_META_STARTUP_SPIN 8
#define ANIM_META_Z_BRIGHTNESS0 9
#define ANIM_META_Z_BRIGHTNESS1 10
#define ANIM_META_Z_BRIGHTNESS2 11
#define ANIM_TYPE_SOLID 0
#define ANIM_TYPE_FASTTWINKLE 1
#define ANIM_TYPE_SLOWTWINKLE 2
extern const leds_animation_t *all_anims[12];
#endif // _H_
