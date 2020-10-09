#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <cmath>

namespace py = pybind11;

int angle(std::vector<int> &a, std::vector<int> &b) {
    if ((a[0] == 0 && a[1] == 0) || (b[0] == 0 && b[1] == 0)){
        std::cout<<"Invalid input\n";
        return -1;
    }
    long long dot = a[0] * b[0] + a[1] * b[1];
    //long long length = sqrt(a[0]*a[0] + a[1]*a[1]) * sqrt(b[0]*b[0] + b[1]*b[1]);
    long long det = a[0] * b[1] - a[1] * b[0];

    //return double(acos(double(dot / length))) * 180/3.1415926;
    return atan2(det, dot) * 180/3.1415926;
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 plugin";
    m.def("angle", &angle, "caculate the angle between two radians.");
}