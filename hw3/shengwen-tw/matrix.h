#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <string>

#define N 1024
#define BLOCK_SIZE 64

class Matrix {
public:
	Matrix();
	Matrix(int _row, int _column) {
		this->data = new double[_row * _column]();
		this->n_row = _row;
		this->n_column = _column;
	}

	~Matrix() {
		if(data != nullptr) delete[] data;
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
				if(fabs(rhs.data[r * n_column + c] - data[r * n_column + c]) > 0.0001) {
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
				if(fabs(rhs.data[r * n_column + c] - data[r * n_column + c]) > 0.0001) {
					return true;
				}
			}
		}

		return false;
	}

	double & operator() (int r, int c) {
		return data[r * this->n_column + c];
	}

	const double & operator() (int r, int c) const {
		return data[r * this->n_column + c];
	}

	double at(int r, int c) {
		return data[r * n_column + c];
	}

	void set(int r, int c, double val) {
		data[r * n_column + c] = val;
	}

	double *raw_data(void) {
		return data;
	}

private:
	int n_row;
	int n_column;
	double *data;
};

void random_matrix(Matrix &mat, double max_val);
void multiply_naive(Matrix &mat1, Matrix &mat2, Matrix &mat_result);
void multiply_tile(Matrix &mat1, Matrix &mat2, Matrix &mat_result);
int matrix_compare(Matrix &mat1, Matrix &mat2);
void multiply_mkl(Matrix &mat1, Matrix &mat2, Matrix &mat_result);

#endif
