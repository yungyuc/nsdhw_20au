#include <vector>
#include <stdexcept>
#include "mkl.h"
#include "matrix.hpp"

// default contructor
Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0)
{
}

// copy constructor
Matrix::Matrix(Matrix const & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer()
{
    std::copy(other.m_buffer.begin(), other.m_buffer.end(), m_buffer.begin());
}

// move constructor
Matrix::Matrix(Matrix && other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer()
{
    other.m_buffer.swap(m_buffer);
}

Matrix::Matrix(std::vector<std::vector<double>> const & other)
    : m_nrow(other.size()), m_ncol(other[0].size())
{
    for(const auto &v: other)
        m_buffer.insert(m_buffer.end(), v.begin(), v.end());
}


Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.m_ncol != mat2.m_nrow)
        throw std::invalid_argument("Input matrix rank didn't match");

    Matrix ret(mat1.m_nrow, mat2.m_ncol);

    for (size_t i = 0; i < ret.m_nrow; ++i) {
        for (size_t k = 0; k < ret.m_ncol; ++k) {
            double v = 0;
            for (size_t j = 0; j < mat1.m_ncol; ++j) {
                v += mat1(i, j) * mat2(j, k);
            }
            ret(i, k) = v;
        }
    }

    return ret;
};

Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, const unsigned int tsize)
{
    if (mat1.m_ncol != mat2.m_nrow)
        throw std::invalid_argument("Input matrix rank didn't match");

    if ((tsize == 0) || (tsize > mat1.m_nrow) || (mat1.m_nrow < 8))
        return multiply_naive(mat1, mat2);

    Matrix ret(mat1.m_nrow, mat2.m_ncol);
    unsigned int remain = ret.m_nrow % 8;
    size_t i, i_max = ret.m_nrow - remain;

    for (i = 0; i < i_max; i += 8) {
        for (size_t k = 0; k < ret.m_ncol; ++k) {
            double v[8] = {0};
            for (size_t j = 0; j < mat1.m_ncol; ++j) {
                v[0] += mat1(i    , j) * mat2(j, k);
                v[1] += mat1(i + 1, j) * mat2(j, k);
                v[2] += mat1(i + 2, j) * mat2(j, k);
                v[3] += mat1(i + 3, j) * mat2(j, k);
                v[4] += mat1(i + 4, j) * mat2(j, k);
                v[5] += mat1(i + 5, j) * mat2(j, k);
                v[6] += mat1(i + 6, j) * mat2(j, k);
                v[7] += mat1(i + 7, j) * mat2(j, k);
            }
            ret(i     , k) = v[0];
            ret(i + 1 , k) = v[1];
            ret(i + 2 , k) = v[2];
            ret(i + 3 , k) = v[3];
            ret(i + 4 , k) = v[4];
            ret(i + 5 , k) = v[5];
            ret(i + 6 , k) = v[6];
            ret(i + 7 , k) = v[7];
        }
    }

    if (remain) {
        for (i = i_max; i < ret.m_nrow; ++i) {
            for (size_t k = 0; k < ret.m_ncol; ++k) {
                double v = 0;
                for (size_t j = 0; j < mat1.m_ncol; ++j) {
                    v += mat1(i, j) * mat2(j, k);
                }
                ret(i, k) = v;
            }
        }
    }

    return ret;
};

Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.m_ncol != mat2.m_nrow)
        throw std::invalid_argument("Input matrix rank didn't match");

    Matrix ret(mat1.m_nrow, mat2.m_ncol);

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.m_nrow, mat2.m_ncol, mat1.m_ncol, 1.0,
        mat1.m_buffer.data(), mat1.m_ncol,
        mat2.m_buffer.data(), mat2.m_ncol, 0.0,
        ret.m_buffer.data(), mat2.m_ncol
    );

    return ret;
};

