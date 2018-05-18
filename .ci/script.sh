#!/usr/bin/env bash

set -ex

mkdir build
cd build
cmake .. -DBOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION=$BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION
make -j4
sudo make install
