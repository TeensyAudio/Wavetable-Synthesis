# Getting started

This is a short guide on how to get started using the Audio Synth Wavetable library. 

# What is the Audio Synth Wavetable library? {#WhatIs}

The Audio Synth Wavetable is a C++ library uses sound data from a decoded SoundFont file and creates sound using wavetable synthesis. This library is for [Teensy 3.2/3.6](https://www.pjrc.com/teensy/).

# How do I use the Audio Synth Wavetable library? {#howtoUse}

To use the Audio Synth Wavetable library you must include the header file in your Arduino sketch (a `.ino` file): 

~~~~~~~~~~~~~~~{.c}
#include <AudioSynthWavetable.h>
~~~~~~~~~~~~~~~

The `AudioSynthWavetable.h` and `AudioSynthWavetable.cpp` files need to be in your Arduino library folder.

# Decoding Soundfont Files {#decoder}

See [SoundFont Decoder User-Guide](https://teensyaudio.github.io/Wavetable-Synthesis/html/md_additional_pages_soundfont_decoder.html) for
information on how to decode a .sf2 file to a usable format for the `AudioSynthWavetable` class.

# Including a Soundfont File in an Arduino Sketch {#including}

When creating a sketch that uses the `AudioSynthWavetable` class, the output files from the soundfont decoder
(in the form of .cpp and .h files) need to be placed in the same directory as the Arduino sketch. Once 
these files are in the same directory, within the Arduino sketch code, `#include "<instrument-name>_samples.h"`
must be added to the top of the sketch in order to use it with the wavetable class.

# Locating the instrument_data Within the Decoded Files {#locating}

An instance of the `AudioSynthWavetable` object requires a complete `instrument_data` object to be passed to the
wavetable object. The instrument_data object can be found in the `<instrument-name>_samples.h` file that was included from 
the output files from the SoundfontDecoder (see [`Including a Soundfont File in an Arduino Sketch`](#including) above).

# Setting up an AudioSynthWavetable Object {#settingObject}

## Audio Connections {#AudioConnectios}

Before using an AudioSynthWavetable Object to synthesize music, it will be necessary to set up some audio connection
objects. See the Teensy Audio Library documentation at https://www.pjrc.com/teensy/td_libs_Audio.html for information
on using AudioConnection objects to connect the AudioSynthWavetable object to mixers and audio output drivers for
your specific Teensy board.

## Setting an Object's Instrument {#settingInst}

To set an object's instrument, be sure that the .h and .cpp files from the SoundfontDecoder have been included in 
your Arduino sketch, and locate the name of the instrument_data object in the `<instrument-name>_samples.h` file. For
example it will look something like this for an instrument named "BasicFlute1":

~~~~~~~~~~~~~~~{.c}
const instrument_data BasicFlute1 = {2, BasicFlute1_ranges, BasicFlute1_samples };
~~~~~~~~~~~~~~~

In this example, assume that `%AudioSynthWavetable wavetable;` is already placed in the variable declaration section of the Arduino
sketch, and assume the instrument_data object that gets passed to the AudioSynthWavetable object is named `BasicFlute1`.
To set the instrument of a particular wavetable object, perform the following:

~~~~~~~~~~~~~~~{.c}
wavetable.setInstrument(BasicFlute1);
~~~~~~~~~~~~~~~

Now, the wavetable's instrument has been set, and the object can now be used to synthesize instrument sounds.


# A very simple first program {#firstProg}

This program is meant to be played using the Teensy development board that has a button on pin 1, and analog knobs on A2 and A3.

Frequency and amplitude are modified using the knobs, and the flute is triggered to play/stop via the button on pin #1.

~~~~~~~~~~~~~~~{.c}
#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <AudioSynthWavetable.h>
#include "BasicFlute1_samples.h"

AudioSynthWavetable wavetable;
AudioOutputI2S i2s1;
AudioMixer4 mixer;
AudioConnection patchCord1(wavetable, 0, mixer, 0);
AudioConnection          patchCord2(mixer, 0, i2s1, 0);
AudioConnection          patchCord3(mixer, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;

// Bounce objects to read pushbuttons 
Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);  // 15 ms debounce time
Bounce button2 = Bounce(2, 15);

void setup() { 
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);
  mixer.gain(0, 0.7);
  
  wavetable.setInstrument(BasicFlute1);
  wavetable.amplitude(1);
}

bool playing = false;

void loop() {
  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();
  //Read knob values
  int knob1 = analogRead(A3);
  int knob2 = analogRead(A2);
  //Get frequency and gain from knobs
  float freq = (float)knob1/5.0;
  float gain = (float)knob2/1023.0;
  //Set a low-limit to the gain
  if (gain < .05) gain = .05;

  if (button1.fallingEdge()) {
    if (playing) {
      playing = false;
      wavetable.stop();
    }
    else {
      playing = true;
      wavetable.playFrequency(freq);
      wavetable.amplitude(gain);
    }    
  }
  wavetable.amplitude(gain);
  wavetable.setFrequency(freq);
}
~~~~~~~~~~~~~~~







