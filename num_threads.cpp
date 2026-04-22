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
        // 1. 取得當前執行緒的 Index (編號永遠從 0 開始)
        int thread_id = omp_get_thread_num();
        
        // 2. 根據知道的 Index，讓執行緒各自做不同的事情 (do things accordingly)
        if (thread_id == 0) {
            // Index 為 0 的必定是 Master Thread
            printf("Thread %d: I am the Master thread. I can assign tasks!\n", thread_id);
        } else {
            // Index 大於 0 的是 Worker Threads
            printf("Thread %d: I am a Worker thread. I am doing my job.\n", thread_id);
        }
    }
    
    return 0;
}