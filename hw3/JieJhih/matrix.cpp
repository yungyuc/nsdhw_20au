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

inline size_t Boundary(size_t n, size_t root) {
  if(n>root)
    return root;
    return n;
}

Matrix multiply_tile(const Matrix &mt1, const Matrix &mt2, size_t t_size) {
 if (mt1.ncol() != mt2.nrow()) {
    throw std::out_of_range("The size of Matrix 1 row is not equal to the size of matrix 2 column");
  }

  Matrix mt3(mt1.nrow(), mt2.ncol());
        // moving block
  for (size_t i = 0; i < mt1.nrow(); i += t_size)
    for (size_t j = 0; j < mt2.ncol(); j += t_size)
      for (size_t k = 0; k < mt1.ncol(); k += t_size)
        // multiply naive
        for (size_t kk = k; kk < Boundary(k+t_size,mt1.ncol()); ++kk)
          for (size_t ii = i; ii < Boundary(i+t_size,mt1.nrow()); ++ii)
            for (size_t jj = j; jj < Boundary(j+t_size,mt2.ncol()); ++jj)
              mt3(ii, jj) += mt1(ii, kk) * mt2(kk, jj);
  return mt3;
}