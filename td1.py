from core.motor import Motor
from core.pmat import PMat
from core.link import Link
from core.gui import Gui
import numpy as np

def modelate(motor, nb_pmat):
    k = 1000.0
    y = 200.0
    start_x = 50.0
    step_x = 20
    motor.pmats.append(PMat.pmat_fix(np.array([start_x, y])))

    for i in range(1, nb_pmat - 1):
        motor.pmats.append(PMat.pmat_movable(np.array([start_x + i * step_x, y]), np.zeros(2), 1.0))

    start_x += (nb_pmat - 1) * step_x
    motor.pmats.append(PMat.pmat_fix(np.array([start_x, y])))

    for i in range(nb_pmat - 1):
        link = Link(k)
        link.connect(motor.pmats[i], motor.pmats[i + 1])
        motor.links.append(link)


def __main__():
    motor = Motor()
    nb = 30
    modelate(motor, nb)
    motor.pmats[int(nb / 2)].position -= np.array([0, 20])
    gui = Gui(motor, 800, 400)
    gui.mainloop()

if __name__ == "__main__":
    __main__()
