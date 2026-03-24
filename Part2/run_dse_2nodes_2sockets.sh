#!/bin/bash
#SBATCH --job-name=dse_2n_2r_2s
#SBATCH --output=dse_2nodes_2sockets.out
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=2
#SBATCH --ntasks-per-socket=2
#SBATCH --cpus-per-task=4
#SBATCH --time=00:10:00

module load gcc openmpi

export OMP_NUM_THREADS=4
mpirun -np 4 --bind-to core --map-by node ./sobel_mpi_omp
