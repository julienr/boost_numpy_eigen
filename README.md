# Boost.NumPy_Eigen [![Build Status](https://travis-ci.org/personalrobotics/Boost.NumPy_Eigen.svg?branch=master)](https://travis-ci.org/personalrobotics/Boost.NumPy_Eigen)

This is a simple example on how to use boost.python to call
c++ code from python and convert numpy arrays to [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page).

To run (assuming you've boost.python already installed):

```console
$ cmake .
$ make hello
$ python hello.py
$ make test_eigen_numpy_mod
$ python test_eigen_numpy.py
```

## Links

There also is the [`ndarray`](https://github.com/ndarray/ndarray) project, that aims at providing a multidimensionnal
array library similar to numpy.ndarray for C++. Some of the code comes from
`ndarray`.

## TODO

- The from python converts do some data copy. This could hurt for big matrices
