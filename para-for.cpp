#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void printout(int n) {
    for(int i=0; i<10000; i++)
    { /* do nothing */ }
    printf("%d, ", n);
}

int main(int argc, char* argv[]) {
    double start_time = omp_get_wtime();
    #pragma omp parallel for
        for(int i=0; i<16; i++) printout(i);
    double end_time = omp_get_wtime();
    printf("Real time: %f sec\n", end_time - start_time);
}