import numpy as np
from .constants import H

# PMat algos

def fix(pmat):
    pmat.force = np.zeros(2)

def movable(pmat):
    # Leap Frog
    pmat.velocity += H / pmat.mass * pmat.force
    pmat.position += H * pmat.velocity
    pmat.force = np.zeros(2)

class PMat:

    def __init__(self, pos, vel, frc, m, algo):
        # Numpy vector
        self.position = pos
        # Numpy vector
        self.velocity = vel
        # Numpy vector
        self.force = frc
        # float
        self.mass = m
        # function of pmat algo
        self._algo = algo

    def distance(self, pmat):
        return np.linalg.norm(self.position - pmat.position)

    def algo(self):
        self._algo(self)

    @staticmethod
    def pmat_fix(pos):
        return PMat(pos, np.zeros(2), np.zeros(2), 1, fix)

    @staticmethod
    def pmat_movable(pos, vel, m):
        return PMat(pos, vel, np.zeros(2), m, movable)
