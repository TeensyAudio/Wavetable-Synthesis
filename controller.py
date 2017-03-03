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
        self.view.setInstrumentList(list(map(lambda x: x.i_name, self.model.Instruments))) 
    def outBrowseSelected(self):
        self.model.setOutDir(filedialog.askdirectory())
        self.view.setOutDirectory(self.model.out_dir)
    def instrumentSelected(self, _selection):
        idxs = _selection
        if len(idxs) == 1:
            self.model.setCurrInstrument(int(idxs[0]))
            self.model.update_samples()
            #TODO set curr in view
            self.view.setSampleList(self.model.samples)
        #reset size labels
        self.view.setTotalSampleSize(0)
        self.view.setNumSelected(0)
        self.view.setTeensyPercent(0)
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
        self.sampleSelected(_selection)
        selected_bags = list()
        curr_inst = self.model.getCurrInstrument()
        inFile = self.model.getInFile()
        gb_idx = self.model.Instruments[curr_inst].gb_idx
        selected_samples = _selection

        # check for user supplied file name
        # else default is instrument name
        default_name_check = self.view.getDefaultOutName()
        out_name = self.view.getOutName()
        if out_name == default_name_check:
            out_name = None
        elif out_name != default_name_check:
            pass

        # TODO user supplied filepath after updating decoder.py
        # 2/28/2017

        for samp in selected_samples:
            selected_bags.append(self.model.Instruments[curr_inst].Samples[int(samp)].bag_idx)
        if decoder.decode_selected(inFile, curr_inst, selected_bags, gb_idx, out_name):
            self.view.setStatus('Decode Successful!')
            self.decodeConfirmation()
        else:
            self.view.setStatus('ERROR! Failed to Decode!')

    def decodeConfirmation(self):
        decoded_samples = ['Decoded sample(s):']
        samples = self.model.samples
        for idx in self.model.getCurrSamples():
            decoded_samples.append(samples[int(idx)])
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
