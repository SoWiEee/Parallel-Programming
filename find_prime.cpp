#include <iostream>
#include <math.h>
#include <omp.h>

using namespace std;

// 投影片提供的質數判斷函式 (保持原樣)
bool TestForPrime(int val)
{
    // 從 3 開始檢查
    int upperlimit, factor = 3;
    // 只需檢查到該數字的平方根即可
    upperlimit = (long)(sqrtf((float)val) + 0.5f);
    
    while ((factor <= upperlimit) && (val % factor)) {
        factor++;
    }
    return (factor > upperlimit);
}

int main() 
{
    int start = 3;
    int end = 10000000;
    int gPrimesFound = 0;
    double start_time, end_time;
    int threads = 4; // 設定執行緒數量

    omp_set_num_threads(threads);
    cout << "Finding primes from " << start << " to " << end << " using " << threads << " threads." << endl;
    cout << "--------------------------------------------------" << endl;

    // 測試 1：使用 Static 排程 (負載不均)
    gPrimesFound = 1; // 預先算入數字 2 是質數
    start_time = omp_get_wtime();

    // 注意：必須加上 reduction(+:gPrimesFound) 否則會 race
    #pragma omp parallel for schedule(static) reduction(+:gPrimesFound)
    for (int i = start; i <= end; i += 2) // 只檢查奇數
    {
        if (TestForPrime(i)) {
            gPrimesFound++;
        }
    }

    end_time = omp_get_wtime();
    cout << "[Static Schedule]  Primes found: " << gPrimesFound << endl;
    cout << "Time taken: " << end_time - start_time << " seconds" << endl << endl;

    // 測試 2：使用 Dynamic 排程 (負載平衡)
    gPrimesFound = 1; // 重置計數器
    start_time = omp_get_wtime();

    // 填上投影片的解答：schedule(dynamic, 8)
    #pragma omp parallel for schedule(dynamic, 8) reduction(+:gPrimesFound)
    for (int i = start; i <= end; i += 2) 
    {
        if (TestForPrime(i)) {
            gPrimesFound++;
        }
    }

    end_time = omp_get_wtime();
    cout << "[Dynamic Schedule] Primes found: " << gPrimesFound << endl;
    cout << "Time taken: " << end_time - start_time << " seconds" << endl;
    cout << "--------------------------------------------------" << endl;

    return 0;
}