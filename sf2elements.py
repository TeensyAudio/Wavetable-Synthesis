class Instrument:
    def __init__(self, i_name, index):
        self.i_name = i_name
        self.original_index = index
        self.gb_idx = None
        self.Samples = list()

    def samplesForDisplay(self):
        samps = list()

        for samp in self.Samples:
            temp = str(samp.getName()) + str(samp.getKeyRange())
            samps.append(temp)

        return samps

    # Getters/Setters
    def getOriginalIndex(self):
        return self.original_index
    def getSamples(self):
        return self.Samples
    def setSamples(self, _new):
        self.Samples = _new
    def getGlobalBag(self):
        return self.gb_idx
    def setGlobalBag(self, index):
        self.gb_idx = index

class Sample:
    def __init__(self, s_name, bag_idx, key_range, size):
        # self.ownder = owner
        self.s_name = s_name
        self.bag_idx = bag_idx
        self.key_range = key_range
        self.size = (size*2)/1000 # size parameter is total # samples(16 bits a piece) 
                                  # size*2 gives bytes. Divide 1000 gives kb.

    def getName(self):
        return self.s_name
    def setName(self, _new):
        self.s_name = _new
    def getBagIndex(self):
        return self.bag_idx
    def setBagIndex(self, _new):
        self.bag_idx = _new
    def getKeyRange(self):
        return self.key_range
    def setKeyRange(self, _new):
        self.key_range = _new
