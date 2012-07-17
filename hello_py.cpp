#include <boost/numpy.hpp>
#include "hello.h"
#include <iostream>
#include <glog/logging.h>

namespace bp = boost::python;
namespace np = boost::numpy;

template<typename T, int R, int C>
Eigen::Matrix<T,R,C> NPToEigen(const np::ndarray& a) {
  const np::dtype edt = np::dtype::get_builtin<T>();
  const np::dtype dtype = a.get_dtype();
  CHECK(edt == dtype);
  CHECK_EQ(2, a.get_nd());
  CHECK_EQ(R, a.shape(0));
  CHECK_EQ(C, a.shape(1));
  auto flags = a.get_flags();
  CHECK(flags & np::ndarray::C_CONTIGUOUS);
  CHECK(flags & np::ndarray::ALIGNED);

  LOG(INFO) << "a.dtype : " << bp::extract<char const*>(bp::str(a.get_dtype()));
  LOG(INFO) << "strides 0 : " << a.strides(0);
  LOG(INFO) << "strides 1 : " << a.strides(1);
  LOG(INFO) << "a : " << bp::extract<char const*>(bp::str(a));

  int dtype_size = dtype.get_itemsize();
  int s1 = a.strides(0);
  int s2 = a.strides(1);
  CHECK_EQ(0, s1 % dtype_size);
  CHECK_EQ(0, s2 % dtype_size);

  T* raw_data = reinterpret_cast<T*>(a.get_data());
  using namespace Eigen;
  auto eigena = Map<Matrix<T, R, C, RowMajor>, Aligned, Stride<Dynamic, Dynamic>>(
    raw_data, Stride<Dynamic, Dynamic>(s1/dtype_size, s2/dtype_size));
  std::cout << eigena << std::endl;

  return eigena;
}

void _PrintMat(const np::ndarray& a) {
  PrintMat(NPToEigen<double, 3, 3>(a));
}

static const int X = Eigen::Dynamic;

BOOST_PYTHON_MODULE(libhello) {
  np::initialize();
  bp::def("Greet", Greet);
  bp::def("PrintMat", _PrintMat);
}
