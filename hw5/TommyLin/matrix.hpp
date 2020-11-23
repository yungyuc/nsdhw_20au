#pragma once

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol);
    Matrix(Matrix const & other);
    Matrix(Matrix && other);
    Matrix(std::vector<std::vector<double>> const & other);

    ~Matrix() = default; // default destructor

    friend Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2);
    friend Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, const unsigned int tsize);
    friend Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2);

    size_t index(size_t row, size_t col) const { return row*m_ncol + col; }
    double   operator() (size_t row, size_t col) const { return m_buffer.at( index(row, col) ); }
    double & operator() (size_t row, size_t col)       { return m_buffer.at( index(row, col) ); }

    bool operator == (Matrix const &other) {
        if (this == &other) return true;
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) return false;
        if (m_buffer == other.m_buffer) return true;
        else return false;
    }

    double *data() { return m_buffer.data(); }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

private:
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> m_buffer;
};

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2);
Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, const unsigned int tsize);
Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2);

