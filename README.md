This is a simple example on how to use boost.python to call
c++ code from python and convert numpy arrays to Eigen.

To run (assuming you've boost.python already installed):

```
cmake .
make hello
python hello.py
make test_eigen_numpy_mod
python test_eigen_numpy.py
```

Links
-----

http://eigen.tuxfamily.org/index.php?title=Main_Page

There also is the 'ndarray' project, that aims at providing a multidimensionnal
array library similar to numpy.ndarray for C++. Some of the code comes from
ndarray :
https://github.com/ndarray/ndarray

TODO
----
- The from python converts do some data copy. This could hurt for big matrices
