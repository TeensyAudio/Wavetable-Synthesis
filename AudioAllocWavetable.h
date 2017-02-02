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

struct freqTrack_t
{
    float freq;
    unsigned int count;
};

const float AMP_DEF = 0.0;

class AudioAllocWavetable
{
public:
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t numVoices)
    {
        init(voices, numVoices, NULL, AMP_DEF);
    }
    
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t numVoices, const unsigned int ** wavetable)
    {
        init(voices, numVoices, wavetable, AMP_DEF);
    }
    
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t numVoices, const unsigned int ** wavetable, float amp)
    {
        init(voices, numVoices, wavetable, amp);
    }
    
    void init(AudioSynthWavetable* voices, uint8_t numVoices, const unsigned int ** wavetable, float amp);
    void setSamples(const unsigned int ** wavetable);
    void setAmplitude(float amp);
    void playFreq(float freq);
    void playNote(byte note);
    void stopFreq(float freq);
    void stopNote(byte note);
    uint8_t voicesPlaying(void);
    float noteToFreq(byte note);
    float getAmplitude() { return amplitude; }
    float getFrequency(uint8_t index) { return freqTrack[index].freq; }
    
private:
    AudioSynthWavetable* voices;
    uint8_t numVoices;
    freqTrack_t freqTrack[256];
    float amplitude;
};
#endif
