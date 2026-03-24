#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
 * Names: Bohan Zhang
 * EEL 6763 Spring 2026
 * Lab 4: Sobel Edge Detector - Hybrid Programming Model
 * Part 1: Serial Implementation
 */

#define N 5000

int input[N][N];
int output[N][N];

int main(int argc, char** argv) {
    FILE *fin = fopen("../input.txt", "r");
    if (!fin) {
        printf("Cannot open ../input.txt\n");
        return 1;
    }

    printf("Reading input...\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(fin, "%d", &input[i][j]);
        }
    }
    fclose(fin);

    printf("Applying Sobel filter...\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 1; i < N - 1; i++) {
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

            output[i][j] = abs(Gx) + abs(Gy);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Time elapsed for Sobel: %f seconds\n", time_spent);

    printf("Writing output...\n");
    FILE *fout = fopen("processed_matrix.txt", "w");
    if (!fout) {
        printf("Cannot open processed_matrix.txt for writing\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(fout, "%d", output[i][j]);
            if (j < N - 1) fprintf(fout, "\t");
        }
        fprintf(fout, "\n");
    }
    fclose(fout);

    return 0;
}
