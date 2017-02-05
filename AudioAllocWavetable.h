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

class AudioAllocWavetable
{
public:
    AudioAllocWavetable(void) 
		: voices(NULL)
		, numVoices(0)
		, amplitude(1.0)
	{}
    
    void init(AudioSynthWavetable* voices, uint8_t numVoices, const unsigned int ** wavetable, float amp);
    void setSamples(const unsigned int ** wavetable);
    void setAmplitude(float amp);
    void playFreq(float freq);
    void playNote(int note);
    void stopFreq(float freq);
    void stopNote(int note);
    uint8_t voicesPlaying(void);
    float noteToFreq(int note);
    float getAmplitude() { return amplitude; }
    float getFrequency(uint8_t index) { return freqTrack[index].freq; }
    
private:
    AudioSynthWavetable* voices;
    uint8_t numVoices;
    freqTrack_t freqTrack[256];
    float amplitude;
};
#endif
