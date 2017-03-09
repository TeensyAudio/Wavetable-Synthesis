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
#

class Instrument:
    def __init__(self, i_name, index):
        self.i_name = i_name
        self.original_index = index
        self.gb_idx = None
        self.Samples = list()

    def samplesForDisplay(self):
        samps = list()

        for samp in self.Samples:
            temp = str(samp.getName()) + str(samp.getKeyRange())
            samps.append(temp)

        return samps

    # Getters/Setters
    def getOriginalIndex(self):
        return self.original_index
    def getSamples(self):
        return self.Samples
    def setSamples(self, _new):
        self.Samples = _new
    def getGlobalBag(self):
        return self.gb_idx
    def setGlobalBag(self, index):
        self.gb_idx = index

class Sample:
    def __init__(self, s_name, bag_idx, key_range, size):
        self.s_name = s_name
        self.bag_idx = bag_idx
        self.key_range = key_range
        self.size = (size*2)/1000 # size parameter is total # samples(16 bits a piece) 
                                  # size*2 gives bytes. Divide 1000 gives kb.

    def getName(self):
        return self.s_name
    def setName(self, _new):
        self.s_name = _new
    def getBagIndex(self):
        return self.bag_idx
    def setBagIndex(self, _new):
        self.bag_idx = _new
    def getKeyRange(self):
        return self.key_range
    def setKeyRange(self, _new):
        self.key_range = _new
