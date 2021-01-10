#include <mkl.h>
#include "matrix.hpp"

bool Matrix::operator==(const Matrix &other) const {
  if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
    return false;
  }
  for (size_t i = 0; i < m_nrow; ++i) {
    for (size_t j = 0; j < m_ncol; ++j) {
      if ((*this)(i, j) != other(i, j)) {
        return false;
      }
    }
  }
  return true;
}

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
