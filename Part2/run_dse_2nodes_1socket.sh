#!/bin/bash
#SBATCH --job-name=dse_2n_2r_1s
#SBATCH --output=dse_2nodes_1socket.out
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=2
#SBATCH --ntasks-per-socket=1
#SBATCH --cpus-per-task=4
#SBATCH --time=00:10:00

module load gcc openmpi

export OMP_NUM_THREADS=4
mpirun -np 4 --bind-to core --map-by socket ./sobel_mpi_omp
