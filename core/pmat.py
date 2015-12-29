import numpy as np
from .constants import H
from tkinter import Canvas

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

        self.radius = 5
        self.color = "red"

    def distance(self, pmat):
        return np.linalg.norm(self.position - pmat.position)

    def algo(self):
        self._algo(self)

    def draw(self, canvas):
        pos1 = self.position - self.radius
        pos2 = self.position + self.radius
        canvas.create_oval(pos1[0], pos1[1], pos2[0], pos2[1], fill=self.color, outline="")

    @staticmethod
    def pmat_fix(pos):
        return PMat(pos, np.zeros(2), np.zeros(2), 1, fix)

    @staticmethod
    def pmat_movable(pos, vel, m):
        return PMat(pos, vel, np.zeros(2), m, movable)
