#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
void printout(int n) {
    for(int i=0; i<10000; i++)
    { /* do nothing */ }
    printf("%d, ",n);
}

int main(int argc, char* argv[]) {
    #pragma omp parallel for
    for(int i=0; i<16; i++) printout(i);
    return 0;
}