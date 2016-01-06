from .pmat import PMat

# Link algos

def spring(link):
    d = link.pmat1.distance(link.pmat2)
    f = -link.k * (1 - (link.l0 / d))
    M1M2 = link.pmat2.position - link.pmat1.position
    link.pmat1.force -= f * M1M2
    link.pmat2.force += f * M1M2

def frein(link):
    f = - link.z * (link.pmat1.velocity - link.pmat2.velocity)
    link.pmat1.force += f 
    link.pmat2.force -= f
    
def springFrein(link):
    d = link.pmat1.distance(link.pmat2)
    f = -link.k * (1 - (link.l0 / d))
    M1M2 = link.pmat2.position - link.pmat1.position
    frein = - link.z * (link.pmat1.velocity - link.pmat2.velocity)
    link.pmat1.force -= f * M1M2 - frein
    link.pmat2.force += f * M1M2 - frein
      

class Link:

    def __init__(self, k, z):
        self.pmat1 = None
        self.pmat2 = None
        self.k = k
        self.z = z
        self.l0 = 0
        # self.s1 = 0
        # self.s2 = 0

    def connect(self, pmat1, pmat2):
        self.pmat1 = pmat1
        self.pmat2 = pmat2
        self.l0 = self.pmat1.distance(self.pmat2)

    # algo_ressort
    def algo(self):
    	# spring(self)
        # frein(self)
        springFrein(self)

    def draw(self, canvas):
        x1, y1 = tuple(self.pmat1.position)
        x2, y2 = tuple(self.pmat2.position)
        color = "black"
        d = self.pmat1.distance(self.pmat2)
        if self.l0 > d :
            color = "red"
        elif self.l0 < d:
            color = "blue"
        canvas.create_line(x1, y1, x2, y2, fill=color)
