#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>

namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer.resize(nelement, 0);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
        : m_nrow(nrow), m_ncol(ncol) {
        m_buffer = vec;
    }

    // No bound check.
    double   operator() (size_t row, size_t col) const { return m_buffer[row*m_ncol + col]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[row*m_ncol + col]; }

    bool operator == (Matrix const &other) {
        if (this == &other) return true;
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
        if (m_buffer == other.m_buffer) return true;
        else return false;
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

private:

    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_buffer;

};

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2) {
    size_t nr = mat1.nrow();
    size_t nc = mat2.ncol();
    Matrix ans(nr, nc);
    for (size_t i = 0; i<nr; i++) {
        for (size_t k = 0; k < nc; k++) {
            double sum = 0;
            for (size_t j = 0; j < mat1.ncol(); j++) {
                sum += mat1(i, j) * mat2(j, k);
            }
            ans(i, k) = sum;
        }
    }
    return ans;
}

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, std::vector<double>&>())
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def("__eq__", &Matrix::operator==)
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i, double v) {
            mat(i.first, i.second) = v;
        })
        .def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> i) {
            return mat(i.first, i.second);
        });
}