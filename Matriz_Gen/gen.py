import numpy as np

matriz = np.random.randint(0, 1000, (1024, 1024))

np.savetxt("B.txt", matriz, fmt="%d")