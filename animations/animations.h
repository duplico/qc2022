#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include "leds.h"

extern const rgbcolor_t b00_frames[][5];
extern const uint16_t b00_durations[];
extern const uint16_t b00_fade_durs[];
extern const leds_animation_t b00;
extern const rgbcolor_t b01_frames[][5];
extern const uint16_t b01_durations[];
extern const uint16_t b01_fade_durs[];
extern const leds_animation_t b01;
extern const rgbcolor_t b02_frames[][5];
extern const uint16_t b02_durations[];
extern const uint16_t b02_fade_durs[];
extern const leds_animation_t b02;
extern const rgbcolor_t b03_frames[][5];
extern const uint16_t b03_durations[];
extern const uint16_t b03_fade_durs[];
extern const leds_animation_t b03;
extern const rgbcolor_t b04_frames[][5];
extern const uint16_t b04_durations[];
extern const uint16_t b04_fade_durs[];
extern const leds_animation_t b04;
extern const rgbcolor_t b05_frames[][5];
extern const uint16_t b05_durations[];
extern const uint16_t b05_fade_durs[];
extern const leds_animation_t b05;
extern const rgbcolor_t b06_frames[][5];
extern const uint16_t b06_durations[];
extern const uint16_t b06_fade_durs[];
extern const leds_animation_t b06;
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
extern const rgbcolor_t h03_frames[][5];
extern const uint16_t h03_durations[];
extern const uint16_t h03_fade_durs[];
extern const leds_animation_t h03;
extern const rgbcolor_t h04_frames[][5];
extern const uint16_t h04_durations[];
extern const uint16_t h04_fade_durs[];
extern const leds_animation_t h04;
extern const rgbcolor_t h05_frames[][5];
extern const uint16_t h05_durations[];
extern const uint16_t h05_fade_durs[];
extern const leds_animation_t h05;
extern const rgbcolor_t h06_frames[][5];
extern const uint16_t h06_durations[];
extern const uint16_t h06_fade_durs[];
extern const leds_animation_t h06;
extern const rgbcolor_t h07_frames[][5];
extern const uint16_t h07_durations[];
extern const uint16_t h07_fade_durs[];
extern const leds_animation_t h07;
extern const rgbcolor_t h08_frames[][5];
extern const uint16_t h08_durations[];
extern const uint16_t h08_fade_durs[];
extern const leds_animation_t h08;
extern const rgbcolor_t h09_frames[][5];
extern const uint16_t h09_durations[];
extern const uint16_t h09_fade_durs[];
extern const leds_animation_t h09;
extern const rgbcolor_t meta_connects_frames[][5];
extern const uint16_t meta_connects_durations[];
extern const uint16_t meta_connects_fade_durs[];
extern const leds_animation_t meta_connects;
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
extern const rgbcolor_t s00_frames[][5];
extern const uint16_t s00_durations[];
extern const uint16_t s00_fade_durs[];
extern const leds_animation_t s00;
extern const rgbcolor_t s01_frames[][5];
extern const uint16_t s01_durations[];
extern const uint16_t s01_fade_durs[];
extern const leds_animation_t s01;
extern const rgbcolor_t s02_frames[][5];
extern const uint16_t s02_durations[];
extern const uint16_t s02_fade_durs[];
extern const leds_animation_t s02;
extern const rgbcolor_t s03_frames[][5];
extern const uint16_t s03_durations[];
extern const uint16_t s03_fade_durs[];
extern const leds_animation_t s03;
extern const rgbcolor_t u00_frames[][5];
extern const uint16_t u00_durations[];
extern const uint16_t u00_fade_durs[];
extern const leds_animation_t u00;
extern const rgbcolor_t u01_frames[][5];
extern const uint16_t u01_durations[];
extern const uint16_t u01_fade_durs[];
extern const leds_animation_t u01;
extern const rgbcolor_t u02_frames[][5];
extern const uint16_t u02_durations[];
extern const uint16_t u02_fade_durs[];
extern const leds_animation_t u02;
#define ANIM_COUNT 24
#define ANIM_COUNT_INCL_META 34
#define ANIM_B00 0
#define ANIM_B01 1
#define ANIM_B02 2
#define ANIM_B03 3
#define ANIM_B04 4
#define ANIM_B05 5
#define ANIM_B06 6
#define ANIM_H00 7
#define ANIM_H01 8
#define ANIM_H02 9
#define ANIM_H03 10
#define ANIM_H04 11
#define ANIM_H05 12
#define ANIM_H06 13
#define ANIM_H07 14
#define ANIM_H08 15
#define ANIM_H09 16
#define ANIM_S00 17
#define ANIM_S01 18
#define ANIM_S02 19
#define ANIM_S03 20
#define ANIM_U00 21
#define ANIM_U01 22
#define ANIM_U02 23
#define ANIM_META_CONNECTS 24
#define ANIM_META_NEWPAIR 25
#define ANIM_META_NEWUBER 26
#define ANIM_META_PAIR 27
#define ANIM_META_PAIRUBER 28
#define ANIM_META_STARTUP_FADE 29
#define ANIM_META_STARTUP_SPIN 30
#define ANIM_META_Z_BRIGHTNESS0 31
#define ANIM_META_Z_BRIGHTNESS1 32
#define ANIM_META_Z_BRIGHTNESS2 33
#define ANIM_TYPE_SOLID 0
#define ANIM_TYPE_SLOWTWINKLE 1
#define ANIM_TYPE_FASTTWINKLE 2
extern const leds_animation_t *all_anims[34];
#endif // _H_
