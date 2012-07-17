import libhello
import numpy as np
print "From libhello : ", libhello.Greet()

a = np.random.randn(3, 3)
#a = np.zeros((3, 3), dtype=np.float64)
libhello.PrintMat(a)
