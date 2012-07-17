#include <iostream>
#include <Eigen/Dense>

using Eigen::Matrix3d;
using namespace std;

void PrintMat(const Matrix3d& m) {
  cout << "Matrix : " << m << endl;
}

char const* Greet() {
  return "Hello, world ! ";
}

