This has been archived. New home is at

https://github.com/personalrobotics/boost_numpy_eigen


This is a simple example on how to use boost.python and boost.numpy to call
c++ code form python and convert numpy arrays to Eigen.

To run (assuming you've boost.python already installed)::

  git submodule init
  git submodule update
  ./build.sh      # will build boost.numpy
  make
  ./run.sh hello.py
  make test
  ./run.sh test_eigen_numpy.py


Links :

https://github.com/ndarray/Boost.NumPy

http://eigen.tuxfamily.org/index.php?title=Main_Page

There also is the 'ndarray' project, that aims at providing a multidimensionnal
array library similar to numpy.ndarray for C++. Some of the code comes from
ndarray :
https://github.com/ndarray/ndarray

TODO:
- The from python converts do some data copy. This could hurt for big
  matrices
