#!/bin/bash

# -- ndarray
echo 'Building ndarray'
pushd libs/boost.numpy
scons --with-eigen-include=/usr/include/eigen3
scons install --prefix=../_install/
popd
