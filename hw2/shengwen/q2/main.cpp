#include <iostream>
#include <cmath>
#include <pybind11/pybind11.h>

using namespace std;

float angle(float x1, float y1, float x2, float y2)
{
	return acos((x1 * y1) + (x2 * y2));
}

PYBIND11_MODULE(_vector, m) {
	m.doc() = "pybind11 example code";
	m.def("angle", &angle, "calculate angle (radian) between two vectors");
}
