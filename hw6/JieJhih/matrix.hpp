#ifndef MATRIX_H_
#define MATRIX_H_

#include <cstddef>
#include <vector>

class Matrix {
    public:
        Matrix(size_t row, size_t col) : m_row(row), m_col(col), m_buffer(row * col, 0) {}
        Matrix(const std::vector<std::vector<double>> &m) {
              m_row = m.size();
              m_col = m[m_row-1].size();
              m_buffer.clear();
              for(size_t i=0; i<m_row; i++)
                for(size_t j=0; j<m_col; j++)
                  m_buffer.push_back(m[i][j]);
        }
        size_t nrow() const { return m_row; }
        size_t ncol() const { return m_col; }
        double *buffer() { return &m_buffer[0]; }
        const double *buffer() const { return &m_buffer[0]; }
        double * data() {return m_buffer.data();}
        bool operator == (Matrix const &other) const {
            if (this == &other) return true;
            if (m_row != other.m_row || m_col != other.m_col) return false;            
            return m_buffer == other.m_buffer;
        }
        double & operator() (size_t r, size_t c) {
		    return m_buffer[r * m_col + c];
	    }
        const double & operator() (size_t r, size_t c) const {
		    return m_buffer[r * m_col + c];
	    }

    private:
        size_t m_row;
        size_t m_col;
        std::vector<double> m_buffer;
};

Matrix multiply_naive(const Matrix &mt1, const Matrix &mt2);
Matrix multiply_tile(const Matrix &mt1, const Matrix &mt2, size_t t_size);
Matrix multiply_mkl(const Matrix &mt1, const Matrix &mt2);

#endif