#ifndef TRIANGLE_COUNTING_H
#define TRIANGLE_COUNTING_H

#include<stdio.h>
#include<stdint.h>
#include<time.h>

uint64_t* v1(int*, const uint32_t);
uint64_t* v2(int*, const uint32_t);
uint64_t* v3(int*, const uint32_t);
double diff_time(struct timespec start, struct timespec end);

#endif
