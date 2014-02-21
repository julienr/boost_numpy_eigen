cmake_minimum_required(VERSION 2.8.3)
project(boost.numpy_eigen)

set(BoostNumPy_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../externals/include)
set(BoostNumPy_LIBRARIES ${PROJECT_SOURCE_DIR}/../externals/lib/libboost_numpy.dylib)

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

include_directories(${PROJECT_SOURCE_DIR}/../externals/include)
include_directories(${BoostNumPy_INCLUDE_DIR})

add_library(boost_numpy_eigen SHARED src/eigen_numpy.cc)
target_link_libraries(boost_numpy_eigen ${Boost_LIBRARIES} ${BoostNumPy_LIBRARIES} ${PYTHON_LIBRARIES})
