#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <mkl.h>
#include <mkl_lapacke.h>
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "matrix.h"

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
	m.doc() = "ok";
	m.def("multiply_naive", &multiply_naive, "");
	m.def("multiply_tile", &multiply_tile, "");
	m.def("multiply_mkl", &multiply_mkl, "");

    m.def("bytes", &bytes, "");
    m.def("allocated", &allocated, "");
    m.def("deallocated", &deallocated, "");

	py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
	.def(py::init<int, int>())
	.def_property_readonly("nrow", &Matrix::row)
	.def_property_readonly("ncol", &Matrix::column)

    .def_buffer(
            [](Matrix &mat)->py::buffer_info {
                return py::buffer_info(mat.raw_data(), sizeof(double),
                                       py::format_descriptor<double>::format(), 2,
                                       {mat.row(), mat.column()},
                                       {sizeof(double) * mat.column(), sizeof(double)});
            }
    )

    .def_property(
            "array", 
            [](Matrix &mat) {
                return py::array_t<double>({mat.row(), mat.column()},
                                           {sizeof(double) * mat.column(), sizeof(double)},
                                           mat.raw_data(), py::cast(mat));
            },
            nullptr
    )

	.def("__getitem__", [](const Matrix &m, std::pair<size_t, size_t> i) {
				return m(i.first, i.second);})
	.def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> i, double val) {
				return m(i.first, i.second) = val;})
	.def("__eq__", [](Matrix &m1, Matrix &m2){return m1 == m2;})
	.def("__ne__", [](Matrix &m1, Matrix &m2){return m1 != m2;})
	;
}
