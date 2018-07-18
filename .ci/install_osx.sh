#!/usr/bin/env bash

set -x
# e option is disabled for now

brew update > /dev/null

/usr/bin/yes | pip2 uninstall numpy
brew upgrade python
brew install numpy

brew install boost-python3
brew install eigen
