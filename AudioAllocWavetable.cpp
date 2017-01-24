
//  AudioAllocWavetable.cpp
//  
//
//  Created by Connor Delaplane on 1/19/17.
//
//

#include <stdio.h>
#include "AudioAllocWavetable.h"
#include <SerialFlash.h>

void AudioAllocWavetable::setSample(const unsigned int* data)
{
    sample = data;
    for (int i=0; i<length; i++) {
        voices[i].setSample(data);
    }
}

void AudioAllocWavetable::setAmplitude(float amp)
{
    amplitude = amp;
    for (int i=0; i<length; i++) {
        voices[i].amplitude(amp);
    }
}

void AudioAllocWavetable::playFreq(float freq)
{
    if (freq < 0) {
        return;
    }
    
    for (int i=0; i<length; i++) {
        if (playingFreqs[i] == freq) {
            return;
        }
    }
    
    for (int i=0; i<length; i++) {
        if (!voices[i].isPlaying()) {
            voices[i].playFrequency(freq);
            playingFreqs[i] = freq;
        }
    }
}

void AudioAllocWavetable::playNote(byte note)
{
    float freq = 440.0 * pow(2.0, (note - 69) / 12.0);
    playFreq(freq);
}

void AudioAllocWavetable::stopFreq(float freq)
{
    if (freq < 0) {
        return;
    }
    
    for (int i=0; i<length; i++) {
        if (playingFreqs[i] == freq) {
            voices[i].stop();
            playingFreqs[i] = -1;
        }
    }
}

void AudioAllocWavetable::stopNote(byte note)
{
    float freq = 440.0 * pow(2.0, (note - 69) / 12.0);
    stop(freq);
}

uint8_t AudioAllocWavetable::playing(void)
{
    int ret = 0;
    for (int i=0; i<length; i++) {
        if (voices[i].isPlaying()) {
            ret++;
        }
    }
    return ret;
}

void AudioAllocWavetable::init(AudioSynthWavetable* voices, uint8_t length, const unsigned int* data, float amp)
{
    this->voices = voices;
    this->length = length;
    setSample(data);
    setAmplitude(amp);
    for (int i=0; i<length; i++) {
        voices[i].stop();
        playingFreqs[i] = -1;
    }
}
