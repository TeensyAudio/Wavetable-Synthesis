##
# Audio Library for Teensy 3.X
# Copyright (c) 2017, TeensyAudio PSU Team
#
# Development of this audio library was sponsored by PJRC.COM, LLC.
# Please support PJRC's efforts to develop open source 
# software by purchasing Teensy or other PJRC products.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice, development funding notice, and this permission
# notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
##

################## IMPORTANT USAGE NOTE!!! ##########################
# In order for this test script to operate correctly, please follow the 
# instructions within the README.txt of the \DecoderTest directory.
#####################################################################

from unittest.mock import patch, MagicMock, mock_open
from io import  StringIO
from contextlib import *
import unittest
import sys
import decoder
import getopt
import sf2utils

import glob
from shutil import copyfile
from subprocess import check_output

########### To use this test, update these values with the locations on your specific machine ######################
arduino_builder_location = '"C:\\Program Files (x86)\\Arduino\\arduino-builder"'
arduino_hardware_location1 = '"C:\\Program Files (x86)\\Arduino\\hardware"'
arduino_tools_location1 = '"C:\\Program Files (x86)\\Arduino\\hardware\\tools"'
arduino_tools_location2 = '"C:\\Program Files (x86)\\Arduino\\tools-builder"'
fully_qual_board_name = 'teensy:avr:teensy36:usb=serialmidi'
wavetable_lib_location = 'C:\\Users\\Ryan\\Documents\\Arduino\\libraries'
teensy_lib_location = '"C:\\Program Files (x86)\\Arduino\\hardware\\teensy\\avr\\libraries"'
####################################################################################################################

arduino_upload_str = '{0} -hardware {1} -tools {2} -tools {3} -fqbn {4} -libraries {5} -libraries {6} ./AutoTest/AutoTest.ino'
arduino_upload_str = arduino_upload_str.format(arduino_builder_location,
												arduino_hardware_location1,
												arduino_tools_location1,
												arduino_tools_location2,
												fully_qual_board_name,
												wavetable_lib_location,
												teensy_lib_location)
path = "*.h"

## This class is for testing the command line input for the decoder
class DecoderTestMethods(unittest.TestCase):
    ## Create the first instrument and first sample for all files in Soundfonts folder
    @patch.object(decoder, 'safe_input', create=True)
    def test_decodeFileTest(self, safe_input):        
        with open('sflist', 'r') as sf2_names:
            for name in sf2_names: 
                out=StringIO()
                safe_input.side_effect = [1, 1, 2, 1, 2]
                with redirect_stdout(out): 
                    argv = ['-i', '../Soundfonts/' + name.strip(), '-o' 'Test']
                    decoder.main(argv)
                    self.assertIn("success", out.getvalue())
                copyfile('./Test_samples.h', './AutoTest/Test_samples.h')
                copyfile('./Test_samples.cpp', './AutoTest/Test_samples.cpp')
                print("\nNow uploading " + name + " to the Teensy...\n")
                check_output(arduino_upload_str, shell=True)
                print("\nCompiled and uploaded " + name + " to Teensy successfully! Next test...\n")

    ## Check that an invalid .sf2 file throws a type error exception
    def test_invalidSF2FileThrowsTypeError(self):
        out=StringIO()
        argv = ['-i', 'notARealFile']
        with redirect_stdout(out), self.assertRaises(SystemExit) as code:
            self.assertRaises(TypeError, decoder.main, argv)
        self.assertEqual(code.exception.code, 2)

    ## Check that if an invalid option is given
    def test_invalidOptionsThrowsGetoptError(self):
        out=StringIO()
        argv = ['-j', 'test.sf2']
        with redirect_stdout(out), self.assertRaises(SystemExit) as code:
            self.assertRaises(getopt.GetoptError, decoder.main, argv)
        self.assertEqual(code.exception.code, 2)       

    ## Check that that malformed .sf2 raises exception by passing empty file
    def test_malformedSF2FileRaisesTypeError(self):
        out=StringIO()
        argv = ['-i', 'malformed.sf2']
        with redirect_stdout(out), self.assertRaises(SystemExit) as code:
            self.assertRaises(TypeError, decoder.main, argv)
        self.assertEqual(code.exception.code, 2)       

    ## Check that if a given .sf2 doesn't exist an exception is raised
    def test_missingSF2FileRaisesFileNotFoundException(self):
        out=StringIO()
        argv = ['-i', 'notARealFile.sf2']
        with redirect_stdout(out), self.assertRaises(SystemExit) as code:
            self.assertRaises(FileNotFoundError, decoder.main, argv)
        self.assertEqual(code.exception.code, 2)       

    ## Check for safe input. To exit the safe_input function valid input must be
    #  given. That's why 1 is given every other input.
    def test_ifInvalidInputIsGivenAtMenuErrorIsDisplayed(self):
        with patch('builtins.input', create=True, side_effect=[1,5,1,-2,1]):
            out=StringIO()
            with redirect_stdout(out):
                # check valid input
                self.assertEqual(decoder.safe_input(['one', 'two', 'three'], min_=1, max_=3), 1)
                out.truncate(0) #clear IO 

                # check too high
                decoder.safe_input(['one', 'two', 'three'], min_=1, max_=3)
                self.assertIn("Input must be less than", out.getvalue())
                out.truncate(0) #clear IO 

                # check too low
                decoder.safe_input(['one', 'two', 'three'], min_=1, max_=3)
                self.assertIn("Input must be greater than", out.getvalue())
                out.truncate(0) #clear IO

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(DecoderTestMethods)
    unittest.TextTestRunner(verbosity=2).run(suite)
