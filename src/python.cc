#include <boost/python.hpp>
#include <iostream>
#include "eigen_numpy.h"

BOOST_PYTHON_MODULE(boost_numpy_eigen)
{
    SetupEigenConverters();
}
