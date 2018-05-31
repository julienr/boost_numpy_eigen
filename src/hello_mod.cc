#include <boost/python.hpp>

#include "eigen_numpy.h"

#include "hello.h"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(libhello) {
  SetupEigenConverters();

  bp::def("Greet", Greet);
  bp::def("PrintMat", PrintMat);
  bp::def("GetMat", GetMat);
}
