import numpy as np

N = 10  # Numero de puntos en la circunferencia
pi = np.pi
r = 2  # radio de la circunferencia

file = open("res/case4.in", "w")
theta = np.linspace(0, 2*pi, num=N)
file.write(f'{N + 1}\n')

for t in theta:
    file.write(f'{r*np.cos(t)} {r*np.sin(t)}\n')

file.write(f'{0} {0}\n')  # Agregar punto central
file.close()
