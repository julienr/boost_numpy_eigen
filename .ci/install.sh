#!/usr/bin/env bash

set -ex

sudo apt-get -qq update
sudo apt-get install -y \
  libboost-python-dev \
  libeigen3-dev \
  libpython3-dev \
  python-numpy \
  python3-numpy
