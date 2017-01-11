// Copyright (c) 2015 Peter Teichman

#ifndef DRAWBAR_OSC_H
#define DRAWBAR_OSC_H

#include <Audio.h>

#include "drawbar_wavetable.h"

class DrawbarOsc : public AudioStream {
 public:
    DrawbarOsc() : AudioStream(0, NULL) {
    }

    void Init() {
        tone_phase = 0;
        tone_incr = 0;
        amp = 0;
    }

    void Begin(float freq, float amp, DrawbarWavetable *wav) {
        Init();
        Fundamental(freq);
        Amplitude(amp);
        wav_ = wav;
    }

    void Fundamental(float freq) {
        if (freq < 0.0) {
            freq = 0.0;
        } else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2) {
            freq = AUDIO_SAMPLE_RATE_EXACT / 2;
        }
        tone_incr = freq * (0x10000000LL/AUDIO_SAMPLE_RATE_EXACT) + 0.5;
    }

    void Amplitude(float v) {
        if (v < 0.0) {
            v = 0.0;
        } else if (v > 1.0) {
            v = 1.0;
        }

        if ((amp == 0) && v) {
            tone_phase = 0;
        }

        amp = (uint16_t)(32767.0*v);
    }

    void update(void);

 private:
    uint16_t amp;
    uint32_t tone_phase;
    uint32_t tone_width;
    uint32_t tone_incr;

    DrawbarWavetable *wav_;
};

#endif
