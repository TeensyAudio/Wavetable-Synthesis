#!/usr/bin/env python3

import sys
import decoder
from sf2utils.sf2parse import Sf2File
from struct import *
from collections import namedtuple

if len(sys.argv) < 2:
	path = raw_input("Please provide filepath to an SF2: ")
#	path = message.prompt("Please provide filepath to an SF2: ")
else:
	path = sys.argv[1]

with open (path, 'rb') as sf2_file:
	sf2 = Sf2File(sf2_file)
	for i, j in enumerate(sf2.samples, 1):
		print("{}. {}".format(i, j.name))

	print('Select an sample')
	selection = int(input('Input the corresponding number: '))
	selection = selection -1
	print(sf2.samples[selection])

decoder.main(path, selection)
