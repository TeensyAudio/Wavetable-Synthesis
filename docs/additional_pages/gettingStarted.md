# Getting started

This section is currently under construction

This is a short guide on how to get started using the Audio Synth Wavetable library. 

## What is the Audio Synth Wavetable library?

The Audio Synth Wavetable is a C++ library uses sound data from a decoded SoundFont file and creates sound using wavetable synthesis. This library is for [Teensy 3.2/3.6](https://www.pjrc.com/teensy/).

## How do I use the Audio Synth Wavetable library?

To use the Audio Synth Wavetable library you must include the header file in your Arduino sketch (a `.ino` file): 

~~~~~~~~~~~~~~~~~~~~~
#include <AudioSynthWavetable.h>
~~~~~~~~~~~~~~~~~~~~~

The `AudioSynthWavetable.h` and `AudioSynthWavetable.cpp` files need to be in your Arduino library folder.

## Decoding Soundfont Files
See https://teensyaudio.github.io/Wavetable-Synthesis/html/md_additional_pages_soundfont_decoder.html for
information on how to decode a .sf2 file to a usable format for the `AudioSynthWavetable` class.

## Including a Soundfont File in an Arduino Sketch
When creating a sketch that uses the `AudioSynthWavetable` class, the output files from the soundfont decoder
(in the form of .cpp and .h files) need to be placed in the same directory as the Arduino sketch. Once 
these files are in the same directory, within the Arduino sketch code, `#include "<instrument-name>_samples.h"
must be added to the top of the sketch in order to use it with the wavetable class.

## Locating the instrument_data Within the Decoded Files
An instance of the `AudioSynthWavetable` object requires a complete instrument_data object to be passed to the
wavetable object. The instrument_data object can be found in the <instrument-name>_samples.h file that was included from 
the output files from the SoundfontDecoder (see `Including a Soundfont File in an Arduino Sketch` above).

## Setting up a AudioSynthWavetable Object
### Audio Connections
Before using an AudioSynthWavetable Object to synthesize music, it will be necessary to set up some audio connection
objects. See the Teensy Audio Library documentation at https://www.pjrc.com/teensy/td_libs_Audio.html for information
on using AudioConnection objects to connect the AudioSynthWavetable object to mixers and audio output drivers for
your specific Teensy board.

### Setting an Object's Instrument
To set an object's instrument, be sure that the .h and .cpp files from the SoundfontDecoder have been included in 
your Arduino sketch, and locate the name of the instrument_data object in the <instrument-name>_samples.h file. For
example it will look something like this for an instrument named "BasicFlute1":

`const instrument_data BasicFlute1 = {2, BasicFlute1_ranges, BasicFlute1_samples };`

In this example, assume that `AudioSynthWavetable wavetable;` is already placed in the variable declaration section of the Arduino
sketch, and assume the instrument_data object that gets passed to the AudioSynthWavetable object is named `BasicFlute1`.
To set the instrument of a particular wavetable object, perform the following:

`wavetable.setInstrument(BasicFlute1);`

Now, the wavetable's instrument has been set, and the object can now be used to synthesize instrument sounds
~~~~~~~~~~~~~~~

# A very simple first program

~~~~~~~~~~~~~~~~~~~~~
#include <AudioSynthWavetable.h>
~~~~~~~~~~~~~~~~~~~~~







