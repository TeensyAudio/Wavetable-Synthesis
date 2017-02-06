class Instrument:
    def __init__(self, i_name):
        self.i_name = i_name
        self.gb_idx = None
        self.Samples = list()

    def set_gb_idx(self, index):
        self.gb_idx = index


class Sample:
    def __init__(self, s_name, bag_idx):
        self.s_name = s_name
        self.bag_idx = bag_idx
