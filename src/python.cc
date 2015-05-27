#include <boost/python.hpp>
#include <iostream>
#include <boost/eigen_numpy.h>

BOOST_PYTHON_MODULE(boost_numpy_eigen)
{
    SetupEigenConverters();
}
