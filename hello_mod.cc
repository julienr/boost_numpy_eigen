#include <boost/numpy.hpp>

#include "eigen_numpy.h"

#include "hello.h"

namespace bp = boost::python;
namespace np = boost::numpy;

BOOST_PYTHON_MODULE(libhello) {
  np::initialize();
  SetupEigenConverters();

  bp::def("Greet", Greet);
  bp::def("PrintMat", PrintMat);
  bp::def("GetMat", GetMat);
}
