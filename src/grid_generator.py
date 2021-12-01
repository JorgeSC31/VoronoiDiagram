import numpy as np

n_width = 5
n_high = 4
x_lim = 3.5
y_lim = 2

file = open("res/case2.in", "w")
file.write(f'{n_width*n_high}\n')
X = np.linspace(-x_lim, x_lim, num = n_width)
Y = np.linspace(-y_lim, y_lim, num = n_high)

for y in Y:
    for x in X:
        file.write(f'{x} {y}\n')

file.close()