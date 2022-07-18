#ifndef BANDANIMS_H_
#define BANDANIMS_H_

#include "leds.h"

#define LEG_CAMO_INDEX 0
#define LEG_INK_INDEX 1
#define LEG_DOUBLEINK_INDEX 2
#define LEG_ANIM_TYPE_NONE 201
#define LEG_ANIM_NONE 211

// frames for the band:
extern const rgbcolor_t h00geek_girl_band_frames[][4];
extern const uint16_t h00geek_girl_band_durations[];
extern const uint16_t h00geek_girl_band_fade_durs[];
extern const leds_animation_t h00geek_girl;
// frames for the band:
extern const rgbcolor_t h01lagoon_band_frames[][4];
extern const uint16_t h01lagoon_band_durations[];
extern const uint16_t h01lagoon_band_fade_durs[];
extern const leds_animation_t h01lagoon;
// frames for the band:
extern const rgbcolor_t h02fire_band_frames[][4];
extern const uint16_t h02fire_band_durations[];
extern const uint16_t h02fire_band_fade_durs[];
extern const leds_animation_t h02fire;
// frames for the band:
extern const rgbcolor_t h03ice_band_frames[][4];
extern const uint16_t h03ice_band_durations[];
extern const uint16_t h03ice_band_fade_durs[];
extern const leds_animation_t h03ice;
// frames for the band:
extern const rgbcolor_t h04rainbow_band_frames[][4];
extern const uint16_t h04rainbow_band_durations[];
extern const uint16_t h04rainbow_band_fade_durs[];
extern const leds_animation_t h04rainbow;
// frames for the band:
extern const rgbcolor_t h05zflag_leather_band_frames[][4];
extern const uint16_t h05zflag_leather_band_durations[];
extern const uint16_t h05zflag_leather_band_fade_durs[];
extern const leds_animation_t h05zflag_leather;
// frames for the band:
extern const rgbcolor_t h06zflag_trans_band_frames[][4];
extern const uint16_t h06zflag_trans_band_durations[];
extern const uint16_t h06zflag_trans_band_fade_durs[];
extern const leds_animation_t h06zflag_trans;
// frames for the band:
extern const rgbcolor_t h07pan_band_frames[][4];
extern const uint16_t h07pan_band_durations[];
extern const uint16_t h07pan_band_fade_durs[];
extern const leds_animation_t h07pan;
// frames for the band:
extern const rgbcolor_t h08zflag_bear_band_frames[][4];
extern const uint16_t h08zflag_bear_band_durations[];
extern const uint16_t h08zflag_bear_band_fade_durs[];
extern const leds_animation_t h08zflag_bear;
// frames for the band:
extern const rgbcolor_t h09wiggle_band_frames[][4];
extern const uint16_t h09wiggle_band_durations[];
extern const uint16_t h09wiggle_band_fade_durs[];
extern const leds_animation_t h09wiggle;
// frames for the band:
extern const rgbcolor_t h10white_band_frames[][4];
extern const uint16_t h10white_band_durations[];
extern const uint16_t h10white_band_fade_durs[];
extern const leds_animation_t h10white;
// frames for the band:
extern const rgbcolor_t h11rainbow2_band_frames[][4];
extern const uint16_t h11rainbow2_band_durations[];
extern const uint16_t h11rainbow2_band_fade_durs[];
extern const leds_animation_t h11rainbow2;
// frames for the band:
extern const rgbcolor_t meta_boop_band_frames[][4];
extern const uint16_t meta_boop_band_durations[];
extern const uint16_t meta_boop_band_fade_durs[];
extern const leds_animation_t meta_boop;
// frames for the band:
extern const rgbcolor_t meta_newpair_band_frames[][4];
extern const uint16_t meta_newpair_band_durations[];
extern const uint16_t meta_newpair_band_fade_durs[];
extern const leds_animation_t meta_newpair;
// frames for the band:
extern const rgbcolor_t meta_pair_band_frames[][4];
extern const uint16_t meta_pair_band_durations[];
extern const uint16_t meta_pair_band_fade_durs[];
extern const leds_animation_t meta_pair;
#define HEAD_ANIM_COUNT 12
#define HEAD_ANIM_COUNT_INCL_META 15
#define HEAD_ANIM_H00GEEK_GIRL 0
#define HEAD_ANIM_H01LAGOON 1
#define HEAD_ANIM_H02FIRE 2
#define HEAD_ANIM_H03ICE 3
#define HEAD_ANIM_H04RAINBOW 4
#define HEAD_ANIM_H05ZFLAG_LEATHER 5
#define HEAD_ANIM_H06ZFLAG_TRANS 6
#define HEAD_ANIM_H07PAN 7
#define HEAD_ANIM_H08ZFLAG_BEAR 8
#define HEAD_ANIM_H09WIGGLE 9
#define HEAD_ANIM_H10WHITE 10
#define HEAD_ANIM_H11RAINBOW2 11
#define HEAD_ANIM_META_BOOP 12
#define HEAD_ANIM_META_NEWPAIR 13
#define HEAD_ANIM_META_PAIR 14
#define ANIM_TYPE_SOLID 0
#define ANIM_TYPE_FASTTWINKLE 1
#define LEG_ANIM_TYPE_COUNT 2
extern const leds_animation_t *band_all_anims[15];
#endif // _H_
