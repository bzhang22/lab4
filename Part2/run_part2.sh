#!/bin/bash
#SBATCH --job-name=sobel_hybrid
#SBATCH --output=sobel_experiments_%j.out
#SBATCH --error=sobel_experiments_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=16
#SBATCH --cpus-per-task=16
#SBATCH --time=00:30:00

# Try to load MPI module (uncomment and modify as per cluster if needed)
# module load openmpi

echo "Compiling MPI+OpenMP code..."
mpicc -O3 -fopenmp sobel_mpi_omp.c -o sobel_mpi_omp

echo "--- Run 1: Good Configuration (Balanced: 4 MPI tasks, 4 OpenMP threads) ---"
export OMP_NUM_THREADS=4
mpirun -np 4 ./sobel_mpi_omp

echo "--- Run 2: Bad Configuration 1 (Oversubscribing: 16 MPI tasks, 16 OpenMP threads) ---"
export OMP_NUM_THREADS=16
mpirun -np 16 --oversubscribe ./sobel_mpi_omp

echo "--- Run 3: Bad Configuration 2 (MPI dominant: 16 MPI tasks, 1 OpenMP thread) ---"
export OMP_NUM_THREADS=1
mpirun -np 16 ./sobel_mpi_omp

echo "--- Run 4: Bad Configuration 3 (OpenMP dominant: 1 MPI task, 16 OpenMP threads) ---"
export OMP_NUM_THREADS=16
mpirun -np 1 ./sobel_mpi_omp

echo "Experiments finished. To generate your report plots, please run: python3 generate_plot.py sobel_experiments_$SLURM_JOB_ID.out"
