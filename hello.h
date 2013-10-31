#ifndef _HELLO_H_
#define _HELLO_H_

#include <Eigen/Dense>

void PrintMat(const Eigen::Matrix3d& m);
Eigen::Matrix3f GetMat();
char const* Greet();

#endif
