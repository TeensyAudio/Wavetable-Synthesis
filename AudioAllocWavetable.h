//
//  AudioAllocWavetable.h
//  
//
//  Created by Connor Delaplane on 1/19/17.
//
//

#ifndef AudioAllocWavetable_h
#define AudioAllocWavetable_h

#include "AudioSynthWavetable.h"

const uint8_t MAX_VOICES = 8;

class AudioAllocWavetable
{
public:
    AudioAllocWavetable(void) { }
    //AudioAllocWavetable(uint8_t num_voices) { MAX_VOICES = num_voices; }
    //void play(const unsigned int *data, int tag);
    uint8_t play(const unsigned int *data, float freq, float amp);
    void stop(uint8_t voice);
    uint8_t available(void)
    {
        int ret = 0;
        for (int i=0; i<MAX_VOICES; i++) {
            if (!voices[i].isPlaying()) {
                ret++;
            }
        }
        return ret;
    }
    AudioSynthWavetable* getVoice(uint8_t tag)
    {
        if (tag >= 0 && tag < MAX_VOICES) {
            return &(voices[tag]);
        }
        return voices;
    }
private:
    AudioSynthWavetable voices[MAX_VOICES];
};
#endif
