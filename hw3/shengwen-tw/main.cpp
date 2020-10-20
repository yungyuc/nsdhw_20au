#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <mkl.h>
#include <mkl_lapacke.h>
#include "matrix.h"

#define N 1024
#define BLOCK_SIZE 64

using namespace std;

double time(void)
{
        static int sec = -1;
        struct timeval tv;
        gettimeofday(&tv, NULL);

        if (sec < 0) sec = tv.tv_sec;
        return (tv.tv_sec - sec) + 1.0e-6 * tv.tv_usec;
}

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

int main(void)
{
	Matrix mat1(N, N);
	random_matrix(mat1, 1000);

	Matrix mat2(N, N);
	random_matrix(mat2, 1000);

	Matrix m_naive(N, N);
	Matrix m_tile(N, N);
	Matrix m_mkl(N, N);

	double start_time, end_time;

	start_time = time();
	multiply_naive(mat1, mat2, m_naive);
	end_time = time();
	double cost_naive = end_time - start_time;
	printf("time cost of the naive multiply: %lf seconds\n", cost_naive);

	start_time = time();
	multiply_tile(mat1, mat2, m_tile);
	end_time = time();
	double cost_tile = end_time - start_time;
	printf("time cost of the tile multiply: %lf seconds\n", cost_tile);

	start_time = time();
	multiply_mkl(mat1, mat2, m_mkl);
	end_time = time();
	double cost_mkl = end_time - start_time;;
	printf("time cost of the mkl multiply: %lf seconds\n", cost_mkl);

	printf("tile multiply is %lfx times faster than naive multiply\n", cost_naive / cost_tile);

	printf("\nmatrix correctness checking:\n");
	int naive_tile_diff = matrix_compare(m_naive, m_tile);
	int tile_mkl_diff = matrix_compare(m_tile, m_mkl);
	int naive_mkl_diff = matrix_compare(m_naive, m_mkl);
	printf("%d elements of m_naive and m_tile are differenct\n", naive_tile_diff);
	printf("%d elements of m_tile and m_mkl are different\n", tile_mkl_diff);
	printf("%d elements of m_naive and m_mkl are differenct\n", naive_mkl_diff);

	if((naive_tile_diff == 0) && (tile_mkl_diff == 0) && (naive_mkl_diff == 0)) {
		printf("-> correct, all answer are matched!\n");
	} else {
		printf("-> failed, something is wrong!\n");
	}

#if 0
	print_matrix("mat1", mat1);
	print_matrix("mat2", mat2);

	print_matrix("m_naive", m_naive);
	print_matrix("m_tile", m_tile);
	print_matrix("m_mkl", m_mkl);
#endif

	return 0;
}
