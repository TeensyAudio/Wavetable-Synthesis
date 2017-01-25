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

const float AMP_DEF = 0;

class AudioAllocWavetable
{
public:
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t numVoices)
    {
        init(voices, numVoices, NULL, AMP_DEF);
    }
    
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t numVoices, const unsigned int* data)
    {
        init(voices, numVoices, data, AMP_DEF);
    }
    
    AudioAllocWavetable(AudioSynthWavetable* voices, uint8_t numVoices, const unsigned int* data, float amp)
    {
        init(voices, numVoices, data, amp);
    }
    
    // Set sample data for all voices.
    void setSample(const unsigned int* data);
    
    // Set amplitude for all voices.
    void setAmplitude(float amp);
    
    // Check whether a voice is currently playing at freq.
    // If not, find an unused voice to play freq (if one exists).
    // Either way, increment count for that frequency.
    void playFreq(float freq);
    
    // Convert note to frequency and pass to playFreq(float).
    void playNote(byte note);
    
    // Check whether a voice is currently playing at freq.
    // If so, decrement count for that frequency by 1.
    // If count hits 0, stop playing that frequency.
    void stopFreq(float freq);
    
    // Convert note to frequency and pass to stopFreq(float).
    void stopNote(byte note);
    
    // Return the number of voices currently playing.
    uint8_t voicesPlaying(void);
    
    // Return byte note data converted to float frequency.
    float noteToFreq(byte note);
    
    // Return amplitude.
    float getAmplitude() { return amplitude; }
    
    // Return the frequency being played by voices[index].
    float getFrequency(uint8_t index) { return freqTrack[index].freq; }
    
private:
    // Initialize object.
    void init(AudioSynthWavetable* voices, uint8_t numVoices, const unsigned int* data, float amp);
    
    AudioSynthWavetable* voices;
    uint8_t numVoices;
    freqTrack_t freqTrack[256];
    float amplitude;
};
#endif
