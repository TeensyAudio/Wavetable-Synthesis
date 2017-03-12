In order to run this test and have the code correctly compile on the Teensy 3.6, add the following lines to
the end of the boards.txt file under C:\Program Files (X86)\Arduino\hardware\teensy\avr\boards.txt:

teensy36.build.fcpu=48000000
teensy36.build.flags.optimize=-Os
teensy36.build.flags.ldspecs=--specs=nano.specs
teensy36.build.keylayout=US_ENGLISH
teensy36.build.usbtype=USB_SERIAL

This is needed because arduino-builder doesn't like optimized code.

Also, update the DecoderTest.py script with the locations of various Arduino directories on your 
personal machine's setup.

Solution found at: http://catherineh.github.io/programming/2016/04/15/arduino-builder-errors