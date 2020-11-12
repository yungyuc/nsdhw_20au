#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include <vector>
#include <stdexcept>
#include <mkl.h>
#include <atomic>

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

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc) {
        reset_buffer(nrow, ncol);
    }

    Matrix(Matrix const &other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(alloc) {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                (*this)(i,j) = other(i,j);
            }
        }
    }

    ~Matrix() {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    bool operator==(Matrix const &other) {
        if (this == &other) {
            return true;
        }
        if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
            return false;
        }
        for (size_t i = 0; i < m_nrow; ++i) {
            for (size_t j = 0; j < m_ncol; ++j) {
                if ((*this)(i,j) != other(i,j)) {
                    return false;
                }
            }
        }
        return true;
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    size_t size() const { return m_nrow * m_ncol; }

    double buffer(size_t i) const { return m_buffer[i]; }
    double *data() { return m_buffer.data(); }

    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer.data(), m_buffer.data() + size()); }

    size_t index(size_t row, size_t col) const {
        return row + col * m_nrow;
    }
    void reset_buffer(size_t nrow, size_t ncol) {
        const size_t nelement = nrow * ncol;
        m_buffer.reserve(nelement);
        m_buffer.clear();
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, MyAllocator<double>> m_buffer;
};

Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("Incorrect dimensions for matrix multiplication");
    }

    size_t nrow = mat1.nrow();
    size_t ncol = mat2.ncol();

    Matrix ret(nrow, ncol);

    for (size_t i = 0; i < nrow; ++i) {
        for (size_t k = 0; k < ncol; ++k) {
            for (size_t j = 0; j < mat1.ncol(); ++j) {
                ret(i,k) += mat1(i,j) * mat2(j,k);
            }
        }
    }

    return ret;
}

inline size_t edge(size_t n, size_t ori) {
    if (n > ori) {
        return ori;
    }
    return n;
}

Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t tile_size) {
    if (mat1.ncol() != mat2.nrow()) {
        throw std::out_of_range("Incorrect dimensions for matrix multiplication");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < mat1.nrow(); i += tile_size) {
        for (size_t k = 0; k < mat2.ncol(); k += tile_size) {
            for (size_t j = 0; j <  mat1.ncol(); j += tile_size) {
                // multiply_naive()
                for (size_t tile_j = j; tile_j < edge(j + tile_size, mat1.ncol()); ++tile_j) {
                    for (size_t tile_i = i; tile_i < edge(i + tile_size, mat1.nrow()); ++tile_i) {
                        for (size_t tile_k = k; tile_k < edge(k + tile_size,mat2.ncol()); ++tile_k) {
                            ret(tile_i, tile_k) += mat1(tile_i, tile_j) * mat2(tile_j, tile_k);
                        }
                    }
                }
            }
        }
    }

    return ret;
}


Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2) {
    Matrix ret = Matrix(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1.nrow(), mat2.ncol(), mat1.ncol(), 1, mat1.m_buffer.data(), mat1.ncol(), mat2.m_buffer.data(), mat2.ncol(), 0, ret.m_buffer.data(), mat2.ncol());

    return ret;
}

size_t bytes() { return alloc.counter.bytes(); }
size_t allocated() { return alloc.counter.allocated(); }
size_t deallocated() { return alloc.counter.deallocated(); }

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "matrix mutiplication module";
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())        
        .def(py::init<size_t, size_t>())
        .def("data", &Matrix::data)
        .def("size", &Matrix::size)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__eq__", &Matrix::operator==)
        .def("__getitem__", [](const Matrix &mat, std::pair<size_t, size_t> i){
            return mat(i.first, i.second);
        })
        .def("__setitem__", [](Matrix &mat, std::pair<size_t, size_t> i, double val){
            mat(i.first, i.second) = val;
        });
}
