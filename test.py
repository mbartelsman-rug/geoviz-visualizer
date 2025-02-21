from matplotlib import pyplot as plt

import numpy as np

from math import floor
import math

x = list(range(5000))

lines = np.empty((len(x), 1), dtype=np.uint8)

bin_size = 100
alpha = 1.1
thickness = 0.01


for i, length in enumerate(x):

    d = length / len(x)

    s = 40
    e = 0.6
    h = 0


    phi = (floor((s * d) ** e + h) - h) ** (1/e) / s


    print(d - phi)

    if d - phi < thickness:
        lines[i] = 255
    else:
        lines[i] = 0
    continue

    d = max(10, int(length / bin_size) * bin_size * alpha)

    if length < thickness or length - floor(length / d) * d < thickness * alpha:
        lines[i] = 255
    else:
        lines[i] = 0

print(lines)

my_cmap = plt.get_cmap("viridis")
plt.bar(np.arange(len(x)), 0.5, width=1, color=my_cmap(lines))

plt.show()
