#ifndef _BACKWARD_COMPATIBILITY_H_
#define _BACKWARD_COMPATIBILITY_H_

#include <boost/version.hpp>

#define BOOST_MAJOR_VERSION (BOOST_VERSION % 100)
#define BOOST_MINOR_VERSION (BOOST_VERSION / 100 % 1000)
#define BOOST_PATCH_VERSION (BOOST_VERSION / 100000)

#define BOOST_VERSION_AT_LEAST(x,y,z) \
  (BOOST_MAJOR_VERSION > x || (BOOST_MAJOR_VERSION >= x && \
  (BOOST_MINOR_VERSION > y || (BOOST_MINOR_VERSION >= y && \
  BOOST_PATCH_VERSION >= z))))

#if BOOST_VERSION_AT_LEAST(1,63,0)

#include <boost/python/numpy.hpp>
namespace np = boost::python::numpy;

#else

#include <boost/numpy.hpp>
namespace np = boost::numpy;

#endif // if BOOST_VERSION_AT_LEAST(1,63,0)

#endif // ifndef _BACKWARD_COMPATIBILITY_H_
