#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Names: Bohan Zhang
 * EEL 6763 Spring 2026
 * Lab 4: Sobel Edge Detector - Hybrid Programming Model
 * Part 2: MPI + OpenMP Implementation
 */

#define N 5000

int main(int argc, char** argv) {
    int rank, size;
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *input_1d = (int*)malloc(N * N * sizeof(int));
    if (!input_1d) {
        printf("Rank %d failed to allocate input array\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int (*input)[N] = (int (*)[N]) input_1d;

    int *output_1d = NULL;
    int (*output)[N] = NULL;
    if (rank == 0) {
        output_1d = (int*)malloc(N * N * sizeof(int));
        output = (int (*)[N]) output_1d;
    }

    if (rank == 0) {
        FILE *fin = fopen("../input.txt", "r");
        if (!fin) {
            printf("Cannot open ../input.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("Rank 0: Reading input...\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fscanf(fin, "%d", &input[i][j]);
            }
        }
        fclose(fin);
    }

    double start_time = MPI_Wtime();

    MPI_Bcast(input_1d, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = N / size;
    int start_row = rank * rows_per_proc;
    int end_row = (rank == size - 1) ? N : start_row + rows_per_proc;
    int num_rows = end_row - start_row;

    int *local_output = (int*)calloc(num_rows * N, sizeof(int));

    #pragma omp parallel for schedule(dynamic)
    for (int i = start_row; i < end_row; i++) {
        if (i == 0 || i == N - 1) continue;

        for (int j = 1; j < N - 1; j++) {
            int p1 = input[i-1][j-1];
            int p2 = input[i-1][j];
            int p3 = input[i-1][j+1];
            int q1 = input[i][j-1];
            int q3 = input[i][j+1];
            int r1 = input[i+1][j-1];
            int r2 = input[i+1][j];
            int r3 = input[i+1][j+1];

            int Gx = (p1 - p3) + 2*(q1 - q3) + (r1 - r3);
            int Gy = (p1 - r1) + 2*(p2 - r2) + (p3 - r3);

            local_output[(i - start_row) * N + j] = abs(Gx) + abs(Gy);
        }
    }

    int *recvcounts = NULL;
    int *displs = NULL;
    if (rank == 0) {
        recvcounts = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));
        int offset = 0;
        for (int i = 0; i < size; i++) {
            int r_start = i * rows_per_proc;
            int r_end = (i == size - 1) ? N : r_start + rows_per_proc;
            recvcounts[i] = (r_end - r_start) * N;
            displs[i] = offset;
            offset += recvcounts[i];
        }
    }

    MPI_Gatherv(local_output, num_rows * N, MPI_INT, output_1d, recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Writing output...\n");
        FILE *fout = fopen("processed_matrix.txt", "w");
        if (!fout) {
            printf("Cannot open processed_matrix.txt for writing\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fprintf(fout, "%d", output[i][j]);
                if (j < N - 1) fprintf(fout, "\t");
            }
            fprintf(fout, "\n");
        }
        fclose(fout);
        
        double end_time = MPI_Wtime();
        printf("Time elapsed for Sobel (MPI + OpenMP): %f seconds\n", end_time - start_time);
        
        free(recvcounts);
        free(displs);
        free(output_1d);
    }

    free(local_output);
    free(input_1d);
    MPI_Finalize();
    return 0;
}
