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

from tkinter import *
from tkinter import Tk, Label, Button, filedialog
from tkinter.ttk import *
import view
import model
import sf2elements
import decoder

class MyController():
    def __init__(self,parent):
        self.parent = parent
        self.model = model.MyModel(self)
        self.view = view.MyView(parent, self)

    #Input Handlers
    def inBrowseSelected(self):
        temp = filedialog.askopenfilename(filetypes=(("sf2 files", "*.sf2"), ("SF2 files", "*.SF2")))
        self.model.setInFile(temp)
        self.view.setInFile(temp)
        self.model.loadSoundfont()
        self.view.setInstrumentList(list(map(lambda x: x.i_name, self.model.getInstrumentList())))
        self.view.setSampleList([])
    def outBrowseSelected(self):
        self.model.setOutDir(filedialog.askdirectory())
        self.view.setOutDirectory(self.model.out_dir)
    def instrumentSelected(self, _selection):
        idxs = _selection
        if len(idxs) == 1:
            self.model.setCurrInstrument(int(idxs[0]))
            curr_inst = self.model.getCurrInstrument()
            self.view.setSampleList(curr_inst.samplesForDisplay())
        #reset size labels
        self.view.setTotalSampleSize(0)
        self.view.setNumSelected(0)
        self.view.setTeensyPercent(0.0)
        self.model.setTotalSampleSize([])

    def sampleSelected(self, _selection):
        idxs = _selection
        if(len(idxs) > 0):
            self.model.setCurrSamples(idxs)  
            #sample count
            self.view.setNumSelected(len(idxs))
            #total sample size
            self.model.setTotalSampleSize(idxs)
            self.view.setTotalSampleSize(self.model.getTotalSampleSize())
            #percent teensy usage
            self.view.setTeensyPercent(self.model.getTeensyPercentUsage())
    def setTeensyMemSize(self, size):
        self.model.setTeensyMemSize(size)
        self.view.setTeensyPercent(self.model.getTeensyPercentUsage())
    def decode(self, _selection):
        if(len(_selection) == 0):
            return
        print('decode')
        self.sampleSelected(_selection)
        selected_bags = list()
        curr_inst = self.model.getCurrInstrument()
        inFile = self.model.getInFile()

        # check for user supplied file name
        # else default is instrument name
        default_name_check = self.view.getDefaultOutName()
        out_name = self.view.getOutName()
        if out_name == default_name_check:
            out_name = None
        elif out_name != default_name_check:
            pass

        for samp in self.model.getCurrSamples():
            selected_bags.append(samp.bag_idx)
        if decoder.decode_selected(inFile, curr_inst.getOriginalIndex(),
                selected_bags, curr_inst.getGlobalBag(), out_name, self.model.getOutDir()):
            self.view.setStatus('Decode Successful!')
            self.decodeConfirmation()
        else:
            self.view.setStatus('ERROR! Failed to Decode!')

    def decodeConfirmation(self):
        decoded_samples = ['Decoded sample(s):']
        for samp in self.model.getCurrSamples():
            decoded_samples.append(str(samp.getName()) + str(samp.getKeyRange()))
        self.view.setSampleList(decoded_samples)

# Model update responses

    # delegates -- add functions called by delegtes in model or view
    def modelChanged(self):
        pass

def main():
    root = Tk()
    root.title('Teensy Soundfont Decoder')
    root.geometry("650x650+450+100")  # height x width + horizontal_pos + vertical_pos
    app = MyController(root)
    root.mainloop()

if __name__ == '__main__':
    main()
