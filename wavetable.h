/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef wavetable_h_
#define wavetable_h_

#include "Arduino.h"
#include "AudioStream.h"

/*class AudioWavetable : public AudioStream
{
public:
	AudioWavetable(void) : AudioStream(0, NULL), playing(0) { }
	void play(const unsigned int *data, double mult);
	void stop(void);
	bool isPlaying(void) { return playing; }
	uint32_t positionMillis(void);
	uint32_t lengthMillis(void);
	virtual void update(void);
private:
	const unsigned int *next;
	const unsigned int *beginning;
	uint32_t length;
	int16_t prior;
	volatile uint8_t playing;
	double multiplier;
};*/

class AudioWavetable : public AudioStream
{
public:
    AudioWavetable(void) : AudioStream(0, NULL), playing(0) { }
    void soundOn(const unsigned int *data, double mult);
    void soundOn(void);
    void soundOff(void);
    void setData(const unsigned int *data);
    unsigned int getData() { return *data; }
    bool isPlaying(void) { return playing; }
    uint32_t positionMillis(void);
    uint32_t lengthMillis(void);
    virtual void update(void);
    void setFrequency(float frequency);
    void setIntensity(uint8_t intensity);
private:
    const unsigned int *data;
    const unsigned int *attack_start;
    const unsigned int *attack_next;
    const unsigned int *sustain_start;
    const unsigned int *sustain_next;
    const unsigned int *release_start;
    const unsigned int *release_next;
    uint32_t attack_length;
    uint32_t sustain_length;
    uint32_t release_length;
    int16_t prior;
    volatile uint8_t playing;
    uint8_t intensity;
    uint16_t frequency;
    double multiplier;
};

#endif
