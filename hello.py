import libhello
import numpy as np
print("From libhello : ", libhello.Greet())

print(libhello.GetMat())

a = np.random.randn(3, 3)
print('----Print Matrix from python')
print(a)
#a = np.zeros((3, 3), dtype=np.float64)
print('----Print Matrix from C++')
libhello.PrintMat(a)

print('----Get Matrix from C++')
m = libhello.GetMat()
print(m)
