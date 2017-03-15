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

import sf2elements
from sf2utils.sf2parse import Sf2File



class MyModel():
    def __init__(self,vc):
        #set delegate/callback pointer
        self.controller = vc
        # infile = Soundfont file loaded by user.
        self.inFile = None
        # Most recently selected Instrument
        self.curr_instrument = None
        # out_dir = filepath to the folder where the output will be saved
        self.out_dir = "."
        # out_name = name of the output files. Default is intrument name.
        self.out_name = None
        # curr_samples = list of 'sf2elements.Sample
        self.curr_samples = list()
        # Instruments = list of 'sf2elements.Instrument' objects
        self.Instruments = list()
        self.total_sample_size = 0
        self.teensy_mem_size = 250 #default 250kb Teensy 3.2

#Model would call this on internal change
    def modelChange(self):
        self.controller.modelChanged()
#Setters and getters for the model
    def setInFile(self, _new):
    # Set the Soundfont file that the user has selected.
        self.inFile =_new
    def getInFile(self):
    # Return the currently loaded Soundfont file.
        return self.inFile
    def setOutDir(self, _new):
        self.out_dir = _new
    def getOutDir(self):
        return self.out_dir
    def setCurrInstrument(self, _index):
        self.curr_instrument = self.Instruments[_index]
    def getCurrInstrument(self):
        return self.curr_instrument
    def setInstrumentList(self, _new):
        self.Instruments = _new
    def getInstrumentList(self):
        return self.Instruments
    def setCurrSamples(self, _idxs):
        """
        Takes a sequence of indices corresponding to selected items
        and uses those indices to set the corresponding samples in
        the current instrument as the current samples.
        """
        temp = list()
        samps = self.curr_instrument.getSamples()
        for idx in _idxs:
            temp.append(samps[idx])
        self.curr_samples = temp
    def getCurrSamples(self):
        return self.curr_samples
    def setTotalSampleSize(self, idxs):
        total = 0
        if len(idxs) > 0:
            inst = self.getCurrInstrument()
            for i in idxs:
                total += inst.Samples[i].size
        self.total_sample_size = total
    def getTotalSampleSize(self):
        return '%.2f'%self.total_sample_size
    def setTeensyMemSize(self, _new):
        self.teensy_mem_size = _new
    def getTeensyMemSize(self):
        return self.teensy_mem_size
    def getTeensyPercentUsage(self):
        return '%.1f'%((self.total_sample_size/self.teensy_mem_size)*100)
# Internal processing for the model
    def loadSoundfont(self):
        """
        This function is constrained by the size of main memory. It
        will instantiate an object for each instrument which will
        contain a list of all the layers/bags that make up said
        instrument. The data used us no-where near the entirety of
        the Soundfont/SF2 so available main memory does not need to
        be greater than the size of the SF2. I don't see this as a
        serious concern but felt it should be noted.
        """
        with open(self.inFile, 'rb') as sf2_file:
            sf2 = Sf2File(sf2_file)
        sf2Instruments = list()
        inst_index = 0
        for inst in sf2.instruments:
            if inst.name == 'EOI':
                break
            sf2Instruments.append(sf2elements.Instrument(inst.name, inst_index))
            inst_index += 1
            bag_index = 0
            for bag in inst.bags:
                if bag.sample is None:
                    sf2Instruments[-1].setGlobalBag(bag_index)
                elif bag.sample is not None and bag.sample not in sf2Instruments[-1].Samples:
                    sf2Instruments[-1].Samples.append(sf2elements.Sample(bag.sample.name, bag_index, bag.key_range, bag.sample.duration))
                bag_index += 1
        # sorted ascending by ascii value of instrument names
        # preserves case but sorts indifferent to it
        # the result is upper and lower case being intermingled
        sf2Instruments.sort(key=lambda x: str.lower(x.i_name))
        self.setInstrumentList(sf2Instruments)
