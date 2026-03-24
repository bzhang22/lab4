#!/bin/bash
#SBATCH --job-name=dse_1n_4r
#SBATCH --output=dse_1node_4ranks.out
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=4
#SBATCH --time=00:10:00

module load gcc openmpi

export OMP_NUM_THREADS=4
mpirun -np 4 ./sobel_mpi_omp
