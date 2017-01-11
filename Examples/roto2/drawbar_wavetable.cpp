// Copyright (c) 2015 Peter Teichman

#include "drawbar_wavetable.h"

void DrawbarWavetable::Rebuild(void) {
    uint32_t buf[DRAWBAR_WAVETABLE_LENGTH+1];

    // Accumulating 24 bits per drawbar (16 sine * 8 volume), then the
    // 9 drawbars accumulate together for 4 more bits. So shift 12 at
    // the end to get back to 16 bits.

    // First drawbar: sub octave (220Hz @ A4)
    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        buf[i] = AudioWaveformSine[i & 0xff] * drawbars[0]
            + AudioWaveformSine[3*i & 0xff] * drawbars[1]
            + AudioWaveformSine[2*i & 0xff] * drawbars[2]
            + AudioWaveformSine[4*i & 0xff] * drawbars[3]
            + AudioWaveformSine[6*i & 0xff] * drawbars[4]
            + AudioWaveformSine[8*i & 0xff] * drawbars[5]
            + AudioWaveformSine[10*i & 0xff] * drawbars[6]
            + AudioWaveformSine[12*i & 0xff] * drawbars[7]
            + AudioWaveformSine[16*i & 0xff] * drawbars[8];
    }

    __disable_irq();
    for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
        Wave[i] = buf[i] >> 10;
    }
    Wave[DRAWBAR_WAVETABLE_LENGTH] = Wave[0];
    __enable_irq();
}
