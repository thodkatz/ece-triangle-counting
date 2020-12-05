#!/bin/bash

echo "Hello, the scheduling started!"

sbatch --ntasks=1 submit_pthread.sh
for ((i=2; i<=20; i+=2))
do
sbatch --ntasks=$i submit_pthread.sh
done

