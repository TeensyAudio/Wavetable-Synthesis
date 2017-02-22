#include "vibrato.h"

uint32_t triangle(uint32_t phase) {
    
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

}  
