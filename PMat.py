import numpy as np


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