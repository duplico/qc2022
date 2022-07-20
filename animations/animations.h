#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include "leds.h"

extern const rgbcolor_t h00geek_girl_frames[][4];
extern const uint16_t h00geek_girl_durations[];
extern const uint16_t h00geek_girl_fade_durs[];
extern const leds_animation_t h00geek_girl;
extern const rgbcolor_t h01lagoon_frames[][4];
extern const uint16_t h01lagoon_durations[];
extern const uint16_t h01lagoon_fade_durs[];
extern const leds_animation_t h01lagoon;
extern const rgbcolor_t h02fire_frames[][4];
extern const uint16_t h02fire_durations[];
extern const uint16_t h02fire_fade_durs[];
extern const leds_animation_t h02fire;
extern const rgbcolor_t h03ice_frames[][4];
extern const uint16_t h03ice_durations[];
extern const uint16_t h03ice_fade_durs[];
extern const leds_animation_t h03ice;
extern const rgbcolor_t h04rainbow_frames[][4];
extern const uint16_t h04rainbow_durations[];
extern const uint16_t h04rainbow_fade_durs[];
extern const leds_animation_t h04rainbow;
extern const rgbcolor_t h05zflag_leather_frames[][4];
extern const uint16_t h05zflag_leather_durations[];
extern const uint16_t h05zflag_leather_fade_durs[];
extern const leds_animation_t h05zflag_leather;
extern const rgbcolor_t h06zflag_trans_frames[][4];
extern const uint16_t h06zflag_trans_durations[];
extern const uint16_t h06zflag_trans_fade_durs[];
extern const leds_animation_t h06zflag_trans;
extern const rgbcolor_t h07pan_frames[][4];
extern const uint16_t h07pan_durations[];
extern const uint16_t h07pan_fade_durs[];
extern const leds_animation_t h07pan;
extern const rgbcolor_t h08zflag_bear_frames[][4];
extern const uint16_t h08zflag_bear_durations[];
extern const uint16_t h08zflag_bear_fade_durs[];
extern const leds_animation_t h08zflag_bear;
extern const rgbcolor_t h09wiggle_frames[][4];
extern const uint16_t h09wiggle_durations[];
extern const uint16_t h09wiggle_fade_durs[];
extern const leds_animation_t h09wiggle;
extern const rgbcolor_t h10white_frames[][4];
extern const uint16_t h10white_durations[];
extern const uint16_t h10white_fade_durs[];
extern const leds_animation_t h10white;
extern const rgbcolor_t h11rainbow2_frames[][4];
extern const uint16_t h11rainbow2_durations[];
extern const uint16_t h11rainbow2_fade_durs[];
extern const leds_animation_t h11rainbow2;
extern const rgbcolor_t meta_boop_frames[][4];
extern const uint16_t meta_boop_durations[];
extern const uint16_t meta_boop_fade_durs[];
extern const leds_animation_t meta_boop;
extern const rgbcolor_t meta_newpair_frames[][4];
extern const uint16_t meta_newpair_durations[];
extern const uint16_t meta_newpair_fade_durs[];
extern const leds_animation_t meta_newpair;
extern const rgbcolor_t meta_pair_frames[][4];
extern const uint16_t meta_pair_durations[];
extern const uint16_t meta_pair_fade_durs[];
extern const leds_animation_t meta_pair;
#define ANIM_COUNT 12
#define ANIM_COUNT_INCL_META 15
#define ANIM_H00GEEK_GIRL 0
#define ANIM_H01LAGOON 1
#define ANIM_H02FIRE 2
#define ANIM_H03ICE 3
#define ANIM_H04RAINBOW 4
#define ANIM_H05ZFLAG_LEATHER 5
#define ANIM_H06ZFLAG_TRANS 6
#define ANIM_H07PAN 7
#define ANIM_H08ZFLAG_BEAR 8
#define ANIM_H09WIGGLE 9
#define ANIM_H10WHITE 10
#define ANIM_H11RAINBOW2 11
#define ANIM_META_BOOP 12
#define ANIM_META_NEWPAIR 13
#define ANIM_META_PAIR 14
#define ANIM_TYPE_SOLID 0
#define ANIM_TYPE_FASTTWINKLE 1
extern const leds_animation_t *all_anims[15];
#endif // _H_
