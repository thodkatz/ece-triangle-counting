#!/bin/bash
#SBATCH --partition=batch
#SBATCH --time=01:00
#SBATCH --output=./slurm/v4/youtube/openmp/%j.out


./main_openmp matrices_collection/com-Youtube.mtx $SLURM_NTASKS
