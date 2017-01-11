// Copyright (c) 2015 Peter Teichman

#include "vibrato.h"

uint32_t triangle(uint32_t phase) {
    // Convert 32-bit phase into signed 31-bit triangle.
    if (phase & 0x80000000) {
        return 0x80000000 + (0x80000000 - phase);
    } else {
        return phase;
    }
}

int32_t lerp(int32_t a, int32_t b, uint16_t scale) {
    return ((0xFFFF - scale)*a + scale*b) >> 16;
}

void Vibrato::update(void) {
    audio_block_t *in = receiveReadOnly(0);
    if (in == NULL) {
        return;
    }

    audio_block_t *out = allocate();
    if (out == NULL) {
        release(in);
        return;
    }

    uint8_t loc_wp = wp;
    uint32_t loc_phase = scan_phase;

    for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++) {
        // Write the input audio to our delay line.
        buf[loc_wp] = in->data[i];

        // Read the delay line into the output buffer.

        // 7 bits of loc_wp to 1..8; subtract triangle. This gives 5
        // bits of sway on a 7 bit counter.
        int32_t loc_rp = (loc_wp << 24) - (int32_t)(triangle(loc_phase) >> depth);

        int16_t pos = loc_rp >> 24;
        int16_t a = buf[pos & 0x7f];
        int16_t b = buf[++pos & 0x7f];

        int16_t val = lerp(a, b, (loc_rp >> 8) & 0xFFFF);
        if (mix) {
            val = ((int32_t)val + buf[loc_wp]) >> 1;
        }

        out->data[i] = val;

        loc_phase += 679632;

        // Increment and wrap loc_wp.
        loc_wp++;
        loc_wp &= 0x7f;
    }

    wp = loc_wp;
    scan_phase = loc_phase;

    transmit(out, 0);
    release(out);
    release(in);
}
