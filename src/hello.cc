#include <iostream>
#include <Eigen/Dense>

using Eigen::Matrix3f;
using Eigen::Matrix3d;
using namespace std;

void PrintMat(const Matrix3d& m) {
  cout << "Matrix : " << m << endl;
}

Eigen::Matrix3f GetMat() {
  Matrix3f m;
  m << 1, 2, 3,
       4, 5, 6,
       7, 8, 9;
  return m;
}

char const* Greet() {
  return "Hello, world ! ";
}

