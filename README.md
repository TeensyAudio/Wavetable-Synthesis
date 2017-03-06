# Wavetable-Synthesis
General repository for Wavetable Synthesis Capstone project at Portland State, Fall 2016 - Winter 2017

The purpose of this project is to provide a C++ library and accompanying Python utility scripts allowing realistic instrumentation audio to be synthesized on the Teensy 3.2/3.6 Arduino Digital Analog Converter (DAC). 

The project consists of two main components:

1. SoundFont Decoder
2. Audio Synth Wavetable class

The project documentation can be found at: https://connorreilly.github.io/Wavetable-Synthesis/html/index.html.

The project's source is available on GitHub: https://github.com/ConnorReilly/Wavetable-Synthesis

## SoundFont Decoder

The SoundFont decoder is a python utility to decode a SoundFont file into native C++ datatypes. The decoder can be run via a GUI or via the command line. 

### GUI
To run the GUI you must use Python3, specifically 3.6
invoke with `$ python3 decoderGui.py`  

### Command Line
How to invoke the script:  
The `-d` flag is for debug mode  
The `-i` flag precedes the input file  
`$ python decoder.py -i soundfont`  
`$ python decoder.py -d -i soundfont`  

## Audio Synth Wavetable

C++ class implementing wavetable synthesis.

