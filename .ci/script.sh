#!/usr/bin/env bash

set -ex

mkdir build
cd build
cmake .. -DBOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION=$BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION
make -j4 all hello test_eigen_numpy_mod
sudo make install

cd ..
python$BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION hello.py
python$BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION test_eigen_numpy.py
