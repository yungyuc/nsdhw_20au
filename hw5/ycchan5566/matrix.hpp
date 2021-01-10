#ifndef MATRIX
#define MATRIX

#include <vector>

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

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }


    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_buffer;

};

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2);

Matrix multiply_tile(Matrix &mat1, Matrix &mat2, int tile_size);

Matrix * multiply_mkl(const Matrix & mat1, const Matrix & mat2);

#endif
