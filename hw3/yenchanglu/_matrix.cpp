#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include <vector>
#include <stdexcept>
#include <mkl.h>
#include <algorithm>

namespace py = pybind11;

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol) {
        reset_buffer(nrow, ncol);
    }

    Matrix(Matrix const &other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    Matrix(std::vector<std::vector<double>> const &v) {
        m_nrow = v.size();
        m_ncol = v[0].size();
        reset_buffer(m_nrow, m_ncol);
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i,j) = v[i][j];
            }
        }
    }

    ~Matrix() {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    Matrix &operator=(Matrix const &other) {
        if (this == &other) {
            return *this;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    bool operator==(Matrix const &other) {
        if (this == &other) {
            return true;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            return false;
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                if ((*this)(i,j) != other(i,j)) {
                    return false;
                }
            }
        }
        return true;
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t size() const { return m_nrow * m_ncol; }

    double buffer(size_t i) const { return m_buffer[i]; }
    double *data() const { return m_buffer; }

    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer + size()); }

private:
    size_t index(size_t row, size_t col) const {
        return row + col * m_nrow;
    }
    void reset_buffer(size_t nrow, size_t ncol) {
        if (m_buffer) {
            delete[] m_buffer;
        }
        const size_t nelement = nrow * ncol;
        if (nelement) {
            m_buffer = new double[nelement];
        } else {
            m_buffer = nullptr;
        }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double *m_buffer = nullptr;
};

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2) {
    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < ret.nrow(); ++i) {
        for (size_t k = 0; k < ret.ncol(); ++k) {
            double v = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j) {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tile_size) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("Incorrect dimensions for matrix multiplication");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); ++i) {
        for (size_t j = 0; j < mat2.ncol(); ++j) {
            ret(i,j) = 0;
        }
    }

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t ncol2 = mat2.ncol();

    double v;

    for (size_t i = 0; i < nrow1; i += tile_size) {
        size_t i_min = std::min(i + tile_size, nrow1);
        for (size_t k = 0; k < ncol2; k += tile_size) {
            size_t k_min = std::min(k + tile_size, ncol2);
            for (size_t j = 0; j < ncol1; j += tile_size) {
                size_t j_min = std::min(j + tile_size, ncol1);
                for (size_t tile_i = i; tile_i < i_min; ++tile_i) {
                    for (size_t tile_k = k; tile_k < k_min; ++tile_k) {
                        v = 0.0;
                        for (size_t tile_j = j; tile_j < j_min; ++tile_j) {
                            v += mat1(tile_i, tile_j) * mat2(tile_j, tile_k);
                        }
                        ret(tile_i,tile_k) += v;
                    }
                }
            }
        }
    }

    return ret;
}


Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2) {
    Matrix ret = Matrix(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1.nrow(), mat2.ncol(), mat1.ncol(), 1, mat1.data(), mat1.ncol(), mat2.data(), mat2.ncol(), 0, ret.data(), mat2.ncol());

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "matrix mutiplication module";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())        
        .def(py::init<size_t, size_t>())
        .def(py::init<Matrix const &>())
        .def(py::init<std::vector<std::vector<double>>&>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__eq__", &Matrix::operator==)
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> i){
            return mat(i.first, i.second);
        })
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i, double val){
            mat(i.first, i.second) = val;
        });
}
