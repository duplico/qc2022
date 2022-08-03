#include "leds.h"
#include "animations.h"

///////////////// B00:
const rgbcolor_t b00_frames[][5] = {
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}},
    {{0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t b00_durations[] = {80, 80, 80, 80};
const uint16_t b00_fade_durs[] = {10, 10, 10, 10};
// the animation:
const leds_animation_t b00 = {b00_frames, b00_durations, b00_fade_durs, 4, ANIM_TYPE_SOLID};

///////////////// B01:
const rgbcolor_t b01_frames[][5] = {
    {{0xff, 0xff, 0x0}, {0x0, 0x0, 0xff}, {0xff, 0x0, 0x0}, {0x0, 0xff, 0xff}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0x0, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0xff}, {0xff, 0xff, 0x0}, {0x0, 0xff, 0xff}},
    {{0x0, 0x0, 0xff}, {0xff, 0xff, 0x0}, {0x0, 0xff, 0xff}, {0xff, 0x0, 0x0}, {0xff, 0xff, 0xff}},
    {{0x0, 0xff, 0xff}, {0xff, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0x0}},
    {{0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0x0}, {0x0, 0x0, 0xff}, {0xff, 0x0, 0x0}},
};
const uint16_t b01_durations[] = {80, 80, 80, 80, 80, 80};
const uint16_t b01_fade_durs[] = {10, 10, 10, 10, 10, 10};
// the animation:
const leds_animation_t b01 = {b01_frames, b01_durations, b01_fade_durs, 6, ANIM_TYPE_SOLID};

///////////////// B02:
const rgbcolor_t b02_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0xff, 0xf, 0xf}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0xf, 0xf}, {0x0, 0x0, 0x0}},
    {{0xf, 0xff, 0xf}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0xff, 0xf}, {0x0, 0x0, 0x0}},
    {{0xf, 0xf, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0xf, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t b02_durations[] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
const uint16_t b02_fade_durs[] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
// the animation:
const leds_animation_t b02 = {b02_frames, b02_durations, b02_fade_durs, 10, ANIM_TYPE_FASTTWINKLE};

///////////////// B03:
const rgbcolor_t b03_frames[][5] = {
    {{0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x45, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0xff, 0x8c, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}},
    {{0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0x0, 0x0}},
};
const uint16_t b03_durations[] = {160, 160, 160, 160, 160};
const uint16_t b03_fade_durs[] = {0, 0, 0, 0, 0};
// the animation:
const leds_animation_t b03 = {b03_frames, b03_durations, b03_fade_durs, 5, ANIM_TYPE_SLOWTWINKLE};

///////////////// B04:
const rgbcolor_t b04_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0xbf, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x20, 0xb2, 0xaa}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0xbf, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x20, 0xb2, 0xaa}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t b04_durations[] = {100, 100, 100, 100, 100};
const uint16_t b04_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t b04 = {b04_frames, b04_durations, b04_fade_durs, 5, ANIM_TYPE_SLOWTWINKLE};

///////////////// B05:
const rgbcolor_t b05_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t b05_durations[] = {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40};
const uint16_t b05_fade_durs[] = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
// the animation:
const leds_animation_t b05 = {b05_frames, b05_durations, b05_fade_durs, 15, ANIM_TYPE_SOLID};

///////////////// B06:
const rgbcolor_t b06_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}},
    {{0xff, 0xff, 0x0}, {0xff, 0xff, 0x0}, {0xff, 0xff, 0x0}, {0xff, 0xff, 0x0}, {0xff, 0xff, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}},
    {{0xff, 0x0, 0xff}, {0xff, 0x0, 0xff}, {0xff, 0x0, 0xff}, {0xff, 0x0, 0xff}, {0xff, 0x0, 0xff}},
    {{0x0, 0xff, 0xff}, {0x0, 0xff, 0xff}, {0x0, 0xff, 0xff}, {0x0, 0xff, 0xff}, {0x0, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
};
const uint16_t b06_durations[] = {60, 60, 60, 60, 60, 60, 60, 60};
const uint16_t b06_fade_durs[] = {20, 20, 20, 20, 20, 20, 20, 20};
// the animation:
const leds_animation_t b06 = {b06_frames, b06_durations, b06_fade_durs, 8, ANIM_TYPE_SOLID};

///////////////// H00:
const rgbcolor_t h00_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h00_durations[] = {600, 600, 600, 600, 600};
const uint16_t h00_fade_durs[] = {400, 400, 400, 400, 400};
// the animation:
const leds_animation_t h00 = {h00_frames, h00_durations, h00_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// H01:
const rgbcolor_t h01_frames[][5] = {
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}},
    {{0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h01_durations[] = {400, 400, 400, 400};
const uint16_t h01_fade_durs[] = {200, 200, 200, 200};
// the animation:
const leds_animation_t h01 = {h01_frames, h01_durations, h01_fade_durs, 4, ANIM_TYPE_SOLID};

///////////////// H02:
const rgbcolor_t h02_frames[][5] = {
    {{0xf0, 0x14, 0x3c}, {0xf0, 0x14, 0x3c}, {0xf0, 0x14, 0x3c}, {0xf0, 0x14, 0x3c}, {0xf0, 0x14, 0x3c}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x99, 0xff}, {0x0, 0x99, 0xff}, {0x0, 0x99, 0xff}, {0x0, 0x99, 0xff}, {0x0, 0x99, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h02_durations[] = {1000, 200, 1000, 200};
const uint16_t h02_fade_durs[] = {200, 200, 200, 200};
// the animation:
const leds_animation_t h02 = {h02_frames, h02_durations, h02_fade_durs, 4, ANIM_TYPE_SLOWTWINKLE};

///////////////// H03:
const rgbcolor_t h03_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0xff, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0xff}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x80, 0x80, 0x80}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h03_durations[] = {150, 150, 150, 150, 150};
const uint16_t h03_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t h03 = {h03_frames, h03_durations, h03_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// H04:
const rgbcolor_t h04_frames[][5] = {
    {{0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0xff, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h04_durations[] = {400, 400, 400, 400};
const uint16_t h04_fade_durs[] = {200, 200, 200, 200};
// the animation:
const leds_animation_t h04 = {h04_frames, h04_durations, h04_fade_durs, 4, ANIM_TYPE_SOLID};

///////////////// H05:
const rgbcolor_t h05_frames[][5] = {
    {{0x0, 0x80, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h05_durations[] = {250, 250, 250, 250, 250};
const uint16_t h05_fade_durs[] = {250, 250, 250, 250, 250};
// the animation:
const leds_animation_t h05 = {h05_frames, h05_durations, h05_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// H06:
const rgbcolor_t h06_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h06_durations[] = {400, 400, 400, 400, 400};
const uint16_t h06_fade_durs[] = {200, 200, 200, 200, 200};
// the animation:
const leds_animation_t h06 = {h06_frames, h06_durations, h06_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// H07:
const rgbcolor_t h07_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h07_durations[] = {400, 400, 400, 400, 400};
const uint16_t h07_fade_durs[] = {200, 200, 200, 200, 200};
// the animation:
const leds_animation_t h07 = {h07_frames, h07_durations, h07_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// H08:
const rgbcolor_t h08_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
};
const uint16_t h08_durations[] = {400, 400, 400, 400, 400};
const uint16_t h08_fade_durs[] = {200, 200, 200, 200, 200};
// the animation:
const leds_animation_t h08 = {h08_frames, h08_durations, h08_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// H09:
const rgbcolor_t h09_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}},
};
const uint16_t h09_durations[] = {400, 400, 400, 400, 400};
const uint16_t h09_fade_durs[] = {200, 200, 200, 200, 200};
// the animation:
const leds_animation_t h09 = {h09_frames, h09_durations, h09_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// H10:
const rgbcolor_t h10_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}},
};
const uint16_t h10_durations[] = {500, 500, 500, 500, 500};
const uint16_t h10_fade_durs[] = {100, 100, 100, 100, 100};
// the animation:
const leds_animation_t h10 = {h10_frames, h10_durations, h10_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// H11:
const rgbcolor_t h11_frames[][5] = {
    {{0x0, 0xbf, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0xbf, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0xbf, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0xbf, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0xbf, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}, {0x80, 0x0, 0x80}},
};
const uint16_t h11_durations[] = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 400};
const uint16_t h11_fade_durs[] = {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 600, 200};
// the animation:
const leds_animation_t h11 = {h11_frames, h11_durations, h11_fade_durs, 14, ANIM_TYPE_SOLID};

///////////////// H12:
const rgbcolor_t h12_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h12_durations[] = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
const uint16_t h12_fade_durs[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
// the animation:
const leds_animation_t h12 = {h12_frames, h12_durations, h12_fade_durs, 15, ANIM_TYPE_SOLID};

///////////////// H13:
const rgbcolor_t h13_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h13_durations[] = {250, 250};
const uint16_t h13_fade_durs[] = {300, 300};
// the animation:
const leds_animation_t h13 = {h13_frames, h13_durations, h13_fade_durs, 2, ANIM_TYPE_SOLID};

///////////////// H14:
const rgbcolor_t h14_frames[][5] = {
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0xff, 0xff, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0xff, 0xff}},
    {{0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0xff, 0xff}, {0xff, 0xff, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0x0}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t h14_durations[] = {100, 100, 100, 100, 100, 100};
const uint16_t h14_fade_durs[] = {60, 60, 60, 60, 60, 60};
// the animation:
const leds_animation_t h14 = {h14_frames, h14_durations, h14_fade_durs, 6, ANIM_TYPE_SOLID};

///////////////// META_CONNECTS:
const rgbcolor_t meta_connects_frames[][5] = {
    {{0xf, 0x40, 0xff}, {0xf, 0x40, 0xff}, {0xf, 0x40, 0xff}, {0xf, 0x40, 0xff}, {0xf, 0x40, 0xff}},
    {{0xf, 0x40, 0xff}, {0xf, 0x40, 0xff}, {0xf, 0x40, 0xff}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0xff}},
    {{0xf, 0x40, 0xff}, {0xf, 0x40, 0xff}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0xff}},
    {{0xf, 0x40, 0xff}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0xff}},
    {{0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0xff}},
    {{0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}},
    {{0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x40, 0x0}},
    {{0xf, 0x40, 0x0}, {0xf, 0x40, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x40, 0x0}},
    {{0xf, 0x40, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x40, 0x0}},
    {{0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x40, 0x0}},
    {{0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}},
    {{0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}},
    {{0xf, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}},
    {{0xf, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}},
};
const uint16_t meta_connects_durations[] = {800, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 2500};
const uint16_t meta_connects_fade_durs[] = {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 2000};
// the animation:
const leds_animation_t meta_connects = {meta_connects_frames, meta_connects_durations, meta_connects_fade_durs, 15, ANIM_TYPE_SOLID};

///////////////// META_NEWPAIR:
const rgbcolor_t meta_newpair_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x2f, 0x4f, 0x4f}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x2f, 0x4f, 0x4f}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x2f, 0x4f, 0x4f}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x2f, 0x4f, 0x4f}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x2f, 0x4f, 0x4f}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x2f, 0x4f, 0x4f}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x2f, 0x4f, 0x4f}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x2f, 0x4f, 0x4f}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x80, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x2f, 0x4f, 0x4f}, {0xff, 0xff, 0xff}, {0x2f, 0x4f, 0x4f}, {0x2f, 0x4f, 0x4f}, {0x2f, 0x4f, 0x4f}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t meta_newpair_durations[] = {140, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 1000, 250};
const uint16_t meta_newpair_fade_durs[] = {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 1500, 2000, 250};
// the animation:
const leds_animation_t meta_newpair = {meta_newpair_frames, meta_newpair_durations, meta_newpair_fade_durs, 33, ANIM_TYPE_SLOWTWINKLE};

///////////////// META_NEWUBER:
const rgbcolor_t meta_newuber_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
};
const uint16_t meta_newuber_durations[] = {1000, 3000};
const uint16_t meta_newuber_fade_durs[] = {4000, 1000};
// the animation:
const leds_animation_t meta_newuber = {meta_newuber_frames, meta_newuber_durations, meta_newuber_fade_durs, 2, ANIM_TYPE_FASTTWINKLE};

///////////////// META_PAIR:
const rgbcolor_t meta_pair_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0xb2, 0x22, 0x22}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xda, 0xa5, 0x20}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x20, 0xb2, 0xaa}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x80, 0x0, 0x80}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t meta_pair_durations[] = {45, 45, 45, 45, 45};
const uint16_t meta_pair_fade_durs[] = {45, 45, 45, 45, 45};
// the animation:
const leds_animation_t meta_pair = {meta_pair_frames, meta_pair_durations, meta_pair_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// META_PAIRUBER:
const rgbcolor_t meta_pairuber_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x2f, 0x4f, 0x4f}, {0xff, 0xff, 0xff}, {0x2f, 0x4f, 0x4f}, {0x2f, 0x4f, 0x4f}, {0x2f, 0x4f, 0x4f}},
};
const uint16_t meta_pairuber_durations[] = {1000, 100};
const uint16_t meta_pairuber_fade_durs[] = {4000, 400};
// the animation:
const leds_animation_t meta_pairuber = {meta_pairuber_frames, meta_pairuber_durations, meta_pairuber_fade_durs, 2, ANIM_TYPE_FASTTWINKLE};

///////////////// META_STARTUP_FADE:
const rgbcolor_t meta_startup_fade_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0xff, 0xa5, 0x0}, {0xff, 0xa5, 0x0}, {0xff, 0xa5, 0x0}, {0xff, 0xa5, 0x0}, {0xff, 0xa5, 0x0}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
};
const uint16_t meta_startup_fade_durations[] = {750, 500, 500, 950};
const uint16_t meta_startup_fade_fade_durs[] = {750, 500, 500, 50};
// the animation:
const leds_animation_t meta_startup_fade = {meta_startup_fade_frames, meta_startup_fade_durations, meta_startup_fade_fade_durs, 4, ANIM_TYPE_SOLID};

///////////////// META_STARTUP_SPIN:
const rgbcolor_t meta_startup_spin_frames[][5] = {
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
};
const uint16_t meta_startup_spin_durations[] = {50, 50, 50, 50, 50};
const uint16_t meta_startup_spin_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t meta_startup_spin = {meta_startup_spin_frames, meta_startup_spin_durations, meta_startup_spin_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// META_Z_BRIGHTNESS0:
const rgbcolor_t meta_z_brightness0_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xf, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xf, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t meta_z_brightness0_durations[] = {75, 75, 75, 75, 75};
const uint16_t meta_z_brightness0_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t meta_z_brightness0 = {meta_z_brightness0_frames, meta_z_brightness0_durations, meta_z_brightness0_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// META_Z_BRIGHTNESS1:
const rgbcolor_t meta_z_brightness1_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x40, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x40, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x40, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xf, 0x40, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xf, 0x40, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t meta_z_brightness1_durations[] = {75, 75, 75, 75, 75};
const uint16_t meta_z_brightness1_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t meta_z_brightness1 = {meta_z_brightness1_frames, meta_z_brightness1_durations, meta_z_brightness1_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// META_Z_BRIGHTNESS2:
const rgbcolor_t meta_z_brightness2_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x40, 0xff}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x40, 0xff}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0xf, 0x40, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xf, 0x40, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xf, 0x40, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
};
const uint16_t meta_z_brightness2_durations[] = {75, 75, 75, 75, 75};
const uint16_t meta_z_brightness2_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t meta_z_brightness2 = {meta_z_brightness2_frames, meta_z_brightness2_durations, meta_z_brightness2_fade_durs, 5, ANIM_TYPE_SOLID};

///////////////// S00:
const rgbcolor_t s00_frames[][5] = {
    {{0xff, 0x0, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x45, 0x0}},
    {{0xff, 0x45, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x0, 0x0}},
    {{0xff, 0x8c, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0xff, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}},
    {{0xff, 0x8c, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x45, 0x0}, {0xff, 0x0, 0x0}},
};
const uint16_t s00_durations[] = {100, 100, 100, 100, 100};
const uint16_t s00_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t s00 = {s00_frames, s00_durations, s00_fade_durs, 5, ANIM_TYPE_SLOWTWINKLE};

///////////////// S01:
const rgbcolor_t s01_frames[][5] = {
    {{0x0, 0x0, 0xff}, {0x20, 0xb2, 0xaa}, {0x0, 0xbf, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0xbf, 0xff}},
    {{0x20, 0xb2, 0xaa}, {0x0, 0xbf, 0xff}, {0x20, 0xb2, 0xaa}, {0x0, 0xbf, 0xff}, {0x0, 0x0, 0xff}},
    {{0x0, 0xbf, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0xbf, 0xff}, {0x20, 0xb2, 0xaa}, {0x0, 0xbf, 0xff}},
    {{0x0, 0x0, 0xff}, {0x0, 0xbf, 0xff}, {0x20, 0xb2, 0xaa}, {0x0, 0xbf, 0xff}, {0x20, 0xb2, 0xaa}},
    {{0x0, 0xbf, 0xff}, {0x20, 0xb2, 0xaa}, {0x0, 0x0, 0xff}, {0x20, 0xb2, 0xaa}, {0x0, 0x0, 0xff}},
};
const uint16_t s01_durations[] = {100, 100, 100, 100, 100};
const uint16_t s01_fade_durs[] = {50, 50, 50, 50, 50};
// the animation:
const leds_animation_t s01 = {s01_frames, s01_durations, s01_fade_durs, 5, ANIM_TYPE_SLOWTWINKLE};

///////////////// S02:
const rgbcolor_t s02_frames[][5] = {
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
};
const uint16_t s02_durations[] = {400};
const uint16_t s02_fade_durs[] = {200};
// the animation:
const leds_animation_t s02 = {s02_frames, s02_durations, s02_fade_durs, 1, ANIM_TYPE_SLOWTWINKLE};

///////////////// U00:
const rgbcolor_t u00_frames[][5] = {
    {{0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}, {0x0, 0x0, 0x0}},
    {{0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}, {0xff, 0x0, 0x0}},
    {{0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}, {0x0, 0x80, 0x0}},
    {{0xff, 0xff, 0x0}, {0xff, 0xff, 0x0}, {0xff, 0xff, 0x0}, {0xff, 0xff, 0x0}, {0xff, 0xff, 0x0}},
    {{0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}, {0x0, 0x0, 0xff}},
    {{0xff, 0x0, 0xff}, {0xff, 0x0, 0xff}, {0xff, 0x0, 0xff}, {0xff, 0x0, 0xff}, {0xff, 0x0, 0xff}},
    {{0x0, 0xff, 0xff}, {0x0, 0xff, 0xff}, {0x0, 0xff, 0xff}, {0x0, 0xff, 0xff}, {0x0, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}},
};
const uint16_t u00_durations[] = {200, 200, 200, 200, 200, 200, 200, 200};
const uint16_t u00_fade_durs[] = {20, 20, 20, 20, 20, 20, 20, 20};
// the animation:
const leds_animation_t u00 = {u00_frames, u00_durations, u00_fade_durs, 8, ANIM_TYPE_SOLID};

///////////////// U01:
const rgbcolor_t u01_frames[][5] = {
    {{0x0, 0xff, 0xff}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0xff}, {0xff, 0xff, 0x0}, {0x0, 0x0, 0x0}},
    {{0x0, 0x0, 0xff}, {0xff, 0xff, 0x0}, {0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0xff, 0x0, 0x0}},
    {{0x0, 0x0, 0x0}, {0xff, 0xff, 0xff}, {0xff, 0x0, 0x0}, {0x0, 0xff, 0xff}, {0xff, 0xff, 0x0}},
    {{0xff, 0x0, 0x0}, {0x0, 0xff, 0xff}, {0xff, 0xff, 0x0}, {0x0, 0x0, 0xff}, {0xff, 0xff, 0xff}},
    {{0xff, 0xff, 0x0}, {0x0, 0x0, 0xff}, {0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0xff, 0xff}},
    {{0xff, 0xff, 0xff}, {0x0, 0x0, 0x0}, {0x0, 0xff, 0xff}, {0xff, 0x0, 0x0}, {0x0, 0x0, 0xff}},
};
const uint16_t u01_durations[] = {100, 100, 100, 100, 100, 100};
const uint16_t u01_fade_durs[] = {400, 400, 400, 400, 400, 400};
// the animation:
const leds_animation_t u01 = {u01_frames, u01_durations, u01_fade_durs, 6, ANIM_TYPE_SOLID};

const leds_animation_t *all_anims[37] = {&b00, &b01, &b02, &b03, &b04, &b05, &b06, &h00, &h01, &h02, &h03, &h04, &h05, &h06, &h07, &h08, &h09, &h10, &h11, &h12, &h13, &h14, &s00, &s01, &s02, &u00, &u01, &meta_connects, &meta_newpair, &meta_newuber, &meta_pair, &meta_pairuber, &meta_startup_fade, &meta_startup_spin, &meta_z_brightness0, &meta_z_brightness1, &meta_z_brightness2};
