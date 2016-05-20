cmake_minimum_required(VERSION 2.8.3)
project(boost.numpy_eigen)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
# commented so modules end up in top level dir where python files are
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(NumPy)
include_directories(${NUMPY_INCLUDE_DIRS})

# main library
add_library(boost_numpy_eigen SHARED src/eigen_numpy.cc)
target_link_libraries(boost_numpy_eigen ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

# unit test
add_library(test_eigen_numpy_mod SHARED EXCLUDE_FROM_ALL src/test_eigen_numpy_mod.cc)
target_link_libraries(test_eigen_numpy_mod boost_numpy_eigen)
set_target_properties(test_eigen_numpy_mod PROPERTIES PREFIX "")

# example
add_library(hello SHARED EXCLUDE_FROM_ALL src/hello.cc src/hello_mod.cc)
target_link_libraries(hello boost_numpy_eigen)

install(TARGETS boost_numpy_eigen
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES src/eigen_numpy.h DESTINATION include/boost/)
