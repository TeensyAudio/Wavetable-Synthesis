from tkinter import *
from sf2utils.sf2parse import Sf2File
from tkinter import Tk, Label, Button, filedialog
from tkinter import N, S, E, W
from tkinter.ttk import *
import decoder
import sf2elements
import JoJenWidgets as JJ

class Application(Frame):
    def __init__(self, parent, **kw):
        super().__init__(parent)

        self.parent = parent
        self.parent.title("Teensy Soundfont Decoder")
        # vital variables
        self.inFile = None
        # currInst logic needs to be changed if implementing ordering
        self.currInst = None
        self.out_dir = StringVar(value='Select Directory...')
        self.out_name = None
        self.total_sample_size = IntVar(value=0)

        # control variables
        self.i_names = StringVar()
        self.s_names = StringVar()
        self.ver_var = IntVar(value=1)

        # List of instrument object instances
        self.Instruments = list()

        self.lrg_font = 14
        self.med_font = 12
        self.sml_font = 10
        self.box_pad = 1
        self.box_relief = RIDGE

        s = Style()
        s.theme_use('default')
        s.configure('.', font=('default', self.sml_font))
        # s.configure('TRadiobutton', font=('default', self.med_font))
        s.configure('TLabelframe.Label', font=('default', self.med_font))
        def_pad = 3

        self.parent.columnconfigure(0, weight=1)
        self.parent.rowconfigure(0, weight=1)
        self.grid(column=0, row=0, sticky=N + S + E + W)
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=6)
        # self.config(relief=SUNKEN)
    # upper frame
        self.frame_1 = JJ.JJFrame(self)
        # self.frame_1 = Frame(self)
        self.frame_1.grid(column=0, row=0, columnspan=4, sticky=N + S + E + W, padx=5, pady=5)
        self.frame_1.rcconfigure(_rows=[(0, 1)], _columns=[(0, 1), (1, 2), (2, 3)])
        # self.frame_1.columnconfigure(0, weight=1)
        # self.frame_1.columnconfigure(1, weight=2)
        # self.frame_1.columnconfigure(2, weight=3)
        # self.frame_1.rowconfigure(0, weight=1)

        # Box 1
        # self.box_1 = LabelFrame(self.frame_1, text='Select Teensy Version', relief=self.box_relief)
        self.box_1 = JJ.JJLabelFrame(self.frame_1, 2, 1, 1, 1, text='Select Teensy Version')
        self.box_1.grid(column=0, row=0, padx=self.box_pad, pady=self.box_pad, sticky=N + S + E + W)
        # self.box_1.columnconfigure(0, weight=1)
        # self.box_1.rowconfigure(0, weight=1)
        # self.box_1.rowconfigure(1, weight=2)
        # self.box_1_label = Label(self.box_1, text='Select Teensy Version',relief=RAISED, font=('default', self.med_font))
        # self.box_1_label.grid(row=0, padx=def_pad, pady=(def_pad+5), sticky=N)
        self.rb_frame = Frame(self.box_1)
        self.rb_frame.grid(row=1, padx=def_pad, sticky=N + E + W)
        self.rb_frame.columnconfigure(0, weight=1)
        self.rb_frame.rowconfigure(0, weight=1)
        self.rb_frame.rowconfigure(1, weight=1)
        self.ver_32 = Radiobutton(self.rb_frame, text='Teensy 3.2 (default)', variable=self.ver_var, value=1)
        self.ver_32.grid(row=0, column=0, sticky=N + S + E + W, padx=def_pad)
        self.ver_36 = Radiobutton(self.rb_frame, text='Teensy 3.6', variable=self.ver_var, value=2)
        self.ver_36.grid(row=1, column=0, sticky=N + S + E + W, padx=def_pad)
        # Box 2
        self.box_2 = LabelFrame(self.frame_1, text='Load a Soundfont File', relief=self.box_relief)
        self.box_2.grid(column=1, row=0, padx=self.box_pad, pady=self.box_pad, sticky=N + S + E + W)
        self.box_2.columnconfigure(0, weight=1)
        self.box_2.rowconfigure(0, weight=1)
        self.box_2.rowconfigure(1, weight=2)
        # self.box_2_label = Label(self.box_2, text='Load a Soundfont File', font=('default', self.med_font))
        # self.box_2_label.grid(row=0, padx=def_pad, pady=(def_pad+5), sticky=N)
        # Browse button
        self.browse_button = Button(self.box_2, text="Browse", command=self.get_file)
        self.browse_button.grid(row=1, column=0, padx=def_pad, pady=def_pad)
        # Box 3
        self.box_3 = LabelFrame(self.frame_1, text='Output Settings', relief=self.box_relief)
        self.box_3.grid(column=2, row=0, padx=self.box_pad, pady=self.box_pad, sticky=N + S + E + W)
        self.box_3.columnconfigure(0, weight=1)
        self.box_3.rowconfigure(0, weight=1)
        self.box_3.rowconfigure(1, weight=1)

        self.dir_line = Frame(self.box_3)
        self.dir_line.grid(row=0, column=0)
        self.folder_label = Label(self.dir_line, text='Folder')
        self.folder_label.grid(row=0, column=0)
        self.folder_entry = Entry(self.dir_line, textvariable=self.out_dir)
        self.folder_entry.grid(row=0, column=1)
        self.folder_button = Button(self.dir_line, text='Browse', command=self.set_dir)
        self.folder_button.grid(row=0, column=2)
        # self.box_3_label = Label(self.box_3, text='Output Settings', font=('default', self.med_font))
        # self.box_3_label.grid(row=0, padx=def_pad, pady=(def_pad+5), sticky=N)
    # lower frame
        self.frame_2 = Frame(self)
        self.frame_2.grid(column=0, row=1, columnspan=4, sticky=N + S + E + W, padx=5, pady=5)
        self.frame_2.columnconfigure(0, weight=1)
        self.frame_2.columnconfigure(1, weight=1)
        self.frame_2.rowconfigure(0, weight=1)
        # left side of lower
        self.sub_frame_1 = Frame(self.frame_2)
        self.sub_frame_1.grid(column=0, row=0, sticky=N + S + E + W, padx=5)
        self.sub_frame_1.columnconfigure(0, weight=1)
        self.sub_frame_1.rowconfigure(0, weight=1)
        self.sub_frame_1.rowconfigure(1, weight=16)
            # Instruments
        self.instList_label = Label(self.sub_frame_1, text='Instruments', font=('default', self.lrg_font))
        self.instList_label.grid(row=0, column=0, padx=1, pady=1)
        self.instList = Listbox(self.sub_frame_1, listvariable=self.i_names, font=('default', self.med_font))
        self.instList.grid(row=1, column=0, rowspan=2, sticky=N + S + E + W)
        self.scrollbar_1 = Scrollbar(self.sub_frame_1, command=self.instList.yview)
        self.scrollbar_1.grid(row=1, column=1, rowspan=2, sticky=N + S + E + W)
        self.instList.config(yscrollcommand=self.scrollbar_1.set)
        # right side of lower
        self.sub_frame_2 = Frame(self.frame_2)
        self.sub_frame_2.grid(column=1, row=0, sticky=N + S + E + W, padx=5)
        self.sub_frame_2.columnconfigure(0, weight=1)
        self.sub_frame_2.columnconfigure(1, weight=3)
        self.sub_frame_2.columnconfigure(2, weight=1)
        self.sub_frame_2.rowconfigure(0, weight=1)
        self.sub_frame_2.rowconfigure(1, weight=15)
        self.sub_frame_2.rowconfigure(2, weight=1)
            # Samples
        self.sampList_label = Label(self.sub_frame_2, text='Samples', font=('default', self.lrg_font))
        self.sampList_label.grid(row=0, column=0, columnspan=3, padx=1, pady=1)
        self.sampList = Listbox(self.sub_frame_2, listvariable=self.s_names, selectmode=EXTENDED, font=('default', self.med_font))
        self.sampList.grid(row=1, column=0, columnspan=3, sticky=N + S + E + W)
        self.scrollbar_2 = Scrollbar(self.sub_frame_2, command=self.sampList.yview)
        self.scrollbar_2.grid(row=1, column=4, rowspan=1, sticky=N + S + E + W)
        self.sampList.config(yscrollcommand=self.scrollbar_2.set)
        self.exp_button = Button(self.sub_frame_2, text='Decode', command=self.send_to_decoder)
        self.exp_button.grid(row=2, column=2, padx=5, pady=5, sticky=N + S + E +W)

        # JOSH! THIS IS YOURS
        # Use IntVar.get() / IntVar.set()
        self.samp_size_label = Label(self.sub_frame_2, textvariable=self.total_sample_size)
        self.samp_size_label.grid(row=2, column=1, padx=2, pady=2, sticky=N + S + E + W)

        # Main Menu Bar
        menu = Menu(parent)
        parent.config(menu=menu)
        filemenu = Menu(menu)
        menu.add_cascade(label='File', menu=filemenu)
        filemenu.add_command(label='Load SF2', command=self.get_file)
        filemenu.add_command(label="Save location...", command=self.set_dir)
        menu.add_command(label='Help', command=self.call_back)
        menu.add_command(label='About', command=self.call_back)

        # Status Bar (at bottom)
        self.status_text = StringVar()
        self.status_text.set('Load a Soundfont File')
        self.status = Label(self, textvariable=self.status_text, border=1, anchor=W, relief=RIDGE, font=('default', self.sml_font))
        self.status.grid(column=0, row=2, sticky=W + E)

        # Set Bindings
        self.instList.bind('<<ListboxSelect>>', self.update_samples)
        self.sampList.bind('<Double-1>', self.send_to_decoder)

    def call_back(self):
        print('called the callback')

    def set_dir(self):
        self.out_dir.set(filedialog.askdirectory())

    def send_to_decoder(self, *args):
        sel_samps = self.sampList.curselection()
        selected_bags = list()
        for samp in sel_samps:
            selected_bags.append(self.Instruments[self.currInst].Samples[int(samp)].bag_idx)
        if decoder.decode_selected(self.inFile, self.currInst, selected_bags, self.Instruments[self.currInst].gb_idx):
            self.status_text.set('Decode Successful!')
        else:
            self.status_text.set('ERROR! Failed to Decode!')

    def update_samples(self, *args):
        idxs = self.instList.curselection()
        if len(idxs) == 1:
            idx = int(idxs[0])
            temp_list = list(map(lambda x: x.s_name, self.Instruments[idx].Samples))
            self.s_names.set(temp_list)
            self.currInst = idx
        self.status_text.set('Select Sample(s)')

    def get_file(self):
        self.inFile = filedialog.askopenfilename(filetypes=(("sf2 files", "*.sf2"), ("SF2 files", "*.SF2")))
        with open(self.inFile, 'rb') as sf2_file:
            sf2 = Sf2File(sf2_file)

        self.Instruments.clear()

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
                bag_index += 1
        # Set instrument control variable
        self.i_names.set(list(map(lambda x: x.i_name, self.Instruments)))
        self.status_text.set('Select an Instrument')


if __name__ == "__main__":
    root = Tk()
    root.geometry("650x650+450+100")  # height x width + horizontal + vertical
    app = Application(root)
    root.mainloop()
