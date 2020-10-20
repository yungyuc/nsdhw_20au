#include <iostream>
#include <mkl.h>
#include <mkl_lapacke.h>
#include "matrix.h"

using namespace std;

void multiply_naive(Matrix &mat1, Matrix &mat2, Matrix &mat_result)
{
	for(int r = 0; r < mat1.row(); r++) {
		for(int c = 0; c < mat2.column(); c++) {
			float tmp = 0;
			for(int i = 0; i < mat1.column(); i++) {
				tmp += mat1(r, i) * mat2(i, c);
			}
			mat_result(r, c) = tmp;
		}
	}
}

void multiply_tile(Matrix &mat1, Matrix &mat2, Matrix &mat_result)
{
}

void multiply_mkl(Matrix &mat1, Matrix &mat2, Matrix &mat_result)
{
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat_result.row(),
                    mat_result.column(), mat2.row(), 1, mat1.raw_data(), mat2.row(),
                    mat2.raw_data(), mat_result.column(), 0, mat_result.raw_data(),
                    mat_result.column());	
}

float random_float(float max)
{
	return random() / ((float)RAND_MAX / 1000);
}

void random_matrix(Matrix &mat, float max_val)
{
	for(int r = 0; r < mat.row(); r++) {
		for(int c = 0; c < mat.column(); c++) {
			mat(r, c) = random_float(max_val);
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
}

int main(void)
{
	Matrix mat1(2, 2);
	random_matrix(mat1, 1000);

	Matrix mat2(2, 2);
	random_matrix(mat2, 1000);

	Matrix m_naive(2, 2);
	Matrix m_tile(2, 2);
	Matrix m_mkl(2, 2);

	multiply_naive(mat1, mat2, m_naive);
	multiply_mkl(mat1, mat2, m_mkl);

	print_matrix("mat1", mat1);
	print_matrix("mat2", mat2);

	print_matrix("m_naive", m_naive);
	print_matrix("m_mkl", m_mkl);

	return 0;
}
