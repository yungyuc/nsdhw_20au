#ifndef MATRIX_H_
#define MATRIX_H_

#include <cstddef>
#include <vector>
#include <cstdint>
#include <limits>
#include <cstdlib>

static uint64_t bytes_allocated;
static uint64_t bytes_deallocated;

uint64_t bytes() { return bytes_allocated - bytes_deallocated; }
uint64_t allocated() { return bytes_allocated; }
uint64_t deallocated() { return bytes_deallocated; }

template <class T>
class CustomAllocator {
public:
  using value_type = T;

  CustomAllocator() = default;

  template <class U>
  CustomAllocator(const CustomAllocator<U> &other) noexcept {}

  T *allocate(std::size_t n) {
    const std::size_t bytes = n * sizeof(T);
    T *p = static_cast<T *>(std::malloc(bytes));
    if (p) {
      bytes_allocated += bytes;
      return p;
    } else {
      throw std::bad_alloc();
    }
  }

  void deallocate(T *p, std::size_t n) noexcept {
    std::free(p);
    const std::size_t bytes = n * sizeof(T);
    bytes_deallocated += bytes;
  }
};

class Matrix {
 public:
  Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0.0) {}
  Matrix(const std::vector<std::vector<double>> &m);

  bool operator==(const Matrix &other);
  double operator()(size_t row, size_t col) const {
    return m_buffer[row * m_ncol + col];
  }
  double &operator()(size_t row, size_t col) {
    return m_buffer[row * m_ncol + col];
  }

  size_t nrow() const { return m_nrow; }
  size_t ncol() const { return m_ncol; }
  double *buffer() { return m_buffer.data(); }
  const double *buffer() const { return m_buffer.data(); }

 private:
  size_t m_nrow;
  size_t m_ncol;
  std::vector<double, CustomAllocator<double>> m_buffer;
};

Matrix multiply_naive(const Matrix &m1, const Matrix &m2);
Matrix multiply_tile(const Matrix &m1, const Matrix &m2, size_t tsize);
Matrix multiply_mkl(const Matrix &m1, const Matrix &m2);

#endif  // MATRIX_H_
