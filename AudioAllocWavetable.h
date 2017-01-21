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

const uint8_t MAX_VOICES = 0xFF;    // Number of voices

class AudioAllocWavetable
{
public:
    AudioAllocWavetable(void) { }
    
    // Find an available voice and make it play audio data.
    // Return the index of the voice found.
    // If there are no available voices, return -1.
    // data:    pointer to audio sample byte data
    // freq:    frequency multiplier
    // amp:     set amplitude
    uint8_t play(const unsigned int *data, float freq, float amp);
    
    // If a voice is playing, stop it.
    // voice:   the voice to stop playing
    void stop(uint8_t voice);
    
    // Return the number of available voices.
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
    
    // Return a pointer to the voice at index tag.
    // If tag is invalid, return pointer to beginning of array.
    AudioSynthWavetable* getVoice(uint8_t tag)
    {
        if (tag >= 0 && tag < MAX_VOICES) {
            return &(voices[tag]);
        }
        return voices;
    }
    
private:
    AudioSynthWavetable voices[MAX_VOICES];     // Array of voices
};
#endif
