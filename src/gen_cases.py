import numpy as np

# Variables para generar el caso
file = open("res/case3.in", "w")
N = 100
x_lim = 9/2
y_lim = x_lim/1.7

X = np.random.uniform(-x_lim, x_lim, N)
Y = np.random.uniform(-y_lim, y_lim, N)

file.write(f'{N}\n')
for x, y in zip(X, Y):
    file.write(f'{x} {y}\n')

file.close()
