import sys, os, string

import webcolors

COLOR_SCALE_FACTOR = 1.0

color_corrections = {
    "default" : (40,240,178),
    "off" : (0,0,0),
    "pink" : (240, 20, 60),
    "bipink" : (0xff, 0, 0xb0),
    "bimiddle" : (0x53, 0x1f, 0x66),
    "biblue" : (0x50, 0, 0xff),
    "pan1" : (0xff,0x21,0x8c),
    "pan2" : (0xff,0xd8,0x00),
    "pan3" : (0x21, 0xb1, 0xff),
    "b1" : (0xc5,0x20,0x04),
    "b2" : (0xd5, 0x63, 0),
    "b3" : (0xfe, 0xdd, 0x63),
    "b4" : (0xbe, 0xa6, 0x38),
    "dim" : (0, 0, 15),
    "fadered" : (15, 0, 0),
    "bright1" : (15, 0, 0),
    "bright2" : (15, 64, 0),
    "bright3" : (15, 64, 255),
    "redblue" : (255, 0, 255),
    "redgreen" : (255, 255, 0),
    "bluegreen" : (0,255,255),
    "whitered" : (255,15,15),
    "whitegreen" : (15,255,15),
    "whiteblue" : (15,15,255)
}

LED_COUNT = 5
HOLD_DUR_INDEX = LED_COUNT
FADE_DUR_INDEX = LED_COUNT+1

global_color_correct = (1,1,1)

eye_frames = dict()
eye_frames_uint32 = dict()

c_lines = [
    '#include "leds.h"',
    '#include "animations.h"',
]
h_lines = [
    "#ifndef ANIMATIONS_H_",
    "#define ANIMATIONS_H_",
    "",
    '#include "leds.h"',
    "",
]

def main():
    all_animations = []
    meta_animations = []
    all_types = []
    
    for anim in os.listdir("."):
        if not anim[-3:] == "txt": continue
        print(anim)
        color_lookups = dict()
        
        
        anim_name = anim[:-4]
        if anim_name.upper().startswith('META'):
            meta_animations.append(anim_name)
        else:
            all_animations.append(anim_name)
        
        c_lines.append("")
        c_lines.append("///////////////// %s:" % anim_name.upper())
        # Append to "ALL" or whatever
        with open(anim) as f:
            local_colors = dict()
            
            lines = [line.strip() for line in f if line.strip() and (line[0] != "#")]
            line_no = 0
            
            # Consume all of the colors in this
            while lines[line_no][0] in string.digits:
                color_num, color_name = lines[line_no].split()
                color_lookups[color_name] = int(color_num)
                local_colors[int(color_num)] = color_corrections.get(
                    color_name,
                    webcolors.name_to_rgb(color_name) if color_name not in color_corrections else None
                )
                
                local_colors[int(color_num)] = tuple(map(lambda a: COLOR_SCALE_FACTOR*a, local_colors[int(color_num)]))
                if color_name not in color_corrections:
                    local_colors[int(color_num)] = tuple(map(lambda a: int(a[0]*a[1]), zip(local_colors[int(color_num)], global_color_correct)))
                line_no += 1
            
            frame_lines_str = ""
            
            # The next line is the animation TYPE. We don't actually use or check the
            #  type value in this script, but the badge code does care whether it's
            #  solid or twinkling or fading or whatever.
            local_type = lines[line_no]
            assert local_type[0] not in string.digits
            if local_type not in all_types: all_types.append(local_type)
            line_no += 1
            
            try:
                while lines[line_no][0] in string.digits:
                    # Consume the animation. Ignore white space
                    frame_lines_str += lines[line_no]
                    if lines[line_no][-1] != ',':
                        frame_lines_str += ','
                    line_no += 1
            except IndexError:
                pass

            if frame_lines_str[-1] == ',':
                frame_lines_str = frame_lines_str[:-1]
            animation_squashed = list(map(lambda s: int(s.strip()), frame_lines_str.split(',')))
            frames = [animation_squashed[i:i+LED_COUNT+2] for i in range(0, len(animation_squashed), LED_COUNT+2)]   
            
            # Great. Now we've ingested the entire file.
            # Time to start generating frames.
            
            local_animation_names = []

            c_lines.append("const rgbcolor_t %s_frames[][%d] = {" % (anim_name, LED_COUNT))
            h_lines.append("extern const rgbcolor_t %s_frames[][%d];" % (anim_name, LED_COUNT))
            
            # Flags are all the same.
            local_animation_names += [anim_name]
            
            metadata1 = []
            metadata2 = []
            total_duration = 0
            for f in frames:
                metadata1 += [str(f[HOLD_DUR_INDEX])]
                metadata2 += [str(f[FADE_DUR_INDEX])]
                total_duration += int(f[HOLD_DUR_INDEX]) + int(f[FADE_DUR_INDEX])
                try:
                    fr = list(map(lambda a: local_colors[a], f[:LED_COUNT]))
                except:
                    print('Error on frame: ', f)
                    exit(1)
                fr = fr[::-1]
                c_lines.append("    {%s}," % ', '.join(map(lambda rgb: "{0x%x, 0x%x, 0x%x}" % tuple(map(int, rgb)), fr)))
            
            c_lines.append("};")
            c_lines.append("const uint16_t %s_durations[] = {%s};" % (anim_name, ', '.join(metadata1)))
            c_lines.append("const uint16_t %s_fade_durs[] = {%s};" % (anim_name, ', '.join(metadata2)))
            
            h_lines.append("extern const uint16_t %s_durations[];" % anim_name)
            h_lines.append("extern const uint16_t %s_fade_durs[];" % anim_name)
            
            c_lines.append("// the animation:")
            c_lines.append("const leds_animation_t %s = {%s_frames, %s_durations, %s_fade_durs, %d, ANIM_TYPE_%s};" % (anim_name, anim_name, anim_name, anim_name, len(frames), local_type.upper()))
            
            h_lines.append("extern const leds_animation_t %s;" % anim_name)

    c_lines.append("")
    h_lines.append("#define ANIM_COUNT %d" % len(all_animations))
    h_lines.append("#define ANIM_COUNT_INCL_META %d" % len(all_animations+meta_animations))
    for i in range(len(all_animations+meta_animations)):
        h_lines.append("#define ANIM_%s %d" % ((all_animations+meta_animations)[i].upper(), i))
    for i in range(len(all_types)):
        h_lines.append("#define ANIM_TYPE_%s %d" % (all_types[i].upper(), i))

    c_lines.append("const leds_animation_t *all_anims[%d] = {%s};" % (len(all_animations+meta_animations), ', '.join(map(lambda a: '&%s' % a, all_animations+meta_animations))))
    h_lines.append("extern const leds_animation_t *all_anims[%d];" % len(all_animations+meta_animations))
    
    h_lines.append("#endif // _H_")
     
    with open("animations.c", 'w') as f:
        f.writelines(map(lambda a: a+"\n", c_lines))
    
    with open("animations.h", 'w') as f:
        f.writelines(map(lambda a: a+"\n", h_lines))
            
    
if __name__ == "__main__":
    main()
