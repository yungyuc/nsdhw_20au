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

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
        reset_buffer(0, 0);
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
        std::swap(m_buffer, other.m_buffer);
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
        		m_buffer[i] = 0.0;
        	}
        }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }
    
    double * all_data() const {return m_buffer;}
private:
    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

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

    for (size_t i=0; i<ret.nrow(); i++)
    {
        for (size_t k=0; k<ret.ncol(); k++)
        {
            for (size_t j=0; j<mat1.ncol(); j++)
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
