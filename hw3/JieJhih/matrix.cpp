#include "matrix.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <algorithm>

#include "mkl.h"

namespace py = pybind11;


bool Matrix::operator==(const Matrix &other) {
  if (m_row != other.m_row || m_col != other.m_col) {
    return false;
  }
  for (size_t i = 0; i < m_row*m_col; ++i) {
      if ((*this)(i/m_row, i%m_col) != other(i/m_row, i%m_col)) {
        return false;
      }
  }
  return true;
}

Matrix multiply_naive(const Matrix &mt1, const Matrix &mt2) {
  if (mt1.ncol() != mt2.nrow()) {
    throw std::out_of_range("The size of Matrix 1 row is not equal to the size of matrix 2 column");
  }

  size_t row = mt1.nrow();
  size_t col = mt2.ncol();
  Matrix mt3(row, col);
  
  for (size_t i = 0; i < row; ++i) {
    for (size_t j = 0; j < col; ++j) {
      for (size_t k = 0; k < mt1.ncol(); ++k) {
        mt3(i, j) += mt1(i, k) * mt2(k, j);
      }
    }
  }
  return mt3;
}