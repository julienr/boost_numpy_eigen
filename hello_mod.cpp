#include <boost/numpy.hpp>
#include "hello.h"
#include <iostream>
#include <glog/logging.h>

#include <numpy/arrayobject.h>

namespace bp = boost::python;
namespace np = boost::numpy;

template <typename SCALAR>
struct NumpyEquivalentType {};

template <> struct NumpyEquivalentType<double> {enum { type_code = NPY_DOUBLE };};
template <> struct NumpyEquivalentType<int> {enum { type_code = NPY_INT };};
template <> struct NumpyEquivalentType<float> {enum { type_code = NPY_FLOAT };};
template <> struct NumpyEquivalentType<std::complex<double> > {enum { type_code = NPY_CDOUBLE };};

template <typename SourceType, typename DestType >
static void copy_array(const SourceType* source, DestType* dest,
                       const npy_int &nb_rows, const npy_int &nb_cols,
    const bool &isSourceTypeNumpy = false, const bool &isDestRowMajor = true,
    const bool& isSourceRowMajor = true,
    const npy_int &numpy_row_stride = 1, const npy_int &numpy_col_stride = 1)
{
  // determine source strides
  int row_stride = 1, col_stride = 1;
  if (isSourceTypeNumpy) {
    row_stride = numpy_row_stride;
    col_stride = numpy_col_stride;
  } else {
    if (isSourceRowMajor) {
    //if (traits::Flags & Eigen::RowMajorBit) {
      row_stride = nb_cols;
    } else {
      col_stride = nb_rows;
    }
  }

  if (isDestRowMajor) {
    for (int r=0; r<nb_rows; r++) {
      for (int c=0; c<nb_cols; c++) {
        *dest = source[r*row_stride + c*col_stride];
        dest++;
      }
    }
  } else {
    for (int c=0; c<nb_cols; c++) {
      for (int r=0; r<nb_rows; r++) {
        *dest = source[r*row_stride + c*col_stride];
        dest++;
      }
    }
  }
}


template<class MatType> // MatrixXf or MatrixXd
struct EigenMatrixToPython {
  static PyObject* convert(const MatType& mat) {
    npy_intp shape[2] = { mat.rows(), mat.cols() };
    PyArrayObject* python_array = (PyArrayObject*)PyArray_SimpleNew(
        2, shape, NumpyEquivalentType<typename MatType::Scalar>::type_code);

    copy_array(mat.data(),
               (typename MatType::Scalar*)PyArray_DATA(python_array),
               mat.rows(),
               mat.cols(),
               false,
               true,
               MatType::Flags & Eigen::RowMajorBit);
    //Py_INCREF(python_array);
    return (PyObject*)python_array;
  }
};

/*template<class MatType>
void np2eigen(const PyObject* o, MatType* m) {
  import_array();

  const np::dtype edt = np::dtype::get_builtin<T>();
  const np::dtype dtype = a.get_dtype();
  CHECK(edt == dtype);
  CHECK_EQ(2, a.get_nd());
  if (MatType::SizeAtCompileTime == Eigen::Dynamic) {
    m->resize(a.shape(0), a.shape(1));
  } else {
    CHECK_EQ(m->rows(), a.shape(0));
    CHECK_EQ(m->cols(), a.shape(1));
  }
  auto flags = a.get_flags();
  CHECK(flags & np::ndarray::C_CONTIGUOUS);
  CHECK(flags & np::ndarray::ALIGNED);

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
}*/

//template<typename T, int R, int C>
template<typename MatType>
struct EigenMatrixFromPython {
  //typedef Eigen::Matrix<T, R, C> MatType;
  //typedef int MatType::RowsAtCompileTime R;
  //typedef int MatType::ColsAtCompileTime C;
  typedef typename MatType::Scalar T;

  EigenMatrixFromPython() {
    bp::converter::registry::push_back(&convertible,
                                       &construct,
                                       bp::type_id<MatType>());
  }

  static void* convertible(PyObject* obj_ptr) {
    if (!PyArray_Check(obj_ptr)) {
      throw "PyArray_Check failed";
    }
    if (PyArray_NDIM(obj_ptr)!=2) {
      throw "dim != 2";
    }
    if (PyArray_ObjectType(obj_ptr, 0) != NumpyEquivalentType<typename MatType::Scalar>::type_code) {
      throw "types not compatible";
    }
    int elsize = (PyArray_DESCR(obj_ptr))->elsize;
    if (PyArray_STRIDE(obj_ptr, 0) % elsize != 0 || PyArray_STRIDE(obj_ptr, 1) % elsize != 0) {
      throw "strides and type size not matched";
    }
    int flags = PyArray_FLAGS(obj_ptr);
    if (!(flags & NPY_ARRAY_C_CONTIGUOUS)) {
      throw "Contiguous C array required";
    }
    if (!(flags & NPY_ARRAY_ALIGNED)) {
      throw "Aligned array required";
    }
    return obj_ptr;
  }

  /*static void construct(PyObject* obj_ptr,
                        bp::converter::rvalue_from_python_stage1_data* data) {
    using bp::extract;
    void* storage=((bp::converter::rvalue_from_python_storage<T>*)
                   (data))->storage.bytes;
    new (storage) MatType;
    MatType* kp = (MatType*)storage;

    pyopencv_to(obj_ptr, *kp);
    data->convertible = storage;
  }*/

  static void construct(PyObject* obj_ptr,
                        bp::converter::rvalue_from_python_stage1_data* data) {
    const int R = MatType::RowsAtCompileTime;
    const int C = MatType::ColsAtCompileTime;

    using bp::extract;
    void* storage=((bp::converter::rvalue_from_python_storage<T>*)
                   (data))->storage.bytes;

    PyArrayObject *array = reinterpret_cast<PyArrayObject*>(obj_ptr);
    if (R != Eigen::Dynamic) {
      CHECK_EQ(R, array->dimensions[0]);
    }
    if (C != Eigen::Dynamic) {
      CHECK_EQ(C, array->dimensions[1]);
    }

    int dtype_size = (PyArray_DESCR(obj_ptr))->elsize;
    int s1 = PyArray_STRIDE(obj_ptr, 0);
    int s2 = PyArray_STRIDE(obj_ptr, 1);
    CHECK_EQ(0, s1 % dtype_size);
    CHECK_EQ(0, s2 % dtype_size);

    T* raw_data = reinterpret_cast<T*>(PyArray_DATA(array));
    using namespace Eigen;

    typedef Map<Matrix<T, R, C, RowMajor>,
                Aligned,
                Stride<Dynamic, Dynamic>> MapType;
    new (storage) MapType(raw_data,
        Stride<Dynamic, Dynamic>(s1/dtype_size, s2/dtype_size));
    MapType* emap = (MapType*)storage;
    data->convertible = storage;
    LOG(INFO) << emap;
  }

  /*static void construct(PyObject* obj_ptr,
                        bp::converter::rvalue_from_python_stage1_data* data) {
    import_array();
    using bp::extract;
    void* storage=((bp::converter::rvalue_from_python_storage<T>*)
                   (data))->storage.bytes;

    PyArrayObject *array = reinterpret_cast<PyArrayObject*>(obj_ptr);
    const np::dtype edt = np::dtype::get_builtin<T>();
    const np::dtype dtype = a.get_dtype();
    CHECK(edt == dtype);
    CHECK_EQ(2, a.get_nd());
    if (R != Eigen::Dynamic) {
      CHECK_EQ(R, a.shape(0));
    }
    if (C != Eigen::Dynamic) {
      CHECK_EQ(T, a.shape(1));
    }

    auto flags = a.get_flags();
    CHECK(flags & np::ndarray::C_CONTIGUOUS);
    CHECK(flags & np::ndarray::ALIGNED);

    int dtype_size = dtype.get_itemsize();
    int s1 = a.strides(0);
    int s2 = a.strides(1);
    CHECK_EQ(0, s1 % dtype_size);
    CHECK_EQ(0, s2 % dtype_size);

    T* raw_data = reinterpret_cast<T*>(a.get_data());
    using namespace Eigen;

    typedef Map<Matrix<T, R, C, RowMajor>,
                Aligned,
                Stride<Dynamic, Dynamic>> MapType;
    new (storage) MapType(raw_data,
        Stride<Dynamic, Dynamic>(s1/dtype_size, s2/dtype_size));
    MapType* emap = (MapType*)storage;
    data->convertible = storage;
    LOG(INFO) << emap;
  }*/

};

/*template<typename T, int R, int C>
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
}*/

//static const int X = Eigen::Dynamic;

#define EIGEN_MATRIX_CONVERTER(Type) \
  EigenMatrixFromPython<Eigen::Type>();  \
  bp::to_python_converter<Eigen::Type, EigenMatrixToPython<Eigen::Type> >();

void SetupEigenConverters() {
  import_array();

  EIGEN_MATRIX_CONVERTER(Matrix3f);
  EIGEN_MATRIX_CONVERTER(Matrix3d);
}

BOOST_PYTHON_MODULE(libhello) {
  np::initialize();
  SetupEigenConverters();

  bp::def("Greet", Greet);
  bp::def("PrintMat", PrintMat);
  bp::def("GetMat", GetMat);
}
