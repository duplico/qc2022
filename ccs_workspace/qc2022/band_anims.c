#include "leds.h"
#include "band_anims.h"

///////////////// H00GEEK_GIRL:
// frames for the band:
const rgbcolor_t h00geek_girl_band_frames[][4] = {
    {{0xf0, 0x14, 0x3c}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0xf0, 0x14, 0x3c}},
    {{0x0, 0x40, 0x0}, {0xf0, 0x14, 0x3c}, {0xf0, 0x14, 0x3c}, {0x0, 0x40, 0x0}},
};
const uint16_t h00geek_girl_band_durations[] = {400, 400};
const uint16_t h00geek_girl_band_fade_durs[] = {200, 200};
// the animation:
const leds_animation_t h00geek_girl = {h00geek_girl_band_frames, h00geek_girl_band_durations, h00geek_girl_band_fade_durs, 2, ANIM_TYPE_SOLID, 5};

///////////////// H01LAGOON:
// frames for the band:
const rgbcolor_t h01lagoon_band_frames[][4] = {
    {{0x0, 0x5f, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x5f, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x5f, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x5f, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x40, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}},
    {{0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x40, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}},
};
const uint16_t h01lagoon_band_durations[] = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 400};
const uint16_t h01lagoon_band_fade_durs[] = {200, 200, 200, 200, 200, 200, 200, 200, 200, 600, 200};
// the animation:
const leds_animation_t h01lagoon = {h01lagoon_band_frames, h01lagoon_band_durations, h01lagoon_band_fade_durs, 11, ANIM_TYPE_SOLID, 1};

///////////////// H02FIRE:
// frames for the band:
const rgbcolor_t h02fire_band_frames[][4] = {
    {{0xff, 0x0, 0x0}, {0xff, 0x22, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x0, 0x0}},
    {{0xff, 0x22, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x22, 0x0}, {0xff, 0x46, 0x0}},
    {{0xff, 0x46, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x22, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x22, 0x0}, {0xff, 0x46, 0x0}},
    {{0xff, 0x46, 0x0}, {0xff, 0x22, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x22, 0x0}},
};
const uint16_t h02fire_band_durations[] = {100, 100, 100, 100, 100};
const uint16_t h02fire_band_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t h02fire = {h02fire_band_frames, h02fire_band_durations, h02fire_band_fade_durs, 5, ANIM_TYPE_FASTTWINKLE, 8};

///////////////// H03ICE:
// frames for the band:
const rgbcolor_t h03ice_band_frames[][4] = {
    {{0x0, 0x0, 0xff}, {0x20, 0x59, 0xaa}, {0x0, 0x5f, 0xff}, {0x0, 0x0, 0xff}},
    {{0x20, 0x59, 0xaa}, {0x0, 0x5f, 0xff}, {0x20, 0x59, 0xaa}, {0x0, 0x5f, 0xff}},
    {{0x0, 0x5f, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x5f, 0xff}, {0x20, 0x59, 0xaa}},
    {{0x0, 0x0, 0xff}, {0x0, 0x5f, 0xff}, {0x20, 0x59, 0xaa}, {0x0, 0x5f, 0xff}},
    {{0x0, 0x5f, 0xff}, {0x20, 0x59, 0xaa}, {0x0, 0x0, 0xff}, {0x20, 0x59, 0xaa}},
};
const uint16_t h03ice_band_durations[] = {100, 100, 100, 100, 100};
const uint16_t h03ice_band_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t h03ice = {h03ice_band_frames, h03ice_band_durations, h03ice_band_fade_durs, 5, ANIM_TYPE_FASTTWINKLE, 8};

///////////////// H04RAINBOW:
// frames for the band:
const rgbcolor_t h04rainbow_band_frames[][4] = {
    {{0xff, 0x0, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x7f, 0x0}, {0x0, 0x40, 0x0}},
    {{0xff, 0x46, 0x0}, {0xff, 0x7f, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x0, 0xff}},
    {{0xff, 0x7f, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x0, 0xff}, {0x80, 0x0, 0x80}},
    {{0x0, 0x40, 0x0}, {0x0, 0x0, 0xff}, {0x80, 0x0, 0x80}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0x80, 0x0, 0x80}, {0xff, 0x0, 0x0}, {0xff, 0x46, 0x0}},
    {{0x80, 0x0, 0x80}, {0xff, 0x0, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x7f, 0x0}},
};
const uint16_t h04rainbow_band_durations[] = {100, 100, 100, 100, 100, 100};
const uint16_t h04rainbow_band_fade_durs[] = {250, 250, 250, 250, 250, 250};
// the animation:
const leds_animation_t h04rainbow = {h04rainbow_band_frames, h04rainbow_band_durations, h04rainbow_band_fade_durs, 6, ANIM_TYPE_SOLID, 2};

///////////////// H05ZFLAG_LEATHER:
// frames for the band:
const rgbcolor_t h05zflag_leather_band_frames[][4] = {
    {{0x0, 0x0, 0xff}, {0xff, 0x7f, 0xff}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0xff, 0x7f, 0xff}, {0x0, 0x0, 0xff}, {0xf, 0x0, 0x0}, {0xff, 0x7f, 0xff}},
    {{0x0, 0x0, 0x0}, {0xff, 0x7f, 0xff}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0xf, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0xff, 0x7f, 0xff}, {0x0, 0x0, 0xff}, {0xf, 0x0, 0x0}, {0xff, 0x7f, 0xff}},
};
const uint16_t h05zflag_leather_band_durations[] = {200, 200, 200, 200, 200, 200, 200, 200};
const uint16_t h05zflag_leather_band_fade_durs[] = {500, 500, 500, 500, 500, 500, 500, 500};
// the animation:
const leds_animation_t h05zflag_leather = {h05zflag_leather_band_frames, h05zflag_leather_band_durations, h05zflag_leather_band_fade_durs, 8, ANIM_TYPE_SOLID, 1};

///////////////// H06ZFLAG_TRANS:
// frames for the band:
const rgbcolor_t h06zflag_trans_band_frames[][4] = {
    {{0x0, 0x5f, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x5f, 0xff}},
    {{0xf0, 0x14, 0x3c}, {0x0, 0x5f, 0xff}, {0x0, 0x5f, 0xff}, {0xf0, 0x14, 0x3c}},
    {{0xff, 0x7f, 0xff}, {0xf0, 0x14, 0x3c}, {0xf0, 0x14, 0x3c}, {0xff, 0x7f, 0xff}},
    {{0xf0, 0x14, 0x3c}, {0xff, 0x7f, 0xff}, {0xff, 0x7f, 0xff}, {0xf0, 0x14, 0x3c}},
    {{0x0, 0x5f, 0xff}, {0xf0, 0x14, 0x3c}, {0xf0, 0x14, 0x3c}, {0x0, 0x5f, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x5f, 0xff}, {0x0, 0x5f, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h06zflag_trans_band_durations[] = {150, 150, 150, 150, 150, 150, 150};
const uint16_t h06zflag_trans_band_fade_durs[] = {140, 140, 140, 140, 140, 140, 140};
// the animation:
const leds_animation_t h06zflag_trans = {h06zflag_trans_band_frames, h06zflag_trans_band_durations, h06zflag_trans_band_fade_durs, 7, ANIM_TYPE_SOLID, 2};

///////////////// H07PAN:
// frames for the band:
const rgbcolor_t h07pan_band_frames[][4] = {
    {{0xff, 0x21, 0x8c}, {0xff, 0xd8, 0x0}, {0x21, 0xb1, 0xff}, {0xff, 0x21, 0x8c}},
    {{0xff, 0xd8, 0x0}, {0x21, 0xb1, 0xff}, {0xff, 0xd8, 0x0}, {0x21, 0xb1, 0xff}},
    {{0x21, 0xb1, 0xff}, {0xff, 0x21, 0x8c}, {0x21, 0xb1, 0xff}, {0xff, 0xd8, 0x0}},
    {{0xff, 0x21, 0x8c}, {0x21, 0xb1, 0xff}, {0xff, 0xd8, 0x0}, {0x21, 0xb1, 0xff}},
    {{0x21, 0xb1, 0xff}, {0xff, 0xd8, 0x0}, {0xff, 0x21, 0x8c}, {0xff, 0xd8, 0x0}},
};
const uint16_t h07pan_band_durations[] = {100, 100, 100, 100, 100};
const uint16_t h07pan_band_fade_durs[] = {99, 99, 99, 99, 99};
// the animation:
const leds_animation_t h07pan = {h07pan_band_frames, h07pan_band_durations, h07pan_band_fade_durs, 5, ANIM_TYPE_SOLID, 6};

///////////////// H08ZFLAG_BEAR:
// frames for the band:
const rgbcolor_t h08zflag_bear_band_frames[][4] = {
    {{0xc5, 0x20, 0x4}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xc5, 0x20, 0x4}},
    {{0xc5, 0x20, 0x4}, {0xc5, 0x20, 0x4}, {0xc5, 0x20, 0x4}, {0xc5, 0x20, 0x4}},
    {{0xd5, 0x63, 0x0}, {0xc5, 0x20, 0x4}, {0xc5, 0x20, 0x4}, {0xd5, 0x63, 0x0}},
    {{0xd5, 0x63, 0x0}, {0xd5, 0x63, 0x0}, {0xd5, 0x63, 0x0}, {0xd5, 0x63, 0x0}},
    {{0xd5, 0x63, 0x0}, {0xd5, 0x63, 0x0}, {0xd5, 0x63, 0x0}, {0xd5, 0x63, 0x0}},
    {{0xc5, 0x20, 0x4}, {0xd5, 0x63, 0x0}, {0xd5, 0x63, 0x0}, {0xc5, 0x20, 0x4}},
    {{0x0, 0x0, 0x0}, {0xc5, 0x20, 0x4}, {0xc5, 0x20, 0x4}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h08zflag_bear_band_durations[] = {50, 50, 50, 50, 50, 50, 50, 50};
const uint16_t h08zflag_bear_band_fade_durs[] = {180, 180, 180, 180, 180, 180, 180, 180};
// the animation:
const leds_animation_t h08zflag_bear = {h08zflag_bear_band_frames, h08zflag_bear_band_durations, h08zflag_bear_band_fade_durs, 8, ANIM_TYPE_SOLID, 3};

///////////////// H09WIGGLE:
// frames for the band:
const rgbcolor_t h09wiggle_band_frames[][4] = {
    {{0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}, {0x0, 0x5f, 0xff}, {0x0, 0x5f, 0xff}},
    {{0x0, 0x5f, 0xff}, {0x0, 0x5f, 0xff}, {0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}},
};
const uint16_t h09wiggle_band_durations[] = {400, 400};
const uint16_t h09wiggle_band_fade_durs[] = {200, 200};
// the animation:
const leds_animation_t h09wiggle = {h09wiggle_band_frames, h09wiggle_band_durations, h09wiggle_band_fade_durs, 2, ANIM_TYPE_SOLID, 5};

///////////////// H10WHITE:
// frames for the band:
const rgbcolor_t h10white_band_frames[][4] = {
    {{0xff, 0x7f, 0xff}, {0xff, 0x7f, 0xff}, {0xff, 0x7f, 0xff}, {0xff, 0x7f, 0xff}},
};
const uint16_t h10white_band_durations[] = {100};
const uint16_t h10white_band_fade_durs[] = {100};
// the animation:
const leds_animation_t h10white = {h10white_band_frames, h10white_band_durations, h10white_band_fade_durs, 1, ANIM_TYPE_FASTTWINKLE, 30};

///////////////// H11RAINBOW2:
// frames for the band:
const rgbcolor_t h11rainbow2_band_frames[][4] = {
    {{0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x7f, 0xff}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}},
    {{0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x7f, 0xff}},
    {{0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}, {0xff, 0x46, 0x0}},
    {{0xff, 0x7f, 0x0}, {0xff, 0x7f, 0x0}, {0xff, 0x7f, 0x0}, {0xff, 0x7f, 0x0}},
    {{0xff, 0x7f, 0xff}, {0xff, 0x7f, 0x0}, {0xff, 0x7f, 0x0}, {0xff, 0x7f, 0x0}},
    {{0xff, 0x7f, 0x0}, {0xff, 0x7f, 0x0}, {0xff, 0x7f, 0x0}, {0xff, 0x7f, 0x0}},
    {{0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}},
    {{0x0, 0x40, 0x0}, {0xff, 0x7f, 0xff}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}},
    {{0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x40, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0xff, 0x7f, 0xff}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}},
    {{0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}},
    {{0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0xff, 0x7f, 0xff}},
    {{0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}},
};
const uint16_t h11rainbow2_band_durations[] = {40, 40, 80, 40, 40, 80, 40, 40, 80, 40, 40, 80, 40, 40, 80, 40, 40, 80};
const uint16_t h11rainbow2_band_fade_durs[] = {40, 40, 125, 40, 40, 125, 40, 40, 125, 40, 40, 125, 40, 40, 125, 40, 40, 125};
// the animation:
const leds_animation_t h11rainbow2 = {h11rainbow2_band_frames, h11rainbow2_band_durations, h11rainbow2_band_fade_durs, 18, ANIM_TYPE_SOLID, 2};

///////////////// META_BOOP:
// frames for the band:
const rgbcolor_t meta_boop_band_frames[][4] = {
    {{0x22, 0x45, 0x22}, {0xda, 0x52, 0x20}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}, {0xb2, 0x11, 0x22}},
    {{0xff, 0x22, 0x0}, {0xda, 0x52, 0x20}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}},
    {{0xb2, 0x11, 0x22}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}},
    {{0x80, 0x0, 0x80}, {0x22, 0x45, 0x22}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0xda, 0x52, 0x20}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}, {0xb2, 0x11, 0x22}},
    {{0xff, 0x22, 0x0}, {0xda, 0x52, 0x20}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}},
    {{0xb2, 0x11, 0x22}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}},
    {{0x80, 0x0, 0x80}, {0x22, 0x45, 0x22}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0xda, 0x52, 0x20}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}, {0xb2, 0x11, 0x22}},
    {{0xff, 0x22, 0x0}, {0xda, 0x52, 0x20}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}},
    {{0xb2, 0x11, 0x22}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}},
    {{0x80, 0x0, 0x80}, {0x22, 0x45, 0x22}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0xda, 0x52, 0x20}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}, {0xb2, 0x11, 0x22}},
    {{0xff, 0x22, 0x0}, {0xda, 0x52, 0x20}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}},
    {{0xb2, 0x11, 0x22}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}},
    {{0x80, 0x0, 0x80}, {0x22, 0x45, 0x22}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0xda, 0x52, 0x20}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}, {0xb2, 0x11, 0x22}},
    {{0xff, 0x22, 0x0}, {0xda, 0x52, 0x20}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}},
    {{0xb2, 0x11, 0x22}, {0x22, 0x45, 0x22}, {0x20, 0x59, 0xaa}, {0x80, 0x0, 0x80}},
    {{0x80, 0x0, 0x80}, {0x22, 0x45, 0x22}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0x0, 0x0, 0x0}, {0xff, 0x22, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x22, 0x45, 0x22}, {0x0, 0x0, 0x0}, {0x80, 0x0, 0x80}, {0x0, 0x0, 0x0}},
    {{0xff, 0x22, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t meta_boop_band_durations[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1000};
const uint16_t meta_boop_band_fade_durs[] = {45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 750};
// the animation:
const leds_animation_t meta_boop = {meta_boop_band_frames, meta_boop_band_durations, meta_boop_band_fade_durs, 29, ANIM_TYPE_SOLID, 1};

///////////////// META_NEWPAIR:
// frames for the band:
const rgbcolor_t meta_newpair_band_frames[][4] = {
    {{0x0, 0x40, 0x0}, {0xff, 0x7f, 0x0}, {0xff, 0x52, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x40, 0x0}, {0x0, 0x0, 0xff}, {0x80, 0x0, 0x80}, {0xff, 0x0, 0x0}},
    {{0xff, 0x52, 0x0}, {0xff, 0x7f, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x0, 0xff}},
    {{0xff, 0x0, 0x0}, {0x0, 0x40, 0x0}, {0x0, 0x0, 0xff}, {0x80, 0x0, 0x80}},
    {{0x80, 0x0, 0x80}, {0x0, 0x40, 0x0}, {0xff, 0x52, 0x0}, {0xff, 0x0, 0x0}},
};
const uint16_t meta_newpair_band_durations[] = {1, 1, 1, 1, 1};
const uint16_t meta_newpair_band_fade_durs[] = {45, 45, 45, 45, 45};
// the animation:
const leds_animation_t meta_newpair = {meta_newpair_band_frames, meta_newpair_band_durations, meta_newpair_band_fade_durs, 5, ANIM_TYPE_SOLID, 26};

///////////////// META_PAIR:
// frames for the band:
const rgbcolor_t meta_pair_band_frames[][4] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xb2, 0x11, 0x22}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xda, 0x52, 0x20}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x20, 0x59, 0xaa}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x80, 0x0, 0x80}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t meta_pair_band_durations[] = {45, 45, 45, 45};
const uint16_t meta_pair_band_fade_durs[] = {45, 45, 45, 45};
// the animation:
const leds_animation_t meta_pair = {meta_pair_band_frames, meta_pair_band_durations, meta_pair_band_fade_durs, 4, ANIM_TYPE_SOLID, 16};

const leds_animation_t *band_all_anims[15] = {&h00geek_girl, &h01lagoon, &h02fire, &h03ice, &h04rainbow, &h05zflag_leather, &h06zflag_trans, &h07pan, &h08zflag_bear, &h09wiggle, &h10white, &h11rainbow2, &meta_boop, &meta_newpair, &meta_pair};
