#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <Windows.h>

int main (int argc, char *argv[])
{
    // ex: ./for 4 20 (表示開 4 個執行緒，跑 20 次迴圈)
    assert(argc == 3);
    
    omp_set_num_threads(atoi(argv[1]));
    int n = atoi(argv[2]);
    
    printf("# of proc = %d\n", omp_get_num_procs());
    printf("# of loop iterations = %d\n", n);
    
    // 開啟平行迴圈區塊
    #pragma omp parallel for
    for(int i = 0; i < n; i ++) {
        // 印出：哪個 thread、負責哪個 loop index、在系統哪個 core 上跑
        printf("thread %d runs index %d in core %lu.\n",
               omp_get_thread_num(), i, GetCurrentProcessorNumber());
    }
    
    // 程式執行到這裡之前，所有執行緒會自動等待彼此 (自動同步)
    return 0;
}