#!/bin/bash
#SBATCH --partition=batch
#SBATCH --time=01:00
#SBATCH --output=./slurm/v4/NACA0015/%j.out


./main matrices_collection/NACA0015.mtx
