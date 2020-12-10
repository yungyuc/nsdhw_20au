#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include <string>
#include <vector>
#include "matrix_allocator.h"

#define N 1024
#define BLOCK_SIZE 10

using namespace std;

extern MatrixDataAllocator<double> matrix_memory_manager;

class Matrix {

public:
    Matrix(int row, int column)
        : n_row(row),
          n_column(column),
          data(row * column, 0, matrix_memory_manager)
    {
        std::fill(data.begin(), data.end(), 0);
    }

    Matrix(Matrix const & other)
        : n_row(other.n_row),
          n_column(other.n_column),
          data(other.n_row * other.n_column, 0, matrix_memory_manager)
    {
        std::copy(other.data.begin(), other.data.end(), data.begin());
    }

    Matrix(Matrix && other)
        : n_row(other.n_row),
          n_column(other.n_column),
          data(matrix_memory_manager)
    {
        other.data.swap(data);
    }

    Matrix(std::vector<std::vector<double>> const & other)
        : n_row(other.size()), n_column(other[0].size())
    {
        for(const auto &other_data: other) {
            data.insert(data.end(), other_data.begin(), other_data.end());
        }
    }

    ~Matrix() {
    }

    void clean(void) {
        data.clear();
	}

	int row(void) {
		return this->n_row;
	}

	int column(void) {
		return this->n_column;
	}

	bool operator== (const Matrix &rhs) {
		if(rhs.n_row != n_row) return false;
		if(rhs.n_column != n_column) return false;

		for(int r = 0; r < n_row; r++) {
			for(int c = 0; c < n_column; c++) {
				if(fabs(rhs.data.at(r * n_column + c) - data.at(r * n_column + c)) > 0.0001) {
					return false;
				}
			}
		}
		return true;
	}

	bool operator!= (const Matrix &rhs) {
		if(rhs.n_row != n_row) return true;
		if(rhs.n_column != n_column) return true;

		for(int r = 0; r < n_row; r++) {
			for(int c = 0; c < n_column; c++) {
				if(fabs(rhs.data.at(r * n_column + c) - data.at(r * n_column + c)) > 0.0001) {
					return true;
				}
			}
		}

		return false;
	}

    double operator() (int row, int col) const {
        return data.at(row * n_column + col);
    }

    double &operator() (int row, int col) {
        return data.at(row * n_column + col);
    }

	double *raw_data(void) {
        return &data[0];
	}

private:
    int n_row;
    int n_column;
    std::vector<double, MatrixDataAllocator<double>> data;
};

size_t bytes();
size_t allocated();
size_t deallocated();
void random_matrix(Matrix &mat, double max_val);
Matrix multiply_naive(Matrix &mat1, Matrix &mat2);
Matrix  multiply_tile(Matrix &mat1, Matrix &mat2, int tile_size);
int matrix_compare(Matrix &mat1, Matrix &mat2);
Matrix multiply_mkl(Matrix &mat1, Matrix &mat2);
void print_matrix(const char *prompt, Matrix &mat);

#endif
