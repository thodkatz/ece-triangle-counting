#!/bin/bash
#SBATCH --partition=batch
#SBATCH --time=01:00
#SBATCH --output=./slurm/v4/NACA0015/pthread/%j.out


./main_pthread matrices_collection/NACA0015.mtx $SLURM_NTASKS
