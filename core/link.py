from .pmat import PMat

# Link algos

def spring(link):
    d = link.pmat1.distance(link.pmat2)
    f = link.k * (1 - (link.l0 / d))
    M1M2 = link.pmat1.pos - link.pmat2.pos
    link.pmat1.force += f * M1M2
    link.pmat2.force -= f * M1M2

class Link:

    def __init__(self, k):
        self.pmat1 = None
        self.pmat2 = None
        self.k = k
        self.z = 0
        self.l0 = 0
        # self.s1 = 0
        # self.s2 = 0

    def connect(self, pmat1, pmat2):
        self.pmat1 = pmat1
        self.pmat2 = pmat2
        self.l0 = self.pmat1.distance(self.pmat2)

    # algo_ressort
    def algo(self):
        spring(self)
