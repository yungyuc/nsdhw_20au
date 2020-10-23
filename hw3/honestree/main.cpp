#include <mkl.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <functional>

#include "StopWatch.hpp"

struct Matrix
{
public:
    Matrix(size_t row, size_t col) : nrow(row), ncol(col)
    {
        if (col * row == 0)
        {
            throw std::out_of_range("number of elements mismatch");
        }
        data = new double[row * col];
    }

    void operator=(Matrix &other)
    {
        if (data)
        {
            delete[] data;
            data = nullptr;
        }
        nrow = other.nrow;
        ncol = other.ncol;
        data = new double[nrow * ncol];
        for (int i = 0; i < nrow * ncol; ++i)
        {
            data[i] = other.data[i];
        }
    }

    ~Matrix()
    {
        if (data)
        {
            delete[] data;
            data = nullptr;
        }
    }
    size_t nrow = 0;
    size_t ncol = 0;
    double *data;
    double operator()(size_t row, size_t col) const { return data[row * ncol + col]; }
    double &operator()(size_t row, size_t col) { return data[row * ncol + col]; }
};

bool operator==(Matrix const &mat1, Matrix const &mat2)
{

    if ((mat1.ncol != mat2.ncol) && (mat1.nrow != mat2.nrow))
    {
        return false;
    }
    for (size_t i = 0; i < mat1.nrow; ++i)
    {
        for (size_t j = 0; j < mat1.ncol; ++j)
        {
            if (mat1(i, j) != mat2(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(Matrix const &mat1, Matrix const &mat2)
{
    return !(mat1 == mat2);
}

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{

    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow, mat2.ncol);

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transa */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transb */
        ,
        mat1.nrow /* const MKL_INT m */
        ,
        mat2.ncol /* const MKL_INT n */
        ,
        mat1.ncol /* const MKL_INT k */
        ,
        1.0 /* const double alpha */
        ,
        mat1.data /* const double *a */
        ,
        mat1.ncol /* const MKL_INT lda */
        ,
        mat2.data /* const double *b */
        ,
        mat2.ncol /* const MKL_INT ldb */
        ,
        0.0 /* const double beta */
        ,
        ret.data /* double * c */
        ,
        ret.ncol /* const MKL_INT ldc */
    );

    return ret;
}

/*
 * Direct naive matrix matrix multiplication.
 */
Matrix multiply_direct(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol != mat2.nrow)
    {
        throw std::out_of_range("number of elements mismatch");
    }

    Matrix ret(mat1.nrow, mat2.ncol);

    for (size_t ret_row = 0; ret_row < mat1.nrow; ++ret_row)
    {
        for (size_t ret_col = 0; ret_col < mat2.ncol; ++ret_col)
        {
            ret(ret_row, ret_col) = 0;
            for (size_t index = 0; index < mat1.ncol; index++)
            {
                ret(ret_row, ret_col) += mat1(ret_row, index) * mat2(index, ret_col);
            }
        }
    }

    return ret;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol != mat2.nrow)
    {
        throw std::out_of_range("number of elements mismatch");
    }

    Matrix ret(mat1.nrow, mat2.ncol);
    Matrix Tmat2(mat2.ncol, mat2.nrow);

    for (size_t Tmat2_row = 0; Tmat2_row < Tmat2.nrow; ++Tmat2_row)
    {
        for (size_t Tmat2_col = 0; Tmat2_col < Tmat2.ncol; ++Tmat2_col)
        {
            Tmat2(Tmat2_row, Tmat2_col) = mat2(Tmat2_col, Tmat2_row);
        }
    }

    for (size_t ret_col = 0; ret_col < mat2.ncol; ++ret_col)
    {
        for (size_t ret_row = 0; ret_row < mat1.nrow; ++ret_row)
        {
            ret(ret_row, ret_col) = 0;
            for (size_t index = 0; index < mat1.ncol; index++)
            {
                ret(ret_row, ret_col) += mat1(ret_row, index) * Tmat2(ret_col, index);
            }
        }
    }

    return ret;
}

int main(int argc, char **argv)
{
    double elapsed;
    Matrix mat1(2048, 2048);
    Matrix mat2(2048, 2048);
    for (int i = 0; i < 2048 * 2048; i++)
    {
        mat2.data[i] = i;
        mat1.data[i] = 2048 * 2048 - 1 - i;
    }

    StopWatch sw;

    std::cout << "multiply_direct" << std::endl;
    sw.lap();
    Matrix ret = multiply_direct(mat1, mat2);
    elapsed = sw.lap();
    std::cout << "Sequential takes: " << elapsed << " sec" << std::endl;

    std::cout << "multiply_tile" << std::endl;
    sw.lap();
    Matrix ret2 = multiply_tile(mat1, mat2);
    elapsed = sw.lap();
    std::cout << "Sequential takes: " << elapsed << " sec" << std::endl;

    std::cout << "multiply_mkl" << std::endl;
    Matrix ret3 = multiply_mkl(mat1, mat2);
    elapsed = sw.lap();
    std::cout << "Sequential takes: " << elapsed << " sec" << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
