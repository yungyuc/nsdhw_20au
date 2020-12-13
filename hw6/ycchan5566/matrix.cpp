#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <iostream>
#include <mkl.h>
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

    py::array_t<double> array() {
		return py::array_t<double>(
                {nrow(), ncol()}, // shape
                {sizeof(double) * ncol(), sizeof(double)}, // C-style contiguous strides for ncol doubles
                m_buffer.data(), // the data pointer
                py::cast(this) // let handle object reference *this*
                );
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }


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

Matrix multiply_tile(Matrix &mat1, Matrix &mat2, int tile_size) {
  Matrix mat3(mat1.nrow(), mat2.ncol());

  for (int i = 0; i < mat1.nrow(); i += tile_size) {
    for (int j = 0; j < mat2.ncol(); j += tile_size) {

      int start = mat1.nrow() < i + tile_size ? mat1.nrow() : i + tile_size;
      int end = mat2.ncol() < j + tile_size ? mat2.ncol() : j + tile_size;

      for (int k = 0; k < mat1.ncol(); k += tile_size) {
        int ret_edge = mat1.ncol() < k + tile_size ? mat1.ncol() : k + tile_size;
        for (int kk = k; kk < ret_edge; kk++) {
          for (int ii = i; ii < start; ii++) {
            for (int jj = j; jj < end; jj++) {
              mat3(ii, jj) += mat1(ii, kk) * mat2(kk, jj);
            }
          }
        }
      }
    }
  }
  return mat3;
}

Matrix * multiply_mkl(const Matrix & mat1, const Matrix & mat2)
{
	mkl_set_num_threads(1);

	Matrix * ret = new Matrix(mat1.nrow(), mat2.ncol());

	cblas_dgemm(
		CblasRowMajor,
		CblasNoTrans,
		CblasNoTrans,
		mat1.nrow(),
		mat2.ncol(),
		mat1.ncol(),
		1.0,
		mat1.m_buffer.data(),
		mat1.ncol(),
		mat2.m_buffer.data(),
		mat2.ncol(),
		0.0,
		(*ret).m_buffer.data(),
		(*ret).ncol()
		);
	return ret;
}

PYBIND11_MODULE(_matrix, m) {
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, std::vector<double>&>())
        .def_property("nrow", &Matrix::nrow, nullptr)
        .def_property("ncol", &Matrix::ncol, nullptr)
        .def_property("array", &Matrix::array, nullptr)
        .def("__eq__", &Matrix::operator==)
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i, double v) {
            mat(i.first, i.second) = v;
        })
        .def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> i) {
            return mat(i.first, i.second);
        });
}