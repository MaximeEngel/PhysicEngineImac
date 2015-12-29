from tkinter import Tk, Canvas

class Gui:
    time_before_update = int(1000 / 60.0)

    def __init__(self, motor, width=400, height=400):
        self.motor = motor
        self.root = Tk()
        self.root.resizable(width=False, height=False)
        self.canvas = Canvas(self.root, width=width, height=height)
        self.canvas.pack()
        # self.root.update()

    def _update_draw(self):
        self.motor.simulate()
        self.motor.draw(self.canvas)
        self.canvas.after(Gui.time_before_update, self._update_draw)

    def mainloop(self):
        self.motor.draw(self.canvas)
        self.canvas.after(Gui.time_before_update, self._update_draw)
        self.root.mainloop()