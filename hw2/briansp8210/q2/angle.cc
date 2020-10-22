#include <cmath>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

double cal_angle(const std::vector<int> v1, const std::vector<int> v2) {
  if (v1.size() != 2 || v2.size() != 2) {
    return -1;
  }
  if ((v1.at(0) == 0 && v1.at(1) == 0) || (v2.at(0) == 0 && v2.at(1) == 0)) {
    return -1;
  }

  double dot = v1.at(0) * v2.at(0) + v1.at(1) * v2.at(1);
  double len1 = sqrt(pow(v1.at(0), 2) + pow(v1.at(1), 2));
  double len2 = sqrt(pow(v2.at(0), 2) + pow(v2.at(1), 2));
  double ang = acos(dot / (len1 * len2));

  return ang;
}

PYBIND11_MODULE(_vector, m) {
  m.doc() = "nsd 2020 hw2";
  m.def("cal_angle", &cal_angle,
        "Calculate angle (in radians) of 2 two-dimensional vectors");
}
