
//  AudioAllocWavetable.cpp
//  
//
//  Created by Connor Delaplane on 1/19/17.
//
//

#include "AudioAllocWavetable.h"
#include <SerialFlash.h>

void AudioAllocWavetable::init(AudioSynthWavetable* voices, uint8_t numVoices, const unsigned int* data, float amp)
{
    this->voices = voices;
    this->numVoices = numVoices;
    setSample(data);
    setAmplitude(amp);
    for (int i=0; i<numVoices; i++) {
        voices[i].stop();
        freqTrack[i].freq = -1;
        freqTrack[i].count = 0;
    }
}

void AudioAllocWavetable::setSample(const unsigned int* data)
{
    for (int i=0; i<numVoices; i++) {
        voices[i].setSample(data);
    }
}

void AudioAllocWavetable::setAmplitude(float amp)
{
    amplitude = amp;
    for (int i=0; i<numVoices; i++) {
        voices[i].amplitude(amp);
    }
}

void AudioAllocWavetable::playFreq(float freq)
{
    if (freq < 0) {
        return;
    }
    
    // Check for voices playing this frequency.
    for (int i=0; i<numVoices; i++) {
        if (freqTrack[i].freq == freq) {
            freqTrack[i].count++;
            return;
        }
    }
    
    // If none, find one to play it.
    for (int i=0; i<numVoices; i++) {
        if (!voices[i].isPlaying()) {
            voices[i].playFrequency(freq);
            freqTrack[i].freq = freq;
            freqTrack[i].count++;
            return;
        }
    }
}

void AudioAllocWavetable::playNote(byte note)
{
    playFreq(noteToFreq(note));
}

void AudioAllocWavetable::stopFreq(float freq)
{
    if (freq < 0) {
        return;
    }
    
    for (int i=0; i<numVoices; i++) {
        if (freqTrack[i].freq == freq) {
            if (--freqTrack[i].count == 0) {
                voices[i].stop();
                freqTrack[i].freq = -1;
            }
            return;
        }
    }
}

void AudioAllocWavetable::stopNote(byte note)
{
    stopFreq(noteToFreq(note));
}

uint8_t AudioAllocWavetable::voicesPlaying(void)
{
    int ret = 0;
    for (int i=0; i<numVoices; i++) {
        if (voices[i].isPlaying()) {
            ret++;
        }
    }
    return ret;
}

float AudioAllocWavetable::noteToFreq(byte note)
{
    return AudioSynthWavetable::noteToFreq(note);
}
