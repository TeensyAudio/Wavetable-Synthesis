from tkinter import *
from sf2utils.sf2parse import Sf2File
from tkinter import Tk, Label, Button, filedialog
from tkinter import RIGHT, LEFT, BOTH, RAISED, X, Y, TOP, Text, N, S, E, W, BOTTOM
from tkinter.ttk import *
import decoder
import sf2elements


class Application(Frame):

    def __init__(self, parent):
        Frame.__init__(self, parent, relief=SUNKEN)

        self.parent = parent
        parent.title("Teensy Soundfont Decoder")
        s = Style()
        s.theme_use('default')
        # s.configure("TFrame", background="#333")

        self.BOX_HEIGHT = 15
        self.inFile = None
        self.currInst = None

        # control variables
        self.i_names = StringVar()
        self.s_names = StringVar()
        # self.u_names = StringVar()

        # List of instrument object instances
        self.Instruments = list()

        # self.usrList = Listbox(parent, listvariable=self.u_names, height=self.BOX_HEIGHT)
        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(0, weight=1)
        self.grid(column=0, row=0, sticky=N+S+E+W)
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=4)
        frame_1 = Frame(self)
        frame_1.grid(column=0, row=0, columnspan=4, sticky=N+S+E+W, padx=5, pady=5)
        frame_1.columnconfigure(0, weight=1)
        frame_1.rowconfigure(0, weight=1)
        frame_1.rowconfigure(1, weight=1)
        frame_2 = Frame(self)
        frame_2.grid(column=0, row=1, columnspan=4, sticky=N+S+E+W, padx=5, pady=5)
        frame_2.columnconfigure(0, weight=1)
        frame_2.columnconfigure(1, weight=1)
        frame_2.rowconfigure(0, weight=1)
        frame_2.rowconfigure(1, weight=4)
        frame_2.rowconfigure(2, weight=1)
        self.inFile_label = Label(frame_1, text='Please Select a Soundfont File', font=('default', 12))
        self.inFile_label.grid(row=0, column=0)
        self.browse_button = Button(frame_1, text="Browse", command=self.get_file)
        self.browse_button.grid(row=1, column=0, sticky=E+W, padx=40)
        self.instList_label = Label(frame_2, text='Instruments', font=('default', 14))
        self.instList_label.grid(row=0, column=0, padx=1, pady=1)
        self.instList = Listbox(frame_2, listvariable=self.i_names, height=self.BOX_HEIGHT, font=('default', 12))
        self.instList.grid(row=1, column=0, rowspan=2, sticky=N+S+E+W, padx=5, pady=5)
        self.sampList_label = Label(frame_2, text='Samples', font=('default', 14))
        self.sampList_label.grid(row=0, column=1, padx=1, pady=1)
        self.sampList = Listbox(frame_2, listvariable=self.s_names, selectmode=EXTENDED, height=self.BOX_HEIGHT, font=('default',12))
        self.sampList.grid(row=1, column=1, sticky=N+S+E+W, padx=5, pady=5)
        self.exp_button = Button(frame_2, text='Decode', command=self.send_to_decoder)
        self.exp_button.grid(row=2, column=1, padx=5, pady=5, sticky=N+S+E+W)

        menu = Menu(parent)
        parent.config(menu=menu)
        filemenu = Menu(menu)
        menu.add_cascade(label='File', menu=filemenu)
        filemenu.add_command(label='Load SF2', command=self.get_file)

        menu.add_command(label='Help', command=self.call_back)
        menu.add_command(label='About', command=self.call_back)

        self.status_text = StringVar()
        self.status_text.set('Load a Soundfont File')
        parent.status = Label(parent, textvariable=self.status_text, border=1, anchor=W)
        parent.status.grid(column=0, row=3, sticky=W+E)

        # self.close_button = Button(parent, text="Close", command=parent.quit)

        # Set Bindings
        self.instList.bind('<<ListboxSelect>>', self.update_samples)
        self.sampList.bind('<Double-1>', self.send_to_decoder)

    def call_back(self):
        print('called the callback')

    def send_to_decoder(self, *args):
        sel_samps = self.sampList.curselection()
        selectedBags = list()
        for samp in sel_samps:
            selectedBags.append(self.Instruments[self.currInst].Samples[int(samp)].bag_idx)
        decoder.decodeSelected(self.inFile, self.currInst, selectedBags, self.Instruments[self.currInst].gb_idx)

    def update_samples(self, *args):
        idxs = self.instList.curselection()
        if len(idxs)==1:
            idx = int(idxs[0])
            temp_list = list(map(lambda x: x.s_name, self.Instruments[idx].Samples))
            self.s_names.set(temp_list)
            self.currInst = idx
        self.status_text.set('Select Sample(s)')

    def get_file(self):
        self.inFile = filedialog.askopenfilename(filetypes = (("sf2 files","*.sf2"),("SF2 files","*.SF2")))
        self.inFile_label.config(text=self.inFile)
        with open(self.inFile, 'rb') as sf2_file:
            sf2 = Sf2File(sf2_file)

        for inst in sf2.instruments:
            if inst.name == 'EOI':
                break
            self.Instruments.append(sf2elements.Instrument(inst.name))
            bag_index = 0
            for bag in inst.bags:
                if bag.sample is None:
                    self.Instruments[-1].set_gb_idx(bag_index)
                elif bag.sample is not None and bag.sample not in self.Instruments[-1].Samples:
                    self.Instruments[-1].Samples.append(sf2elements.Sample(bag.sample.name, bag_index))
                bag_index +=1
        # Set instrument control variable
        self.i_names.set(list(map(lambda x: x.i_name, self.Instruments)))
        self.status_text.set('Select an Instrument')

if __name__ == "__main__":
    root = Tk()
    root.geometry("450x450+450+450")
    app = Application(root)
    root.mainloop()
