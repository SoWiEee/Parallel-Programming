#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    // 確保執行時有傳入 1 個參數 (執行緒數量)
    assert(argc == 2);
    
    // 設定 OpenMP 要使用的執行緒數量
    omp_set_num_threads(atoi(argv[1]));
    
    // 開啟平行區塊
    #pragma omp parallel
    {
        printf("Hello, world from thread: %d.\n", omp_get_thread_num());
        printf("# of proc = %d\n", omp_get_num_procs());
        printf("# of threads = %d\n", omp_get_num_threads());
    }
    
    // 離開平行區塊後，由 Master thread (通常是 thread 0) 繼續執行
    printf("Master thread: %d\n", omp_get_thread_num());
    
    return 0;
}