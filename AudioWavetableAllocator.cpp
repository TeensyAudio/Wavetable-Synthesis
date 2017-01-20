//
//  AudioWavetableAllocator.cpp
//  
//
//  Created by Connor Delaplane on 1/19/17.
//
//

#include <stdio.h>
#include "AudioWavetableAllocator.h"
#include <SerialFlash.h>

uint8_t AudioWavetableAllocator::play(const unsigned int *data, float freq, float amp)
{
    for (int i=0; i<max_voices; i++) {
        if (!voices[i].isPlaying()) {
            voices[i].begin(freq, amp);
            voices[i].play(data);
            //break;
            return i;
        }
    }
    return -1;
}

/*void AudioWavetableAllocator::play(const unsigned int *data, int tag)
{
    if (tag >= 0 && tag < max_voices) {
        if (voices[tag].isPlaying()) {
            voices[tag].stop();
        }
        voices[tag].play(data);
    }
}*/

void AudioWavetableAllocator::stop(uint8_t voice)
{
    if (voice >= 0 && voice < max_voices) {
        voices[voice].stop();
    }
}
