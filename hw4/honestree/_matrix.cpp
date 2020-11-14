#include <mkl.h>
#include "StopWatch.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <functional>

#include <atomic>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <tuple>
namespace py = pybind11;




struct ByteCounterImpl {
    std::atomic_size_t allocated = 0;
    std::atomic_size_t deallocated = 0;
    std::atomic_size_t refcount = 0;
};

class ByteCounter {
public:
    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }

    ByteCounter(ByteCounter const & other)
    : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter const & other) {
        if (&other != this) {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other) {
        if (&other != this) {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other) {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount) {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount) {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }

    std::size_t refcount() const { return m_impl->refcount; }

private:
    void incref() { ++m_impl->refcount; }
    void decref() {
        if (nullptr == m_impl) {
            // Do nothing
        } else if (1 == m_impl->refcount) {
            delete m_impl;
            m_impl = nullptr;
        } else {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl * m_impl;
};

template <class T>
struct MyAllocator {
    using value_type = T;

    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U> & other) noexcept {
        counter = other.counter;
    }

    T * allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n * sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p) {
            counter.increase(bytes);
            return p;
        } else {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::free(p);

        const std::size_t bytes = n * sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;
};

static MyAllocator<double> alloc;

struct Matrix
{
public:
    Matrix(size_t row, size_t col) : nrow(row), ncol(col), data( alloc )
    {
        if (col * row == 0)
        {
            throw std::out_of_range("number of elements mismatch");
        }
        // init to 0
        data.resize( nrow * ncol , 0 );
    }

    void operator=(Matrix &other)
    {
        nrow = other.nrow;
        ncol = other.ncol;
        data.assign( other.data.begin(), other.data.end() );
    }

    Matrix(Matrix && other) : nrow(other.nrow), ncol(other.ncol), data(alloc) {
		data.clear();
        std::swap(data, other.data);
    }

    ~Matrix(){}

    size_t m_row() const { return nrow; }
    size_t m_col() const { return ncol; }
    size_t nrow = 0;
    size_t ncol = 0;
    std::vector <double,MyAllocator<double>> data;
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
        mat1.data.data() /* const double *a */
        ,
        mat1.m_col() /* const MKL_INT lda */
        ,
        mat2.data.data() /* const double *b */
        ,
        mat2.m_col() /* const MKL_INT ldb */
        ,
        0.0 /* const double beta */
        ,
        ret.data.data() /* double * c */
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


size_t bytes() { return alloc.counter.bytes(); }
size_t allocated() { return alloc.counter.allocated(); }
size_t deallocated() { return alloc.counter.deallocated(); }


PYBIND11_MODULE(_matrix, m)
{
    m.def("multiply_naive", &multiply_direct);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
	m.def("allocated", &allocated);
	m.def("deallocated", &deallocated);
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
