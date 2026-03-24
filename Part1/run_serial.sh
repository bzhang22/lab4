#!/bin/bash
#SBATCH --job-name=sobel_serial
#SBATCH --output=sobel_serial_%j.out
#SBATCH --error=sobel_serial_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=00:05:00

# Compile the serial C program
gcc -O3 sobel.c -o sobel

# Run the program
./sobel
