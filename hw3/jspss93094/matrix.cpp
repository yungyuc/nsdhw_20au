#include<pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <mkl.h>


namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
        (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
        if (size() != vec.size())
        {
            throw std::out_of_range("number of elements mismatch");
        }

        size_t k = 0;
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = vec[k];
                ++k;
            }
        }

        return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(other.m_nrow, other.m_ncol);
        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
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
            for (size_t j=0; j<m_ncol; ++j)
            {
                (*this)(i,j) = other(i,j);
            }
        }
        return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
        if (this == &other) { return *this; }
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
        return *this;
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }


    size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) 
        { 
        	m_buffer = new double[nelement]; 
        	for(size_t i = 0; i<nelement; i++)
        	{
        		m_buffer[i] = 0;
        	}
        }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }
    
    double * all_data() {return m_buffer;}

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};

bool operator== (Matrix & mat1, Matrix & mat2)
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
Matrix multiply_naive(Matrix & mat1, Matrix & mat2)
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
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}

/*
 * Tiled matrix matrix multiplication.
 */
Matrix multiply_tile(Matrix & mat1, Matrix & mat2, int tile_width)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());
    //if(tile_width < 0) tile_width = 0;
     
     for (size_t a = 0; a < mat1.ncol(); a += tile_width) {
         for (size_t b = 0; b < mat2.ncol(); b += tile_width) {
             for (size_t c = 0; c < mat1.nrow(); c++) {
                 for (size_t i = a; i < std::min(a + tile_width, mat1.ncol()); i++) {
                     for (size_t j = b; j < std::min(b + tile_width, mat2.ncol()); j++) {
                         	ret(c,j) +=  mat1(c,i) * mat2(i,j);
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
Matrix multiply_mkl(Matrix & mat1, Matrix & mat2)
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
    	mat1.all_data(), 
    	mat1.ncol(), 
    	mat2.all_data(), 
    	mat2.ncol(), 
    	0, 
    	ret.all_data(), 
    	mat2.ncol());
    	
	return ret;
}


PYBIND11_MODULE(_matrix, m) {
	m.doc() = "pybind11 example plugin";
	m.def("multiply_naive", & multiply_naive);
	m.def("multiply_tile", & multiply_tile);
	m.def("multiply_mkl", & multiply_mkl);
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
