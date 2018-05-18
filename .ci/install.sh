#!/usr/bin/env bash

set -ex

sudo apt-add-repository -y ppa:personalrobotics/ppa
sudo apt-get -qq update
sudo apt-get install -y \
  libeigen3-dev \
  libboost-python-dev \
  libboost-python-numpy-dev \
  python3-numpy \
  libpython3-dev \
