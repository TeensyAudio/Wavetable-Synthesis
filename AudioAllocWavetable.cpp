//
//  AudioAllocWavetable.cpp
//  
//
//  Created by Connor Delaplane on 1/19/17.
//
//

#include <stdio.h>
#include "AudioAllocWavetable.h"
#include <SerialFlash.h>

uint8_t AudioAllocWavetable::play(const unsigned int *data, float freq, float amp)
{
    for (int i=0; i<MAX_VOICES; i++) {
        if (!voices[i].isPlaying()) {
            voices[i].begin(freq, amp);
            voices[i].play(data);
            return i;
        }
    }
    return -1;
}

void AudioAllocWavetable::stop(uint8_t voice)
{
    if (voice >= 0 && voice < MAX_VOICES) {
        voices[voice].stop();
    }
}
