#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <mkl.h>
#include <vector>

namespace py = pybind11;

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

class Matrix {

public:

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

    ~Matrix() {}

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }

    size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        const size_t nelement = nrow * ncol;
        m_buffer.reserve(nelement);
        m_buffer.clear();
        m_nrow = nrow;
        m_ncol = ncol;
    }
    
    double * all_data() {return m_buffer.data();}

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, MyAllocator<double>> m_buffer;

};

bool operator== (const Matrix & mat1, const Matrix & mat2)
    {
    	if(mat1.nrow() != mat2.nrow() || mat1.ncol() != mat2.ncol()) return false;
    	
    	for(size_t i = 0; i<mat1.nrow(); i++)
    	{
    		for(size_t j = 0; j<mat1.ncol(); j++)
    		{
    			if(mat1(i,j) != mat2(i,j)) return false;
    		}
    	}
    	return true;
    }

/*
 * Naive matrix matrix multiplication.
 */
Matrix multiply_naive(const Matrix & mat1, const Matrix & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                ret(i,k) += mat1(i,j) * mat2(j,k);
            }
        }
    }

    return ret;
}

/*
 * Tiled matrix matrix multiplication.
 */
Matrix multiply_tile(const Matrix & mat1, const Matrix & mat2, size_t tile_width)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());
    double v;
     
     for (size_t a = 0; a < mat1.nrow(); a += tile_width) {
     	 size_t a_min = std::min(a + tile_width, mat1.nrow());
         for (size_t b = 0; b < mat2.ncol(); b += tile_width) {
             size_t b_min = std::min(b + tile_width, mat2.ncol());
             for (size_t c = 0; c < mat1.ncol(); c += tile_width) {
                 size_t c_min = std::min(c + tile_width, mat1.ncol());
                 for (size_t i = a; i < a_min; i++) {
                     for (size_t j = b; j < b_min; j++) {
                        v = 0.0;
                     	for (size_t k = c; k < c_min; k++) {
                         	v +=  mat1(i,k) * mat2(k,j);
                        }
                        ret(i,j) += v;
                     } 
                 }
             }
         }
     }

    return ret;
}

/*
 * dgemm matrix matrix multiplication.
 */
Matrix multiply_mkl(const Matrix & mat1, const Matrix & mat2)
{
	if (mat1.ncol() != mat2.nrow())
	{
		throw std::out_of_range(
            	"the number of first matrix column "
           	 "differs from that of second matrix row");
	}

    	Matrix ret(mat1.nrow(), mat2.ncol());
   	
	cblas_dgemm(CblasRowMajor, 
    	CblasNoTrans, 
    	CblasNoTrans, 
    	mat1.nrow(), 
    	mat2.ncol(), 
    	mat1.ncol(), 
    	1, 
    	mat1.m_buffer.data(), 
    	mat1.ncol(), 
    	mat2.m_buffer.data(), 
    	mat2.ncol(), 
    	0, 
    	ret.all_data(), 
    	mat2.ncol());
    	
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
	.def("all_data", &Matrix::all_data)
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
