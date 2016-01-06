from core.motor import Motor
from core.pmat import PMat
from core.link import Link
from core.gui import Gui
from core.constants import Fe
import numpy as np


# Frequence : Fe = 1 / h
# Masse : m = 1
# raideur : k = 1 * Fe^2
# viscosite : z = 0.1 * Fe

def modelate(motor, nb_pmat, width):
    k = 0.8 * Fe ** 2
    y = 200.0
    start_x = 50.0
    step_x = (width - 10) / nb_pmat
    motor.pmats.append(PMat.pmat_fix(np.array([start_x, y])))

    for i in range(1, nb_pmat - 1):
        motor.pmats.append(PMat.pmat_movable(np.array([start_x + i * step_x, y]), np.zeros(2), 1.0))

    start_x += (nb_pmat - 1) * step_x
    motor.pmats.append(PMat.pmat_fix(np.array([start_x, y])))

    for i in range(nb_pmat - 1):
        link = Link(k, 0.005 * Fe)
        link.connect(motor.pmats[i], motor.pmats[i + 1])
        motor.links.append(link)


def __main__():
    width = 1000
    motor = Motor()
    nb = 30
    modelate(motor, nb, width)
    gui = Gui(motor, width, 800)
    def anim(event):	 
	# motor.pmats[int(nb / 2)].position -= np.array([15, 25])
	motor.pmats[int(nb / 2)].force += np.array([5000, 2000])
    gui.lunchAnim.bind("<Button-1>", anim)
    gui.mainloop()

if __name__ == "__main__":
    __main__()
