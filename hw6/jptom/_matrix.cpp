#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <tuple>
#include <mkl.h>
#include <stdexcept>
#include <functional>
#include <vector>

namespace py=pybind11;

struct ByteCounterImpl
{
	size_t allocated = 0;
	size_t deallocated = 0;
	size_t refcount = 0;
};

class ByteCounter
{
public:
	ByteCounter():m_impl(new ByteCounterImpl) { incref(); }
	ByteCounter(ByteCounter const & other) :m_impl(other.m_impl) { incref(); }
	ByteCounter & operator=(ByteCounter const & other)
	{
		if ( &other != this)
		{
			decref();
			m_impl = other.m_impl;
			incref();
		}

		return *this;
	}
	ByteCounter(ByteCounter && other):m_impl(other.m_impl) { other.decref(); }
	ByteCounter & operator=(ByteCounter && other)
	{
		if ( &other != this){
			decref();
			m_impl = other.m_impl;
		}

		return *this;
	}
	~ByteCounter() { decref(); }

	void swap(ByteCounter & other)
	{
		std::swap(m_impl, other.m_impl);
	}
	void increase(size_t amount)
	{
		m_impl->allocated += amount;
	}
	void decrease(size_t amount)
	{
		m_impl->deallocated += amount;
	}
	size_t bytes() const { return m_impl->allocated - m_impl->deallocated; }
	size_t allocated() const { return m_impl->allocated; }
	size_t deallocated() const { return m_impl->deallocated; }

private:
	void incref() { ++m_impl->refcount; }
	void decref()
	{
		if (nullptr == m_impl) {}
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
	MyAllocator(const MyAllocator<U> & other) noexcept: counter(other.counter)
	{
		//counter = other.counter;
	}

	T * allocate(std::size_t n)
	{
		if (n > std::numeric_limits<size_t>::max() / sizeof(T))
		{
			throw std::bad_alloc();
		}
		const size_t bytes = n*sizeof(T);
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

	void deallocate(T * p, size_t n) noexcept
	{
		std::free(p);
		const size_t bytes = n*sizeof(T);
		counter.decrease(bytes);
	}

	ByteCounter counter;

};

template <class T, class U>
bool operator==(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
	return a.counter == b.counter;
}

template <class T, class U>
bool operator !=(const MyAllocator<T> & a, const MyAllocator<U> & b)
{
	return a.counter != b.counter;
}

static MyAllocator<double> allocator;

struct Matrix
{
public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol), m_buffer(nrow * ncol, 0, allocator) {
        std::fill(m_buffer.begin(), m_buffer.end(), 0);
    }

    // copy constructor
    Matrix(Matrix const & other): m_nrow(other.m_nrow), m_ncol(other.m_ncol), m_buffer(other.m_nrow * other.m_ncol, 0, allocator) {
        std::copy(other.m_buffer.begin(), other.m_buffer.end(), m_buffer.begin());
    }

	Matrix & operator=(Matrix const & other)
	{
		if (this == &other) { return *this; }
		if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
		{
			reset_buffer(other.m_nrow, other.m_ncol);
		}
		for (size_t i=0; i<m_nrow; ++i)
		{
			m_buffer[i] = other.m_buffer[i];
		}

		return *this;
	}

	Matrix(Matrix && other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
	{
		reset_buffer(0, 0);
		std::swap(m_nrow, other.m_nrow);
		std::swap(m_ncol, other.m_ncol);
		std::swap(m_buffer, other.m_buffer);
	}

	Matrix & operator=(Matrix && other)
	{
		if (this == &other) { return * this; }
		reset_buffer(0, 0);
		std::swap(m_nrow, other.m_nrow);
		std::swap(m_ncol, other.m_ncol);
		std::swap(m_buffer, other.m_buffer);
		return * this;
	}

	~Matrix()
	{
		reset_buffer(0, 0);
	}

	// check before access
	double  operator() (const size_t row, const size_t col) const { return m_buffer[index(row, col)]; }
	double & operator() (const size_t row, const size_t col)       { return m_buffer[index(row, col)]; }

	//no check access
	double   buffer(const size_t i) const { return m_buffer[i]; }
	double & buffer(const size_t i)       { return m_buffer[i]; }

	size_t nrow() const { return m_nrow; }
	size_t ncol() const { return m_ncol; }

	bool operator== (const Matrix & other)
	{
		if (this == &other) { return true; }
		if (m_nrow != other.nrow() || m_ncol != other.ncol()) { return false; }

		const size_t len = m_nrow*m_ncol;
		bool flag = true;

		for (size_t i=0; i<len; ++i)
		{
			if( m_buffer[i] != other.buffer(i))
			{
				flag = false;
				break;
			}
		}

		return ( flag ? true : false );
	}

	void fill(const double v) {for (size_t i=0; i<m_nrow*m_ncol; ++i) { m_buffer[i] = v; } }

public:

	size_t index(size_t const row, size_t const col) const
	{
		if ( m_nrow<row || m_ncol<col) { throw std::out_of_range("Matrix index out of range"); }
		return row + col * m_nrow;
	}

	void reset_buffer(const size_t nrow,const size_t ncol)
	{
		const size_t nelement = nrow * ncol;
		if (nelement) {
			m_buffer.reserve(nelement);
			m_buffer.clear();
			m_nrow = nrow;
			m_ncol = ncol;
		}
	}
	double *data() { return m_buffer.data(); }

	size_t m_nrow = 0;
	size_t m_ncol = 0;
	std::vector<double, MyAllocator<double>> m_buffer;
};

/*
 *  Direct naive matrix multiplication.
 */
Matrix * multiply_naive(const Matrix & mat1, const Matrix & mat2)
{
	//std::cout<<"muliply_naive start"<<std::endl;

	if (mat1.ncol() != mat2.nrow())
	{
		throw std::out_of_range("the number of first matrix column differs from that of second matrix row");
	}

	Matrix * ret = new Matrix(mat1.nrow(), mat2.ncol());

	const size_t nrow1 = mat1.nrow();
	const size_t ncol1 = mat1.ncol();
	const size_t ncol2 = mat2.ncol();

	for (size_t i=0; i<nrow1; ++i)
	{
		const size_t base1 = i * ncol1;
		for (size_t j=0; j<ncol2; ++j)
		{
			double v = 0;
			for (size_t k=0; k<ncol1; ++k){
				v += mat1.buffer(base1 + k) * mat2.buffer(k*ncol2 + j);
			}
			(*ret).buffer(base1 + j) = v;
		}
	}
	return ret;
}

/*
 *  Tiled matrix matrix multiplication.
 */
void process_tile(Matrix * ret, const Matrix & mat1, const Matrix & mat2, const size_t ncol1, const size_t nrow2, const size_t ncol2,
		  const size_t i_tsize, const size_t j_tsize, const size_t k_tsize, const size_t itsize, const size_t jtsize, const size_t ktsize)
{
	for (size_t ti=0; ti<itsize; ++ti)
	{
		const size_t ibase = (i_tsize + ti)*ncol1 + k_tsize;
		for (size_t tj=0; tj<jtsize; ++tj)
		{
			const size_t jbase = (j_tsize + tj)*nrow2 + k_tsize;
			const size_t ret_ij = (i_tsize + ti)*ncol2 + j_tsize + tj;
			double v = 0;
			for (size_t tk=0; tk<ktsize; ++tk)
			{
				//std::cout<<ret_ij<<" "<<ibase+tk<<" "<<jbase+tk<<std::endl;
				v += mat1.buffer(ibase + tk)*mat2.buffer(jbase + tk);
			}
			(*ret).buffer(ret_ij) += v;
		}
	}
}

Matrix * multiply_tile(const Matrix & mat1, const Matrix & mat2, const size_t tsize)
{
	//std::cout<<"multiply_itle start"<<std::endl;

	if (mat1.ncol() != mat2.nrow())
	{
		throw std::out_of_range("the number of first matrix column differs from that of second matrix row");
	}

	if (tsize <= 0 || mat1.nrow()<=tsize || mat1.ncol()<=tsize || mat2.ncol()<=tsize) { return multiply_naive(mat1, mat2); }

	Matrix colmat2 = Matrix(mat2.nrow(), mat2.ncol());

	Matrix * ret = new Matrix(mat1.nrow(), mat2.ncol());

	const size_t nrow1 = mat1.nrow();
	const size_t ncol1 = mat1.ncol();
	const size_t nrow2 = mat2.nrow();
	const size_t ncol2 = mat2.ncol();

	const size_t ntrow1 = nrow1/tsize;
	const size_t ntcol2 = ncol2/tsize;
	const size_t ntcol1 = ncol1/tsize;

	// rowmajor to colmajor
	//std::cout<<"multiply_itle rowmajor to colmajor"<<std::endl;
	for (size_t i=0; i<nrow2; ++i)
	{
		for (size_t j=0; j<ncol2; ++j)
		{
			colmat2.buffer(j*nrow2 + i) = mat2.buffer(i*ncol2 + j);
		}
	}

	const size_t remnrow1 = nrow1 % tsize;
	const size_t remncol2 = ncol2 % tsize;
	const size_t remncol1 = ncol1 % tsize;

	//std::cout<<"multiply_itle multiply"<<std::endl;
	for (size_t i=0; i<ntrow1; ++i)
	{
		for (size_t j=0; j<ntcol2; ++j)
		{
			for (size_t k=0; k<ntcol1; ++k)
			{
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, tsize, tsize, tsize);

			}
			if(remncol1)
			{
				const size_t k = ntcol1;
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, tsize, tsize, remncol1);
			}
		}
		if (remncol2)
		{
			const size_t j = ntcol2;
			for (size_t k=0; k<ntcol1; ++k)
			{
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, tsize, remncol2, tsize);
			}
			if(remncol1)
			{
				size_t k = ntcol1;
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, tsize, remncol2, remncol1);
			}
		}

	}
	if (remnrow1)
	{
		const size_t i = ntrow1;
		for (size_t j=0; j<ntcol2; ++j)
		{
			for (size_t k=0; k<ntcol1; ++k)
			{
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, remnrow1, tsize, tsize);

			}
			if(remncol1)
			{
				const size_t k = ntcol1;
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, remnrow1, tsize, remncol1);
			}
		}
		if (remncol2)
		{
			const size_t j = ntcol2;
			for (size_t k=0; k<ntcol1; ++k)
			{
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, remnrow1, remncol2, tsize);
			}
			if(remncol1)
			{
				size_t k = ntcol1;
				process_tile(ret, mat1, colmat2, ncol1, nrow2, ncol2, i*tsize, j*tsize, k*tsize, remnrow1, remncol2, remncol1);
			}
		}
	}

	return ret;
}


/*
 * Use MKL for the matrix matrix multiplication.
 */
Matrix * multiply_mkl(const Matrix & mat1, const Matrix & mat2)
{
	//std::cout<<"multiply_mkl start"<<std::endl;

	mkl_set_num_threads(1);

	Matrix * ret = new Matrix(mat1.nrow(), mat2.ncol());

	cblas_dgemm(
		CblasRowMajor,
		CblasNoTrans,
		CblasNoTrans,
		mat1.nrow(),
		mat2.ncol(),
		mat1.ncol(),
		1.0,
		mat1.m_buffer.data(),
		mat1.ncol(),
		mat2.m_buffer.data(),
		mat2.ncol(),
		0.0,
		(*ret).m_buffer.data(),
		(*ret).ncol()
		);
	return ret;
}

size_t bytes() { return allocator.counter.bytes(); }
size_t allocated() { return allocator.counter.allocated(); }
size_t deallocated() { return allocator.counter.deallocated(); }

PYBIND11_MODULE(_matrix, m)
{
	m.doc() = "pybind11 matrix multiplication test";
	m.def("multiply_naive", & multiply_naive, "naive method");
	m.def("multiply_tile", & multiply_tile, "Tiling solution");
	m.def("multiply_mkl", & multiply_mkl, "use mkl");
	m.def("bytes", &bytes);
	m.def("allocated", &allocated);
	m.def("deallocated", &deallocated);
	py::class_<Matrix>(m, "Matrix")
		.def(py::init<const size_t, const size_t>())
		.def_property_readonly("nrow", &Matrix::nrow)
		.def_property_readonly("ncol", &Matrix::ncol)
        .def_property_readonly("array",[](Matrix & mat)
		{
			return py::array_t<double>(
				{mat.nrow(), mat.ncol()},
				{sizeof(double) * mat.ncol(), sizeof(double)},
				mat.data(),
				py::cast(mat)
			);
		})
		.def("__getitem__", [](const Matrix & mat, std::tuple<size_t, size_t> t) -> double
		{
			//std::cout<<"getitem:start"<<std::endl;
			return mat(std::get<0>(t), std::get<1>(t));
		})
		.def("__setitem__", [](Matrix & mat, std::tuple<size_t, size_t> t, const double & v)
		{
			//std::cout<<"setitem:start"<<std::endl;
			mat(std::get<0>(t), std::get<1>(t)) = v;
		})
		.def("__eq__", &Matrix::operator==)
		;
}