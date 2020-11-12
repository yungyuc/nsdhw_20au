#ifndef MATRIX_H_
#define MATRIX_H_

#include <pybind11/pybind11.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
using namespace std;
struct ByteCounterImpl
{

    size_t allocated = 0;
    size_t deallocated = 0;
    size_t refcount = 0;

};

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

};

template <class T>
struct MyAllocator
{

    using value_type = T;

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

};

static MyAllocator<double> myallocator;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), m_buffer(myallocator)
    {
        reset(nrow, ncol);
    }

    Matrix(Matrix && other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(myallocator)
    {
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    ~Matrix() { reset(0, 0); }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t size() const { return m_nrow * m_ncol; }

    double   operator() (size_t row, size_t col) const { return m_buffer[row + col * m_nrow]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[row + col * m_nrow]; }
    bool operator==(Matrix const &other) {
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            return false;
        }
        return true;
    }

    void reset(size_t nrow, size_t ncol)
    {
        m_buffer.resize(nrow * ncol);
        m_nrow = nrow;
        m_ncol = ncol;
    }

    double * all() {return m_buffer.data();}

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, MyAllocator<double>> m_buffer;
};
#endif