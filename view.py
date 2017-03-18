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
from tkinter import Label, Button
from tkinter.ttk import *
import pjrcWidgets as pjrc

#View : User interface elements.
#       --Controller can send messages to it.
#       --View can call methods from Controller vc when an event happens.
#       --NEVER communicates with Model.
#       --Has setters and getters to communicate with controller
class MyView(Frame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        #make the view

        #set the delegate/callback pointer
        self.controller = controller
        self.parent = parent

        #control variables
        # i_names = instruments to be shown in the instrument listbox
        self.i_names = StringVar()
        # s_names = samples to be shown in the sample listbox
        self.s_names = StringVar()
        # version = teensy version reflected in the selected element
        # of the radiobutton in the 'select teensy version' frame.
        self.version = IntVar(value=1)
        # out_dir = reflects contents of the 'folder' entry in the
        # 'output settings' frame
        self.out_dir = StringVar(value='Select Directory...')
        self.default_out_name = 'Default is Instrument Name'
        # out_name = reflects contents of the 'name' entyr in the
        # 'output settings' frame
        self.out_name = StringVar(value=self.default_out_name)
        self.total_sample_size = IntVar(value=0)
        self.num_samples_selected = IntVar(value=0)
        self.teensy_percent_used = IntVar(value=0.0)
        # inFile = reflects the filepath that the user chooses as the
        # soundfont file to decode samples from
        self.inFile = StringVar(value='Select a Soundfont File to Continue')

        #formating variables
        self.lrg_font = 14
        self.med_font = 12
        self.sml_font = 10
        self.box_relief = RIDGE
        self.box_pad = 1
        def_pad = 3
        s = Style()
        s.theme_use('default')
        s.configure('.', font=('default', self.sml_font))
        s.configure('TRadiobutton', font=('default', self.med_font))
        s.configure('TLabelframe.Label', font=('default', self.med_font))

        #set up the widgets/layout
        self.parent.columnconfigure(0, weight=1)
        self.parent.rowconfigure(0, weight=1)
        self.grid(column=0, row=0, sticky=N + S + E + W)
        self.columnconfigure(0, weight=1)
        self.rowconfigure(0, weight=1) # Boxes
        self.rowconfigure(1, weight=6) # ListBoxes
        self.rowconfigure(2, weight=1) # StatusBar

        # main menu
        self.menu = pjrc.JJMenu(self.parent)
        self.parent.config(menu=self.menu)
        self.menu.filemenu.add_command(label='Load SF2', command=self.controller.inBrowseSelected)


        self.upper_frame = pjrc.JJFrame(self)
        self.upper_frame.rcconfigure(_rows=[(0,1)], _columns=[(0,1),(1,2),(2,3)])
        self.upper_frame.grid(column=0, row=0, columnspan=4, sticky=N + S + E + W, padx=5, pady=5)

        self.box_1 = pjrc.JJLabelFrame(self.upper_frame, 3, 1, 1, 1, text='Select Teensy Version')
        self.box_1.grid(column=0, row=0, padx=self.box_pad, pady=self.box_pad, sticky=N + S + E + W)
        self.version_frame = pjrc.JJFrame(self.box_1, 2, 1, 1, 1)
        self.version_frame.grid(row=1, padx=def_pad, sticky=N + E + W)
        self.ver_32 = Radiobutton(self.version_frame, text='Teensy 3.2 (default)',
                variable=self.version, command=lambda: self.teensyButtonSelect(1), value=1)
        self.ver_32.grid(row=0, column=0, sticky=N + S + E + W, padx=def_pad)
        self.ver_36 = Radiobutton(self.version_frame, text='Teensy 3.6',
                variable=self.version, command=lambda: self.teensyButtonSelect(2), value=2)
        self.ver_36.grid(row=1, column=0, sticky=N + S + E + W, padx=def_pad)

        self.box_2 = pjrc.JJLabelFrame(self.upper_frame, 2, 1, 1, 1, text='Load a SoundFont')
        self.box_2.grid(column=1, row=0, padx=self.box_pad, pady=self.box_pad, sticky=N + S + E + W)
        # TODO make this text wrap better
        self.infile_label = Message(self.box_2, textvariable=self.inFile, anchor=W, justify=LEFT, aspect=400)
        self.infile_label.grid(row=0, column=0, padx=def_pad, pady=def_pad, sticky=E + W)
        self.browse_button = Button(self.box_2, text="Browse", command=self.controller.inBrowseSelected)
        self.browse_button.grid(row=1, column=0, padx=def_pad, pady=def_pad, sticky=N)

        self.box_3 = pjrc.JJLabelFrame(self.upper_frame, 2, 1, 1, 1, text='Output Settings')
        self.box_3.grid(column=2, row=0, padx=self.box_pad, pady=self.box_pad, sticky=N + S + E + W)
        self.dir_line = Frame(self.box_3)
        self.dir_line.grid(row=0, column=0)
        self.folder_label = Label(self.dir_line, text='Folder')
        self.folder_label.grid(row=1, column=0)
        self.folder_entry = Entry(self.dir_line, textvariable=self.out_dir)
        self.folder_entry.grid(row=1, column=1)
        self.folder_button = Button(self.dir_line, text='Browse', command=self.controller.outBrowseSelected)
        self.folder_button.grid(row=1, column=2)
        self.name_label = Label(self.dir_line, text = 'Name')
        self.name_label.grid(row=0, column=0)
        self.name_entry = Entry(self.dir_line, textvariable=self.out_name)
        self.name_entry.grid(row=0, column=1, columnspan=2, sticky=E + W)

        self.lower_frame = pjrc.JJFrame(self, 1, 2, 1, 1)
        self.lower_frame.configurerows([(0,1), (1,0)])
        self.lower_frame.grid(column=0, row=1, columnspan=4, sticky=N + S + E + W, padx=1, pady=1)

        # Instruments
        self.inst_listbox = pjrc.JJListBox(self.lower_frame, 'Instruments', self.i_names)
        self.inst_listbox.grid(column=0, row=0, rowspan=2, sticky=N + S + E + W, padx=5)

        # Samples
        self.samp_listbox = pjrc.JJListBox(self.lower_frame, 'Samples', self.s_names)
        self.samp_listbox.grid(column=1, row=0, sticky=N + S + E + W, padx=5)
        self.samp_listbox.list_box.config(selectmode = EXTENDED)
        # self.samp_listbox.list_box.config(selectmode=MULTIPLE) FOR CHECKBOX STYLE SELECTION


        self.under_frame = pjrc.JJFrame(self.lower_frame, 1, 1, 1, 1)
        self.under_frame.grid(column=1, row=1, sticky=N + S + E + W)
        self.decode_button = Button(self.under_frame, text='Decode', command=self.samplesSelected)
        self.decode_button.grid(row=1, column=0, padx=2, pady=2)

        # Displays sample size, num selected and memory usage percentage for teensy
        #TODO put in different frame to fix listbox stretching
        self.stats_frame = pjrc.JJLabelFrame(self.under_frame, 3, 2, 1, 1, text='Sample Stats')
        self.stats_frame.grid(row=0, column=0, sticky=N + E + W, padx=5, pady=0)
        self.stats_frame.num_selected_text_label = Label(self.stats_frame, text='Num Samples Selected:', anchor=E)
        self.stats_frame.num_selected_text_label.grid(row=0, column=0, padx=2, pady=0, sticky=N + S + E)
        self.stats_frame.num_selected_label = Label(self.stats_frame, textvariable=self.num_samples_selected, anchor=W)
        self.stats_frame.num_selected_label.grid(row=0, column=1, padx=2, pady=0, sticky=N + S + W)
        self.stats_frame.samp_size_text_label = Label(self.stats_frame, text='Size of Selections (kb):', anchor=E)
        self.stats_frame.samp_size_text_label.grid(row=1, column=0, padx=2, pady=0, sticky=N + S + E)
        self.stats_frame.samp_size_label = Label(self.stats_frame, textvariable=self.total_sample_size, anchor=W)
        self.stats_frame.samp_size_label.grid(row=1, column=1, padx=2, pady=0, sticky=N + S + W)
        self.stats_frame.teensy_percent_text_label = Label(self.stats_frame, text='Est. Teensy Usage (%):', anchor=E)
        self.stats_frame.teensy_percent_text_label.grid(row=2, column=0, padx=2, pady=0, sticky=N + S + E)
        self.stats_frame.teensy_percent_label = Label(self.stats_frame, textvariable=self.teensy_percent_used, anchor=W)
        self.stats_frame.teensy_percent_label.grid(row=2, column=1, padx=2, pady=0, sticky=N + S + W)

        # Status bar at bottom
        self.status_bar = pjrc.JJStatusBar(self, 'Load a Soundfont')
        self.status_bar.grid(row=2, column=0, sticky=E + W + S, padx=0, pady=0)

        # Set Bindings
        self.inst_listbox.list_box.bind('<<ListboxSelect>>', self.instSelected)
        self.samp_listbox.list_box.bind('<<ListboxSelect>>', self.sampleSelected)
        self.samp_listbox.list_box.bind('<Double-1>', self.samplesSelected)

    # controller callback helpers
    def instSelected(self, *args):
        self.samp_listbox.list_box.config(state=NORMAL)
        self.controller.instrumentSelected(self.inst_listbox.getCurrSelection())
    def sampleSelected(self, *args):
        self.controller.sampleSelected(self.samp_listbox.getCurrSelection())
    def samplesSelected(self, *args):
        if len(self.samp_listbox.getCurrSelection()) > 0:
            self.samp_listbox.list_box.config(state=DISABLED)
            self.controller.decode(self.samp_listbox.getCurrSelection())
    def teensyButtonSelect(self, value):
        if value == 1:
            self.controller.setTeensyMemSize(250) #teensy 3.2 size in kb
        if value == 2:
            self.controller.setTeensyMemSize(1048) #teensy 3.6 size in kb


    # Setters and Getters for the control variables and such
    def setInstrumentList(self, _newInstruments):
        self.i_names.set(_newInstruments)
        for i in range(0, len(_newInstruments), 2):
            self.inst_listbox.list_box.itemconfigure(i, background='#f0f0ff')
    def getInstrumentList(self):
        return self.i_names.get()
    def setSampleList(self, _newSamples):
        self.s_names.set(_newSamples)
        for i in range(0, len(_newSamples), 2):
            self.samp_listbox.list_box.itemconfigure(i, background='#f0f0ff')
    def getSampleList(self):
        return self.s_names.get()
    def setInFile(self, _newFile):
        self.inFile.set(_newFile)
    def getInFile(self):
        return self.inFile.get()
    def setOutDirectory(self, _newDir):
        self.out_dir.set(_newDir)
    def getOutDirectory(self):
        return self.out_dir.get()
    def setOutName(self, _newName):
        self.out_name.set(_newName)
    def getOutName(self):
        return self.out_name.get()
    def setStatus(self, _new):
        self.status_bar.setStatus(_new)
    def getDefaultOutName(self):
        return self.default_out_name
    def setTotalSampleSize(self, _new):
        self.total_sample_size.set(_new)
    def getTotalSampleSize(self):
        return self.total_sample_size.get()
    def setNumSelected(self, _new):
        self.num_samples_selected.set(_new)
    def getNumSelected(self):
        return self.num_samples_selected.get()
    def setTeensyPercent(self, _new):
        self.teensy_percent_used.set(_new)
    def getTeensyPercent(self):
        return self.teensy_percent_used.get()
    def setVersion(self, _new):
        self.version.set(_new)
    def getVersion(self):
        return self.version.get()
