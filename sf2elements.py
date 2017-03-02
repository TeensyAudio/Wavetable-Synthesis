class Instrument:
    def __init__(self, i_name):
        self.i_name = i_name
        self.gb_idx = None
        self.Samples = list()

    def set_gb_idx(self, index):
        self.gb_idx = index

class Sample:
    def __init__(self, s_name, bag_idx, size):
        self.s_name = s_name
        self.bag_idx = bag_idx
        self.key_range = key_range
        self.size = (size*2)/1000; # size parameter is total # samples(16 bits a piece) 
                                   # size*2 gives bytes. Divide 1000 gives kb.
