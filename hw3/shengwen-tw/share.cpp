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
	py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
	.def(py::init<int, int>())
	.def_property_readonly("nrow", &Matrix::row)
	.def_property_readonly("ncol", &Matrix::column)
	.def("__getitem__", [](const Matrix &m, std::pair<size_t, size_t> i) {
				return m(i.first, i.second);})
	.def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> i, double val) {
				return m(i.first, i.second) = val;})
	.def("__eq__", &Matrix::operator==)
	.def("__ne__", &Matrix::operator!=);

;
}
