#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <mkl.h>
#include <mkl_lapacke.h>
#include "matrix.h"

using namespace std;

double time(void)
{
        static int sec = -1;
        struct timeval tv;
        gettimeofday(&tv, NULL);

        if (sec < 0) sec = tv.tv_sec;
        return (tv.tv_sec - sec) + 1.0e-6 * tv.tv_usec;
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
