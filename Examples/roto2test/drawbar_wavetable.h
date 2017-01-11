// Copyright (c) 2015 Peter Teichman

#ifndef DRAWBAR_WAVETABLE_H
#define DRAWBAR_WAVETABLE_H

#include <Audio.h>

#define DRAWBAR_WAVETABLE_LENGTH (768)

class DrawbarWavetable {
 public:
    int16_t Wave[DRAWBAR_WAVETABLE_LENGTH+1];

    DrawbarWavetable() {
    }

    void Init() {
        for (int i=0; i<9; i++) {
            drawbars[i] = 0;
        }

        for (int i=0; i<DRAWBAR_WAVETABLE_LENGTH; i++) {
            Wave[i] = 0;
        }
        Wave[DRAWBAR_WAVETABLE_LENGTH] = Wave[0];
    }

    // AllDrawbars takes a long long, so that drawbar settings can be
    // made inline. Each nibble turns into a single drawbar value, so
    // e.g. 0x888800000 sets the first four drawbars to 8 and the rest
    // to zero.
    void AllDrawbars(long long harmonics) {
        drawbars[8] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[7] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[6] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[5] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[4] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[3] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[2] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[1] = 28*(harmonics & 0xf);
        harmonics >>= 4;
        drawbars[0] = 28*(harmonics & 0xf);

        Rebuild();
    }

    void Drawbar(int n, float v) {
        if (v < 0.0) {
            v = 0.0;
        } else if (v > 1.0) {
            v = 1.0;
        }
        drawbars[n] = (uint8_t)(255*v);

        Rebuild();
    }

 private:
    uint8_t drawbars[9];

    void Rebuild(void);
};

#endif
