import numpy as np

N = 70 #Numero de puntos en la circunferencia
pi = np.pi
r = 2 #radio de la circunferencia 

file = open("res/case4.in", "w")
theta = np.linspace(0, 2*pi, num = N)
file.write(f'{N}\n')

theta = theta[:-1] #Quitar theta = 2*pi para no repetir con theta = 0

for t in theta:
    file.write(f'{r*np.cos(t)} {r*np.sin(t)}\n')

file.write(f'{0} {0}\n') #Agregar punto central
file.close()
