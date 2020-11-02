#include <mkl.h>
#include "StopWatch.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <functional>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <tuple>
namespace py = pybind11;

struct Matrix
{
public:
    Matrix(size_t row, size_t col) : nrow(row), ncol(col)
    {
        if (col * row == 0)
        {
            throw std::out_of_range("number of elements mismatch");
        }
        // init to 0
        data = new double[row * col]();
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
        for (size_t i = 0; i < nrow * ncol; ++i)
        {
            data[i] = other.data[i];
        }
    }

    size_t m_row() const { return nrow; }
    size_t m_col() const { return ncol; }
    size_t nrow = 0;
    size_t ncol = 0;
    double *data = nullptr;
    double operator()(size_t row, size_t col) const { return data[row * ncol + col]; }
    double &operator()(size_t row, size_t col) { return data[row * ncol + col]; }
};

/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{

    if (mat1.ncol != mat2.nrow)
        throw std::invalid_argument("Input matrix rank didn't match");

    Matrix ret(mat1.nrow, mat2.ncol);

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transa */
        ,
        CblasNoTrans /* const CBLAS_TRANSPOSE transb */
        ,
        mat1.m_row() /* const MKL_INT m */
        ,
        mat2.m_col() /* const MKL_INT n */
        ,
        mat1.m_col() /* const MKL_INT k */
        ,
        1.0 /* const double alpha */
        ,
        mat1.data /* const double *a */
        ,
        mat1.m_col() /* const MKL_INT lda */
        ,
        mat2.data /* const double *b */
        ,
        mat2.m_col() /* const MKL_INT ldb */
        ,
        0.0 /* const double beta */
        ,
        ret.data /* double * c */
        ,
        ret.m_col() /* const MKL_INT ldc */
    );

    return ret;
}

bool operator==(Matrix const &mat1, Matrix const &mat2)
{
    if ((mat1.ncol != mat2.ncol) && (mat1.nrow != mat2.ncol))
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
            for (size_t index = 0; index < mat1.ncol; index++)
            {
                ret(ret_row, ret_col) += mat1(ret_row, index) * mat2(index, ret_col);
            }
        }
    }

    return ret;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tsize)
{
    if (mat1.ncol != mat2.nrow)
    {
        throw std::out_of_range("number of elements mismatch");
    }

    Matrix ret(mat1.nrow, mat2.ncol);
    Matrix Tmat2(mat2.ncol, mat2.nrow);

    // perform mat2 transpose
    for (size_t Tmat2_row = 0; Tmat2_row < Tmat2.nrow; ++Tmat2_row)
    {
        for (size_t Tmat2_col = 0; Tmat2_col < Tmat2.ncol; ++Tmat2_col)
        {
            Tmat2(Tmat2_row, Tmat2_col) = mat2(Tmat2_col, Tmat2_row);
        }
    }

    // choose tile to start
    for (size_t tile_row = 0; tile_row * tsize < mat1.nrow; ++tile_row)
    {
        for (size_t tile_col = 0; tile_col * tsize < mat2.ncol; ++tile_col)
        {
            for (size_t tile_mul_index = 0; tile_mul_index * tsize < mat1.ncol; tile_mul_index++)
            {
                // start calculate the return tile
                // tile(tile_row, tile_col) = mat1(tile_row, tile_mul_index) * Tmat2(tile_col, tile_mul_index)
                for (size_t in_tile_mul_index = tile_mul_index * tsize; in_tile_mul_index < std::min((tile_mul_index + 1) * tsize, mat1.ncol); ++in_tile_mul_index)
                {
                    for (size_t in_tile_row = tile_row * tsize; in_tile_row < std::min((tile_row + 1) * tsize, mat1.nrow); ++in_tile_row)
                    {
                        for (size_t in_tile_col = tile_col * tsize; in_tile_col < std::min((tile_col + 1) * tsize, mat2.ncol); ++in_tile_col)
                        {
                            ret(in_tile_row, in_tile_col) += mat1(in_tile_row, in_tile_mul_index) * Tmat2(in_tile_col, in_tile_mul_index);
                        }
                    }
                }
            }
        }
    }

    return ret;
}

PYBIND11_MODULE(_matrix, m)
{
    m.def("multiply_naive", &multiply_direct);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    py::class_<Matrix>(m, "Matrix")
        .def_property_readonly("nrow", &Matrix::m_row)
        .def_property_readonly("ncol", &Matrix::m_col)
        .def(py::init<const size_t, const size_t>())
        .def("__getitem__", [](const Matrix &mat, std::tuple<size_t, size_t> t) -> double {
            return mat(std::get<0>(t), std::get<1>(t));
        })
        .def("__setitem__", [](Matrix &mat, std::tuple<size_t, size_t> t, const double &v) {
            mat(std::get<0>(t), std::get<1>(t)) = v;
        })
        .def("__eq__", &operator==);
}
