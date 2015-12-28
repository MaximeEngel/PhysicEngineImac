class Motor:

    def __init__(self):
        self.links = []
        self.pmats = []

    def simulate(self):
        for pmat in self.pmats:
            pmat.algo()
        for link in self.links:
            link.algo()

    def draw(self):
        for pmat in self.pmats:
            pmat.draw()
        for link in self.links:
            link.draw()
