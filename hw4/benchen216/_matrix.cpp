//
// Created by ben on 2020/11/9.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;
#include <mkl.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>
struct ByteCounterImpl
{

    size_t allocated = 0;
    size_t deallocated = 0;
    size_t refcount = 0;

}; /* end struct ByteCounterImpl */

/**
 * One instance of this counter is shared among a set of allocators.
 *
 * The counter keeps track of the bytes allocated and deallocated, and report
 * those two numbers in addition to bytes that remain allocated.
 */
class ByteCounter
{

public:

    ByteCounter()
      : m_impl(new ByteCounterImpl)
    { incref(); }

    ByteCounter(ByteCounter const & other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter const & other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ByteCounter(ByteCounter && other)
      : m_impl(other.m_impl)
    { incref(); }

    ByteCounter & operator=(ByteCounter && other)
    {
        if (&other != this)
        {
            decref();
            m_impl = other.m_impl;
            incref();
        }

        return *this;
    }

    ~ByteCounter() { decref(); }

    void swap(ByteCounter & other)
    {
        std::swap(m_impl, other.m_impl);
    }

    void increase(std::size_t amount)
    {
        m_impl->allocated += amount;
    }

    void decrease(std::size_t amount)
    {
        m_impl->deallocated += amount;
    }

    std::size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
    std::size_t allocated() const { return m_impl->allocated; }
    std::size_t deallocated() const { return m_impl->deallocated; }
    /* This is for debugging. */
    std::size_t refcount() const { return m_impl->refcount; }

private:

    void incref() { ++m_impl->refcount; }

    void decref()
    {
        if (nullptr == m_impl)
        {
            // Do nothing.
        }
        else if (1 == m_impl->refcount)
        {
            delete m_impl;
            m_impl = nullptr;
        }
        else
        {
            --m_impl->refcount;
        }
    }

    ByteCounterImpl * m_impl;

}; /* end class ByteCounter */

/**
 * Very simple allocator that counts the number of bytes allocated through it.
 *
 * It's made to demonstrate the STL allocator and only works in this example.
 * A lot of modification is needed to use it in a real application.
 */
template <class T>
struct MyAllocator
{

    using value_type = T;

    // Just use the default constructor of ByteCounter for the data member
    // "counter".
    MyAllocator() = default;

    template <class U> constexpr
    MyAllocator(const MyAllocator<U> & other) noexcept : counter(other.counter) {}

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            counter.increase(bytes);
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;

}; /* end struct MyAllocator */

static MyAllocator<double> alloc;
struct Matrix {
    public:
    double * mydata() { return m_buffer.data(); }
   const double * mydata() const { return m_buffer.data(); }
    ~Matrix() { reset_buffer(0, 0); }

Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(alloc)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
            : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc) {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix(Matrix const &other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol) , m_buffer(alloc){
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i, j) = other(i, j);
            }
        }
    }

    Matrix &operator=(std::vector<double> const &vec) {
        if (size() != vec.size()) {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i, j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix &operator=(Matrix const &other) {
        if (this == &other) {
            return *this;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            reset_buffer(other.m_nrow, other.m_ncol);
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i, j) = other(i, j);
            }
        }
        return *this;
    }

    Matrix &operator=(Matrix &&other) {
        if (this == &other) {
            return *this;
        }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    Matrix &operator=(double val) {
        for (size_t i = 0, end = m_nrow * m_ncol; i < end; ++i) {
            m_buffer[i] = val;
        }
        return *this;
    }

    Matrix &operator+=(const Matrix &other) {
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            throw std::out_of_range("the shape of first matrix differs from"
                                    "that of second matrix");
        }

        for (size_t i = 0, end = m_nrow * m_ncol; i < end; ++i) {
            m_buffer[i] += other.m_buffer[i];
        }
        return *this;
    }

    double operator()(size_t row, size_t col) const {
        return m_buffer[index(row, col)];
    }
    double &operator()(size_t row, size_t col) {
        return m_buffer[index(row, col)];
    }

    double operator[](size_t idx) const { return m_buffer[idx]; }
    double &operator[](size_t idx) { return m_buffer[idx]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    //std::vector<double> buffer_vector() const {
    //    return std::vector<double>(m_buffer, m_buffer + size());
    //}

    Matrix transpose() const;

    size_t index(size_t row, size_t col) const { return row * m_ncol + col; }

    void save(Matrix &mat, size_t it, size_t jt);
    void zero() {
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i, j) = 0;
            }
        }
    }
    void reset_buffer(size_t nrow, size_t ncol) {
        const size_t nelement = nrow * ncol;
        m_buffer.reserve(nelement);
         m_buffer.clear();
        m_nrow = nrow;
        m_ncol = ncol;
    }


    //friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    //friend Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, const size_t tile_size);
    //friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
    //friend bool operator==(Matrix const &mat1, Matrix const &mat2);

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, MyAllocator<double>> m_buffer;
};

Matrix Matrix::transpose() const {
    Matrix ret(nrow(), ncol());

    for (size_t i = 0; i < ret.nrow(); ++i) {
        for (size_t j = 0; j < ret.ncol(); ++j) {
            ret(j, i) = (*this)(i, j);
        }
    }

    return ret;
}

bool operator==(Matrix const &mat1, Matrix const &mat2) {
    if ((mat1.ncol() != mat2.ncol()) && (mat1.nrow() != mat2.ncol())) {
        return false;
    }

    for (size_t i = 0; i < mat1.nrow(); ++i) {
        for (size_t j = 0; j < mat1.ncol(); ++j) {
            if (mat1(i, j) != mat2(i, j)) {
                return false;
            }
        }
    }

    return true;
}

bool operator!=(Matrix const &mat1, Matrix const &mat2) {
    return !(mat1 == mat2);
}

/*
 * Throw an exception if the shapes of the two matrices don't support
 * multiplication.
 */
void validate_multiplication(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
                "the number of first matrix column "
                "differs from that of second matrix row");
    }
}

/*
 * Get the number of floating-point operations.
 */
size_t calc_nflo(Matrix const & mat1, Matrix const & mat2)
{
    return mat1.nrow() * mat1.ncol() * mat2.ncol();
}


Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2){
    validate_multiplication(mat1,mat2);
    Matrix ret(mat1.nrow(), mat2.ncol());
    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();


    for (size_t i=0; i<nrow1; ++i)
    {
        const size_t base1 = i * ncol1;
        for (size_t k=0; k<ncol2; ++k)
        {
            double v = 0;
            for (size_t j=0; j<ncol1; ++j)
            {
                v += mat1.m_buffer[base1 + j] * mat2.m_buffer[j*ncol2 + k];
            }
            ret.m_buffer[base1 + k] = v;
        }
    }
    return ret;

}


Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2,size_t lsize){
    validate_multiplication(mat1,mat2);
    Matrix ret(mat1.nrow(), mat2.ncol());
    ret.zero();
    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();
    for (size_t i = 0; i < nrow1; i += lsize){
        for (size_t j = 0; j < ncol2; j += lsize){
            for (size_t k = 0; k < nrow1; k += lsize){
                // multiply naive
                for (size_t nk = k; nk < (lsize+k>ncol1?ncol1:lsize+k); ++nk){
                    for (size_t ni = i; ni <(lsize+i>nrow1?nrow1:lsize+i); ++ni){
                        for (size_t nj = j; nj < (lsize+j>ncol2?ncol2:lsize+j); ++nj){
                            ret(ni, nj) += mat1(ni, nk) * mat2(nk, nj);
                        }
                    }
                }
            }
        }
    }
    return ret;
}
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2){
    mkl_set_num_threads(1);
    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
            CblasRowMajor /* const CBLAS_LAYOUT Layout */
            , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
            , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
            , mat1.nrow() /* const MKL_INT m */
            , mat2.ncol() /* const MKL_INT n */
            , mat1.ncol() /* const MKL_INT k */
            , 1.0 /* const double alpha */
            , mat1.mydata() /* const double *a */
            , mat1.ncol() /* const MKL_INT lda */
            , mat2.mydata() /* const double *b */
            , mat2.ncol() /* const MKL_INT ldb */
            , 0.0 /* const double beta */
            , ret.mydata() /* double * c */
            , ret.ncol() /* const MKL_INT ldc */
    );


    return ret;
}




size_t bytes() { return alloc.counter.bytes(); }
size_t allocated() { return alloc.counter.allocated(); }
size_t deallocated() { return alloc.counter.deallocated(); }

PYBIND11_MODULE(_matrix, m) {
	m.doc() = "pybind11 example plugin";
	m.def("multiply_naive", & multiply_naive);
	m.def("multiply_tile", & multiply_tile);
	m.def("multiply_mkl", & multiply_mkl);
	m.def("bytes", & bytes);
	m.def("allocated", & allocated);
	m.def("deallocated", & deallocated);
	py::class_<Matrix>(m, "Matrix")
	.def(py::init<size_t, size_t>())
	.def("size", &Matrix::size)
	.def_property_readonly("nrow", &Matrix::nrow)
	.def_property_readonly("ncol", &Matrix::ncol)
	.def("__eq__", &operator==)
	.def("__getitem__", [](const Matrix & mat, std::pair<size_t, size_t> t)
	{
		return mat(t.first, t.second);
	})
	.def("__setitem__", [](Matrix & mat, std::pair<size_t, size_t> t, float v)
	{
		mat(t.first, t.second) = v;
	})
	;

}

/*
PYBIND11_MODULE(_matrix, m)
{
m.doc() = "pybind11 matrix multiplication test";
m.def("multiply_naive", & multiply_naive, "naive method");
m.def("multiply_tile", & multiply_tile, "Tiling solution");
m.def("multiply_mkl", & multiply_mkl, "use mkl");
py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
.def(py::init<size_t, size_t>())
.def(py::init<size_t, size_t, const std::vector<double> &>())
.def(py::init<const Matrix &>())
.def_property("nrow", &Matrix::nrow, nullptr)
.def_property("ncol", &Matrix::ncol, nullptr)
.def("__eq__", &operator==)
.def("buffer_vector", &Matrix::buffer_vector)
.def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i,
double val) { mat(i.first, i.second) = val; })
.def("__getitem__", [](Matrix &mat, std::pair<size_t, size_t> i) {
return mat(i.first, i.second);
});
}*/