#!/bin/bash
#SBATCH --partition=batch
#SBATCH --time=01:00
#SBATCH --output=./slurm/v4/NACA0015/cilk/%j.out


#./main matrices_collection/belgium_osm.mtx $SLURM_NTASKS
CILK_NWORKERS=$SLURM_NTASKS ./main_cilk matrices_collection/NACA0015.mtx
