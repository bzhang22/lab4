#!/bin/bash
#SBATCH --job-name=dse_4n_1r
#SBATCH --output=dse_4nodes_1rank.out
#SBATCH --nodes=4
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=4
#SBATCH --time=00:10:00

module load gcc openmpi

export OMP_NUM_THREADS=4
mpirun -np 4 --bind-to core ./sobel_mpi_omp
