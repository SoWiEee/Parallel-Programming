#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main() 
{
    int dim = 1000;      // 矩陣大小：1000 x 1000
    int threads = 12;     // 設定 OpenMP 執行緒數量
    
    // 動態配置記憶體，並預設給予初始值
    vector<vector<double>> a(dim, vector<double>(dim, 1.0));
    vector<vector<double>> b(dim, vector<double>(dim, 2.0));
    
    // 三種方法各自的結果矩陣 (皆初始化為 0.0)
    vector<vector<double>> c_seq(dim, vector<double>(dim, 0.0));
    vector<vector<double>> c_par_inner(dim, vector<double>(dim, 0.0));
    vector<vector<double>> c_par_outer(dim, vector<double>(dim, 0.0));

    double start_time, end_time;
    double time_seq, time_par_inner, time_par_outer;

    omp_set_num_threads(threads);
    cout << "Matrix size : " << dim << " x " << dim << endl;
    cout << "Threads     : " << threads << endl;
    cout << "==================================================" << endl;

    // ==========================================
    // 方法 1：單核循序 內積相乘 (Sequential Inner-Product)
    // ==========================================
    start_time = omp_get_wtime();
    
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                c_seq[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    
    end_time = omp_get_wtime();
    time_seq = end_time - start_time;
    cout << "[Method 1] Seq Inner-Product Time: " << time_seq << " s" << endl;

    // ==========================================
    // 方法 2：多核平行 內積相乘 (Parallel Inner-Product)
    // 傳統的 i-j-k 迴圈，將最外層 i 迴圈平行化
    // ==========================================
    start_time = omp_get_wtime();
    
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                c_par_inner[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    
    end_time = omp_get_wtime();
    time_par_inner = end_time - start_time;
    cout << "[Method 2] Par Inner-Product Time: " << time_par_inner << " s" << endl;

    // ==========================================
    // 方法 3：多核平行 外積相乘 (Parallel Outer-Product)
    // k-i-j 迴圈 (Cache Friendly)，並將內部的 i 迴圈平行化
    // ==========================================
    start_time = omp_get_wtime();
    
    #pragma omp parallel 
    {
        // 每個執行緒都擁有自己的 k，並且同步推進 (因為 #pragma omp for 結尾有隱式屏障)
        for (int k = 0; k < dim; k++) {
            
            // 將 i 迴圈打散給不同執行緒，j 迴圈則享受連續記憶體存取
            #pragma omp for schedule(static)
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    c_par_outer[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }
    
    end_time = omp_get_wtime();
    time_par_outer = end_time - start_time;
    cout << "[Method 3] Par Outer-Product Time: " << time_par_outer << " s" << endl;

    // ==========================================
    // 效能比較總結 (Speedup)
    // ==========================================
    cout << "==================================================" << endl;
    cout << "Performance Summary (Speedup vs Sequential):" << endl;
    if (time_par_inner > 0) 
        cout << "- Par Inner-Product is " << time_seq / time_par_inner << "x faster." << endl;
    if (time_par_outer > 0) 
        cout << "- Par Outer-Product is " << time_seq / time_par_outer << "x faster." << endl;
    
    if (time_par_inner > 0 && time_par_outer > 0) {
        cout << "\nAlgorithm & Cache Bonus:" << endl;
        cout << "- Outer-Product is " << time_par_inner / time_par_outer 
             << "x faster than Inner-Product (under same " << threads << " threads)!" << endl;
    }

    return 0;
}