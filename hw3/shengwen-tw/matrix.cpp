#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <mkl.h>
#include <mkl_lapacke.h>
#include "matrix.h"

using namespace std;

void multiply_naive(Matrix &mat1, Matrix &mat2, Matrix &mat_result)
{
	for(int r = 0; r < mat1.row(); r++) {
		for(int c = 0; c < mat2.column(); c++) {
			for(int i = 0; i < mat1.column(); i++) {
				mat_result(r, c) += mat1(r, i) * mat2(i, c);
			}
		}
	}
}

void multiply_tile(Matrix &mat1, Matrix &mat2, Matrix &mat_result)
{
	int DIM = mat1.row();

	for(int r = 0; r < DIM; r+=BLOCK_SIZE) {
		for(int c = 0; c < DIM; c+=BLOCK_SIZE) {
			for(int i = 0; i < DIM; i++) {
				for(int br = r; br < r + BLOCK_SIZE; br++) {
					for(int bc = c; bc < c + BLOCK_SIZE; bc++) {
						mat_result(br, bc + 0) += mat1(br, i) * mat2(i, bc + 0);
					}
				}
			}
		}
	}
}

int matrix_compare(Matrix &mat1, Matrix &mat2)
{
	int difference = 0;
	double f;
	
	for(int r = 0; r < mat1.row(); r++) {
		for(int c = 0; c < mat1.column(); c++) {
			if((f = fabsf(mat1.at(r, c) - mat2.at(r, c))) > 0.01) {
				difference++;
				//cout << f << " ("<< r << "," << c << ")" << endl;
			}
		}
	}

	return difference;
}

void multiply_mkl(Matrix &mat1, Matrix &mat2, Matrix &mat_result)
{
        cblas_dgemm(CblasRowMajor,
		    CblasNoTrans,
		    CblasNoTrans,
		    mat1.row(),
                    mat2.column(),
		    mat1.column(),
		    1,
		    mat1.raw_data(),
		    mat1.column(),
                    mat2.raw_data(),
		    mat_result.column(),
		    0,
		    mat_result.raw_data(),
                    mat_result.column());	
}

double random_double(double max)
{
	return random() / ((double)RAND_MAX / 1000);
}

void random_matrix(Matrix &mat, double max_val)
{
	for(int r = 0; r < mat.row(); r++) {
		for(int c = 0; c < mat.column(); c++) {
			mat(r, c) = random_double(max_val);
		}
	}
}

void print_matrix(const char *prompt, Matrix &mat)
{
	cout << prompt << " (" << mat.row()  << "x" << mat.column() << "):\n";
	for(int r = 0; r < mat.row(); r++) {
		for(int c = 0; c < mat.column(); c++) {
			cout << mat.at(r, c) << "  ";
		}
		cout << "\n";
	}
	cout << "\n";
}
