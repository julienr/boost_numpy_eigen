cmake_minimum_required(VERSION 2.8.3)
project(boost.numpy_eigen)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(PythonLibs 2 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(NumPy)
include_directories(${NUMPY_INCLUDE_DIRS})

include_directories(${BoostNumPy_INCLUDE_DIR})
link_directories(${BoostNumPy_LIBRARY_DIR})

add_library(boost_numpy_eigen SHARED src/eigen_numpy.cc)
target_link_libraries(boost_numpy_eigen ${Boost_LIBRARIES} ${BoostNumPy_LIBRARIES} ${PYTHON_LIBRARIES})

add_library(boost_numpy_eigen_python SHARED
  src/python.cc
)
target_link_libraries(boost_numpy_eigen_python
  boost_numpy_eigen
  ${Boost_LIBRARIES}
)
set_target_properties(boost_numpy_eigen_python PROPERTIES
  OUTPUT_NAME boost_numpy_eigen
  PREFIX ""
  SUFFIX ".so"
)

install(TARGETS boost_numpy_eigen
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES src/eigen_numpy.h DESTINATION include/boost/)

# Building a package with CPack.
set(CPACK_PACKAGE_NAME boost-numpy-eigen)
set(CPACK_PACKAGE_VERSION 1)
set(CPACK_PACKAGE_CONTACT "Michael Koval <mkoval@cs.cmu.edu>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "python-dev, libboost-python-dev, boost-numpy, libeigen3-dev")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Automatic conversions between NumPy and Eigen using Boost.NumPy.")
include(CPack)
