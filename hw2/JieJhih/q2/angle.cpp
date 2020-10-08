#include <math.h>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

double calculate(const std::vector<int> v1, const std::vector<int> v2) {
    if ((v1.size()!=2 || v2.size()!=2) || ((v1.at(0) == 0 && v1.at(1) == 0) || (v2.at(0) == 0 && v2.at(1) == 0))) {
        return -1;
    }
    double x1 = v1.at(0);
    double y1 = v1.at(1);
    double x2 = v2.at(0);
    double y2 = v2.at(1);

    double t = x1*x2+y1*y2;
    double l1 = sqrt(x1*x1+y1*y1);
    double l2 = sqrt(x2*x2+y2*y2);

    return acos(t/(l1*l2));
}

PYBIND11_MODULE(_vector, m) {
  m.def("calculate", &calculate,
        "Calculate the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system");
}