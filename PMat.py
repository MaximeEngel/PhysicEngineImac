h = 0.1

class PMat:

    def __init__(self, pos, vit, frc, m):
        self.pos = pos
        self.vit = vit
        self.frc = frc
        self.m = m

    def algo(self):
        pass

class PMatFix(PMat):

    def __init__(self, pos):
        super(PMatFix).__init__(pos, 0, 0, 1)

    def algo(self):
        self.frc = 0

class PMatMobile(PMat):

    def __init__(self, pos, vit, m):
        super(PMatMobile).__init__(pos, vit, 0, m)


    def algo(self):
        # Leap Frog
        self.vit += h / self.m * self.frc
        self.pos += h * self.vit
        self.frc = 0

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
        self.l0 = distance(self.pmat1.pos, self.pmat1.pos)

    # algo_ressort
    def algo(self):
        d = distance(self.pmat1.pos, self.pmat2.pos)
        f = self.k * (1 - (self.l0 / d))
        M1M2 = vecteur(self.pmat1.pos, self.pmat2.pos) # M1M2 est un vecteur
        self.pmat1.frc += f * M1M2
        self.pmat2.frc -= f * M1M2


class Moteur:

    def __init__(self):
        self.tabL = []
        self.tabM = []

    def modeleur(self):
        pointFixe = PMatFix(Vecteur())
        nbPointMobile = 10
        for i in xrange(nbPointMobile):
            self.tabM.add(PMatMobile())
        # boucle pour linker deux points entre eux en construisant ressort et connect points en parcourant tabM avec pas de vitesse et une masse de 1
        # k pour le ressort = 1000 car h = 0.01

    def simulateur(self):
        for pmat in self.tabM:
            pmat.algo()
        for link in self.tabL:
            link.algo()

    def draw(self):
        # call le draw de l'object


def __main__():
    moteur = Moteur()
    moteur.modeleur()
    moteur.simulateur()