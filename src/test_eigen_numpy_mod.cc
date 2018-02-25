// Tests from ndarray lib
#include <boost/python.hpp>
#include <Eigen/Eigen>
#include "eigen_numpy.h"

#include <iostream>

namespace bp = boost::python;

template <typename M>
bool acceptMatrix(M m) {
    return (m(0,0) == 1) && (m(0,1) == 2) && (m(0,2) == 3) 
        && (m(1,0) == 4) && (m(1,1) == 5) && (m(1,2) == 6);
}

template <typename M>
bool acceptVector(M m) {
    return (m[0] == 1) && (m[1] == 2) && (m[2] == 3) && (m[3] == 4);
}

template <typename M>
void fillMatrix(M & m) {
    m(0,0) = 1;
    m(0,1) = 2;
    m(0,2) = 3;
    m(1,0) = 4;
    m(1,1) = 5;
    m(1,2) = 6;
}

template <typename M>
M returnMatrix() {
    static typename boost::remove_const<typename boost::remove_reference<M>::type>::type m(2,3);
    fillMatrix(m);
    return m;
}

template <typename M>
void fillVector(M & m) {
    m[0] = 1;
    m[1] = 2;
    m[2] = 3;
    m[3] = 4;
}

template <typename M>
M returnVector() {
    static typename boost::remove_const<typename boost::remove_reference<M>::type>::type m(4);
    fillVector(m);
    return m;
}

template <typename M>
bp::object returnObject() {
    static typename boost::remove_const<typename boost::remove_reference<M>::type>::type m(2,3);
    fillMatrix(m);
    bp::object o(m);
    return o;
}

static const int X = Eigen::Dynamic;

BOOST_PYTHON_MODULE(test_eigen_numpy_mod) {
  SetupEigenConverters();
  bp::def("acceptMatrix_23d_cref", acceptMatrix< Eigen::Matrix<double,2,3> const &>);
  bp::def("acceptMatrix_X3d_cref", acceptMatrix< Eigen::Matrix<double,X,3> const &>);
  bp::def("acceptMatrix_2Xd_cref", acceptMatrix< Eigen::Matrix<double,2,X> const &>);
  bp::def("acceptMatrix_XXd_cref", acceptMatrix< Eigen::Matrix<double,X,X> const &>);
  bp::def("acceptVector_41d_cref", acceptVector< Eigen::Matrix<double,4,1> const &>);
  bp::def("acceptVector_X1d_cref", acceptVector< Eigen::Matrix<double,X,1> const &>);
  bp::def("acceptVector_14d_cref", acceptVector< Eigen::Matrix<double,1,4> const &>);
  bp::def("acceptVector_1Xd_cref", acceptVector< Eigen::Matrix<double,1,X> const &>);
  bp::def("returnVector_41d", returnVector< Eigen::Matrix<double,4,1> >);
  bp::def("returnVector_14d", returnVector< Eigen::Matrix<double,1,4> >);
  bp::def("returnVector_X1d", returnVector< Eigen::Matrix<double,X,1> >);
  bp::def("returnVector_1Xd", returnVector< Eigen::Matrix<double,1,X> >);
  bp::def("returnMatrix_23d", returnMatrix< Eigen::Matrix<double,2,3> >);
  bp::def("returnMatrix_X3d", returnMatrix< Eigen::Matrix<double,X,3> >);
  bp::def("returnMatrix_2Xd", returnMatrix< Eigen::Matrix<double,2,X> >);
  bp::def("returnMatrix_XXd", returnMatrix< Eigen::Matrix<double,X,X> >);
  bp::def("returnMatrix_23d_c", returnMatrix< Eigen::Matrix<double,2,3> const>);
  bp::def("returnMatrix_X3d_c", returnMatrix< Eigen::Matrix<double,X,3> const>);
  bp::def("returnMatrix_2Xd_c", returnMatrix< Eigen::Matrix<double,2,X> const>);
  bp::def("returnMatrix_XXd_c", returnMatrix< Eigen::Matrix<double,X,X> const>);
  bp::def("returnObject_23d", returnObject< Eigen::Matrix<double,2,3> >);
  bp::def("returnObject_X3d", returnObject< Eigen::Matrix<double,X,3> >);
  bp::def("returnObject_2Xd", returnObject< Eigen::Matrix<double,2,X> >);
  bp::def("returnObject_XXd", returnObject< Eigen::Matrix<double,X,X> >);
}
