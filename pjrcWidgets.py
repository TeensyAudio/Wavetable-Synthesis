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
from tkinter import Label
from tkinter.ttk import *


class JJFrame(Frame):
    def __init__(self, parent, rows=1, columns=1, rweight=1, cweight=1):
        super().__init__(parent)

        self.autoconfigure(rows, columns, rweight, cweight)

    def configurerows(self, _rows):
        """
        Summary:
            Sets weight of a frames rows given a list of row number/weight pairs.

        Parameters
        configurerows(
        """
        for row in _rows:
            self.rowconfigure(row[0], weight=row[1])

    def configurecolumns(self, _columns):
        """
        Summary:
            Sets weight of a frames columns given a list of column number/weight pairs.

        """
        for column in _columns:
            self.columnconfigure(column[0], weight=column[1])

    def rcconfigure(self, _rows, _columns):
        self.configurerows(_rows)
        self.configurecolumns(_columns)

    def autoconfigure(self, _rows, _columns, _rweight=1, _cweight=1):
        """Set the WEIGHT of rows and columns to some integer value. """
        for row in range(_rows):
            self.rowconfigure(row, weight=_rweight)

        for column in range(_columns):
            self.columnconfigure(column, weight=_cweight)


class JJLabelFrame(LabelFrame):
    def __init__(self, parent, rows=1, columns=1, rweight=1, cweight=1, **kwargs):
        super().__init__(parent, **kwargs)

        self.autoconfigure(rows, columns, rweight, cweight)

    def configurerows(self, _rows):
        """
        Summary:
            Sets weight of a frames rows given a list of row number/weight pairs.

        Parameters
        configurerows(
        """
        for row in _rows:
            self.rowconfigure(row[0], weight=row[1])

    def configurecolumns(self, _columns):
        """
        Summary:
            Sets weight of a frames columns given a list of column number/weight pairs.

        """
        for column in _columns:
            self.columnconfigure(column[0], weight=column[1])

    def rcconfigure(self, _rows, _columns):
        self.configurerows(_rows)
        self.configurecolumns(_columns)

    def autoconfigure(self, _rows, _columns, _rweight=1, _cweight=1):
        """Set the WEIGHT of rows and columns to some integer value. """
        for row in range(_rows):
            self.rowconfigure(row, weight=_rweight)

        for column in range(_columns):
            self.columnconfigure(column, weight=_cweight)


class JJListBox(JJFrame):
    lrg_font = 14
    med_font = 12
    sml_font = 10

    def __init__(self, parent, _title='ListBox', _listvar=None):
        super().__init__(parent)

        self.title_label = Label(self, text=_title, font=('default', self.lrg_font))
        self.title_label.grid(row=0, column=0, padx=1, pady=1)
        self.list_box = Listbox(self, listvariable=_listvar, font=('default', self.med_font))
        self.list_box.grid(row=1, column=0, rowspan=2, sticky=N + S + E + W)
        self.scrollbar_1 = Scrollbar(self, command=self.list_box.yview)
        self.scrollbar_1.grid(row=1, column=1, rowspan=2, sticky=N + S + E + W)
        self.list_box.config(yscrollcommand=self.scrollbar_1.set)

        self.rcconfigure([(0,1),(1,16)], [(0,1)])

    def getCurrSelection(self):
        return self.list_box.curselection()

class JJStatusBar(JJFrame):
    def __init__(self, parent, _initVal=''):
        super().__init__(parent)

        self.autoconfigure(1, 1, 1, 1)
        self.config(borderwidth=0, relief=GROOVE)
        self.status_text = StringVar(value=_initVal)
        self.status_label = Label(self, textvariable=self.status_text, anchor=W, font=('default', 12))
        self.status_label.grid(column=0, row=0, sticky=W+E, pady=1)

    def setStatus(self, _new):
        self.status_text.set(_new)

class JJMenu(Menu):
    def __init__(self, parent):
        super().__init__(parent)

        parent.config(menu=self)

        self.filemenu = Menu(self)
        self.add_cascade(label='File', menu=self.filemenu)

