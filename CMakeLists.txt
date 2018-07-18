cmake_minimum_required(VERSION 2.8.3)
project(BoostNumpyEigen)

# Put our local cmake find scripts at the beginning of the CMake
# module search path.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(NOT BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION)
  set(BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION 3.4 CACHE STRING "Choose the target Python version (e.g., 3.4, 2.7)" FORCE)
endif()

find_package(PythonInterp ${BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION} REQUIRED)
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c
  "from distutils.sysconfig import get_python_lib;\
  print(get_python_lib(plat_specific=True, prefix=''))"
  OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(MODULE_INSTALL_PREFIX "${MODULE_INSTALL_PREFIX}" CACHE STRING
  "Output directory for Python modules.")
message(STATUS "Installing Python module to: ${MODULE_INSTALL_PREFIX}")

# Find required python packages.
find_package(PythonLibs ${BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION} REQUIRED)

# Find boost with python components. The name of python component varies
# depending on the platform, boost version, and python version.
if(APPLE)
  find_package(Boost REQUIRED
    COMPONENTS
      python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}
  )
else() # LINUX assumed
  if(${PYTHON_VERSION_MAJOR} EQUAL 3)
    find_package(Boost
      COMPONENTS
        python-py${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}
    )
    if(NOT Boost_FOUND)
      find_package(Boost REQUIRED COMPONENTS python3)
    endif()
  else() # Python 2 assumed
    find_package(Boost REQUIRED COMPONENTS python)
  endif()
endif()

find_package(Eigen3 REQUIRED)
find_package(NumPy ${BOOST_PYTHON_NUMPY_EIGEN_PYTHON_VERSION} REQUIRED)

# Global settings for include paths.
include_directories(
  ${PYTHON_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
  ${NUMPY_INCLUDE_DIRS}
)

# main library
add_library(boost_numpy_eigen SHARED
  src/eigen_numpy.cc
  src/python.cc
)
target_link_libraries(boost_numpy_eigen
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES}
)
set_target_properties(boost_numpy_eigen PROPERTIES
  PREFIX ""
  SUFFIX ".so"  # python uses '.so' extension even on macOS
)
install(TARGETS boost_numpy_eigen
  LIBRARY DESTINATION "${PYTHON_SITE_PACKAGES}"
)

# unit test
add_library(test_eigen_numpy_mod SHARED EXCLUDE_FROM_ALL src/test_eigen_numpy_mod.cc)
target_link_libraries(test_eigen_numpy_mod boost_numpy_eigen)
set_target_properties(test_eigen_numpy_mod PROPERTIES
  SUFFIX ".so"  # python uses '.so' extension even on macOS
)
set_target_properties(test_eigen_numpy_mod PROPERTIES PREFIX "")
add_custom_command(TARGET test_eigen_numpy_mod
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy test_eigen_numpy_mod*.so ${CMAKE_SOURCE_DIR}
)

# example
add_library(hello SHARED EXCLUDE_FROM_ALL src/hello.cc src/hello_mod.cc)
target_link_libraries(hello boost_numpy_eigen)
set_target_properties(hello PROPERTIES
  SUFFIX ".so"  # python uses '.so' extension even on macOS
)
add_custom_command(TARGET hello
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy libhello*.so ${CMAKE_SOURCE_DIR}
)
