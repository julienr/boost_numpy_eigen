cmake_minimum_required(VERSION 2.8.3)
project(BoostNumpyEigen)

# Put our local cmake find scripts at the beginning of the CMake
# module search path.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Configure output folders so artifacts are built into a single set of
# top-level folders rather than the default cmake build structure that
# matches the source tree.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    CACHE PATH "Output directory for static libraries.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    CACHE PATH "Output directory for shared libraries.")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    CACHE PATH "Output directory for executables and DLL's.")

# Find required python packages.
find_package(PythonLibs 2 REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(NumPy)
find_package(BoostNumpy)

# Global settings for include paths.
include_directories(
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    ${NUMPY_INCLUDE_DIRS}
)

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

# Create CMake package configuration.
set(INCLUDE_INSTALL_DIR include/ CACHE PATH
    "The subdirectory relative to the install prefix where header will be installed (default prefix/include)")
set(LIB_INSTALL_DIR lib/ CACHE PATH
    "The subdirectory relative to the install prefix where libraries will be installed (default is prefix/lib)")
set(SYSCONFIG_INSTALL_DIR etc/ CACHE PATH
    "The subdirectory relative to the install prefix where sysconfig will be installed (default prefix/etc)")

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/BoostNumpyEigenConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/BoostNumpyEigenConfig.cmake
    INSTALL_DESTINATION ${LIB_INSTALL_DIR}/BoostNumpyEigen/cmake
    PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/BoostNumpyEigenConfigVersion.cmake
    VERSION 0.1.0
    COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/BoostNumpyEigenConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/BoostNumpyEigenConfigVersion.cmake
        DESTINATION ${LIB_INSTALL_DIR}/BoostNumpyEigen/cmake)

# Building a package with CPack.
set(CPACK_PACKAGE_NAME boost-numpy-eigen)
set(CPACK_PACKAGE_VERSION 1)
set(CPACK_PACKAGE_CONTACT "Michael Koval <mkoval@cs.cmu.edu>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "python-dev, libboost-python-dev, boost-numpy, libeigen3-dev")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Automatic conversions between NumPy and Eigen using Boost.NumPy.")
include(CPack)
