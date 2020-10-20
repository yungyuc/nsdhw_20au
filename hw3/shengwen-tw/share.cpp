#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <mkl.h>
#include <mkl_lapacke.h>
#include <pybind11/pybind11.h>
#include "matrix.h"

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
	m.doc() = "ok";
	m.def("multiply_naive", &multiply_naive, "uber eat");
	m.def("multiply_tile", &multiply_naive, "food panda");
	m.def("multiply_mkl", &multiply_naive, "im just random typing");
	py::class_<Matrix>(m, "Matrix", py::buffer_protocol()).def(py::init<int, int>());
}
