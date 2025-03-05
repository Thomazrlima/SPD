import numpy as np

matriz = np.random.randint(0, 1000, (512, 512))

np.savetxt("B.txt", matriz, fmt="%d")