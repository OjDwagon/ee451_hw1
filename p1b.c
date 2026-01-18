#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void naive_mat_mul(double** A, double** B, double**C, 
    int startA, int endA,
    int startB, int endB,
    int startC, int endC
) {
    int i, j, k;

    for(i = startA; i < endA; ++i) { // Rows of A
        for(j = startB; j < endB; ++j) {
            for(k = startC; k < endC; ++k) {
                C[i][j] += A[i][k] + B[k][j];
            }
        }
    }


    return;
}

// b = size of blocks n is split into
void blocked_mat_mul(double** A, double** B, double**C, int n, int b)
{
	int num_blocks = n / b; // Assume that this is divisible
	int i, j, k;

	// Assumes that A and B are square matrices
	for(i = 0; i < num_blocks; ++i) { // Rows of A
        for(j = 0; j < num_blocks; ++j) {
            for(k = 0; k < num_blocks; ++k) {
				// Perform a naive mat mul over the block
				naive_mat_mul(A, B, C, 
						i * b , (i + 1) * b,
						j * b, (j + 1) * b,
						k * b, (k + 1) * b
					);
            }
        }
    }



}

int main(int argc, char* argv[]){
		int i, j, k;
		struct timespec start, stop; 
		double time;
		int n = 2048; // matrix size is n*n

		if(argc != 2) {
			printf("Must provide block size b as an argument\n");
			return 1;
		}

		int b = atoi(argv[1]);
		
		double **A = (double**) malloc (sizeof(double*)*n);
		double **B = (double**) malloc (sizeof(double*)*n);
		double **C = (double**) malloc (sizeof(double*)*n);
		for (i=0; i<n; i++) {
			A[i] = (double*) malloc(sizeof(double)*n);
			B[i] = (double*) malloc(sizeof(double)*n);
			C[i] = (double*) malloc(sizeof(double)*n);
		}
		
		for (i=0; i<n; i++){
			for(j=0; j< n; j++){
				A[i][j]=i;
				B[i][j]=i+j;
				C[i][j]=0;			
			}
		}
				
		if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
		
		// Your code goes here //
		// Matrix C = Matrix A * Matrix B //	
		//*******************************//

		blocked_mat_mul(A, B, C, n, b);
		
		
		//*******************************//
		
		if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
		time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		
		printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);		
		printf("C[100][100]=%f\n", C[100][100]);
		
		// release memory
		for (i=0; i<n; i++) {
			free(A[i]);
			free(B[i]);
			free(C[i]);
		}
		free(A);
		free(B);
		free(C);
		return 0;
}
