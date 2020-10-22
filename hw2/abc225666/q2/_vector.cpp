#include <pybind11/pybind11.h>
#include <bits/stdc++.h>

using namespace std;

double angle(double a1, double a2, double b1, double b2) {
    // a dot b = |a||b|cos(theta)
    double dot = a1*b1 + a2*b2;
    double ab_multi = sqrt(((a1*a1)+(a2*a2))*((b1*b1)+(b2*b2)));
    if(ab_multi == 0) {
        return nan("");
    }
    double cos_theta = dot / ab_multi;
    return acos(cos_theta);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 calc angle";
    m.def("angle", &angle, "Description");
}
