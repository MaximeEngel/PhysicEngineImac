from tkinter import Canvas, ALL
import time
class Motor:

    def __init__(self):
        self.links = []
        self.pmats = []

    def simulate(self):
        for pmat in self.pmats:
            pmat.algo()
        for link in self.links:
            link.algo()

    def draw(self, canvas):
        canvas.delete(ALL)
        for pmat in self.pmats:
            pmat.draw(canvas)
        for link in self.links:
            link.draw(canvas)
