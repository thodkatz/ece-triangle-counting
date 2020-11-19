#include<time.h>
#include<stdint.h>
#include<stdio.h>

double diff_time (struct timespec start, struct timespec end) {
    uint32_t diff_sec = (end.tv_sec - start.tv_sec);
    int64_t diff_nsec = (end.tv_nsec - start.tv_nsec);
    //printf("Diff sec %d and diff nsec %d\n", diff_sec, diff_nsec);
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        diff_sec -= 1;
        diff_nsec = 1e9 + end.tv_nsec - start.tv_nsec;
        //printf("We add %lf with %ld and we get %f and we substract %ld and get %ld\n", 1e9, end.tv_nsec, 1e9 + end.tv_nsec, start.tv_nsec, diff_nsec);
    }
    //printf("Actual diff sec %ld and nsec %ld\n", diff_sec, diff_nsec);

    return (1e9*diff_sec + diff_nsec)/1e9;
}
