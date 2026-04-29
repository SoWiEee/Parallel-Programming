#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <windows.h>

#define THREADS 8
#define N 1024

int main() {
    int a[10000], b[10000];
    double start_time, end_time;

    // 初始化陣列
    // for(int i=0; i<N; i++) b[i] = i;

    printf("Starting parallel loop with %d threads...\n", THREADS);
    
    // 開始計時
    start_time = omp_get_wtime();

    // 💡 測試重點：請嘗試把 static 改成 dynamic 或 guided 看看時間變化
    #pragma omp parallel for schedule(dynamic, 3) num_threads(THREADS)
    for (int i = 0; i < N; i++) {
        Sleep(i);
        a[i] = b[i] * 5 - 100;
        // printf("Thread %d doing iteration %d.\n", omp_get_thread_num(), i);
    }

    end_time = omp_get_wtime();

    printf("All done!\n");
    printf("Total Execution Time: %f seconds\n", end_time - start_time);
    
    return 0;
}