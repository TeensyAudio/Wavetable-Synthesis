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

const float AMP_DEF = 0.5

class AudioAllocWavetable
{
public:
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t length)
    {
        init(voices, length, NULL, AMP_DEF);
    }
    
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t length, const unsigned int* data)
    {
        init(voices, length, data, AMP_DEF);
    }
    
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t length, const unsigned int* data, float amp)
    {
        init(voices, length, data, amp);
    }
    
    void setSample(const unsigned int* data);
    void setAmplitude(float amp);
    void playFreq(float freq);
    void playNote(byte note);
    void stopFreq(float freq);
    void stopNote(byte note);
    uint8_t playing(void);
    
private:
    void init(AudioSynthWavetable* voices, uint8_t length, const unsigned int* data, float amp);
    
    AudioSynthWavetable* voices;
    uint8_t length;
    float playingFreqs[256];
    unsigned int* sample;
    float amplitude;
};
#endif
