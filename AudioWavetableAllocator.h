//
//  AudioWavetableAllocator.h
//  
//
//  Created by Connor Delaplane on 1/19/17.
//
//

#ifndef AudioWavetableAllocator_h
#define AudioWavetableAllocator_h

#include "AudioWavetableSynth.h"

const uint8_t max_voices = 8;

class AudioWavetableAllocator
{
public:
    AudioWavetableAllocator(void) { }
    //AudioWavetableAllocator(uint8_t num_voices) { max_voices = num_voices; }
    //void play(const unsigned int *data, int tag);
    uint8_t play(const unsigned int *data, float freq, float amp);
    void stop(uint8_t voice);
    uint8_t available(void)
    {
        int ret = 0;
        for (int i=0; i<max_voices; i++) {
            if (!voices[i].isPlaying()) {
                ret++;
            }
        }
        return ret;
    }
    AudioWavetableSynth* getVoice(uint8_t tag)
    {
        if (tag >= 0 && tag < max_voices) {
            return &(voices[tag]);
        }
        return NULL;
    }
private:
    AudioWavetableSynth voices[max_voices];
};
#endif /* AudioWavetableAllocator_h */
