#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <time.h>

using namespace std;

static int N = 10000;
static int TEST = 1;

void initialize_mtx(float**& mtx) {
    mtx = new float* [N];
    for (int i = 0; i < N; i++) {
        mtx[i] = new float[N];
        for (int j = 0; j < N; j++) {
            mtx[i][j] = 0;
        }
    }
}

float** generate_imatrix(float**& mtx) {

    //srand((int)time(0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mtx[i][j] = (rand() % 301) / 10;
        }
    }
    return mtx;
}

void print_mtx(float**& mtx) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << "\t" << mtx[i][j];
        }
        cout << endl;
    }
}

float* m1(float* mtx1, float** mtx2, float*& result) {
    //平凡算法
    long long freq, head, tail;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);

    for (int t = 0; t < TEST; t++) {
        for (int i = 0; i < N; i++) {
            result[i] = 0;
        }
        //测试主程序
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                result[i] += mtx1[j] * mtx2[j][i];
            }
        }

    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << (tail - head) * 1000.0 / (freq * TEST) << endl;
    return result;
}
float* m2(float* mtx1, float** mtx2, float*& result) {
    //cache优化
    long long freq, head, tail;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int t = 0; t < TEST; t++) {
        for (int i = 0; i < N; i++) {
            result[i] = 0;
        }
        //测试主程序
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i++) {
                result[i] += mtx1[j] * mtx2[j][i];
            }
        }

    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << (tail - head) * 1000.0 / (freq * TEST) << endl;
    return result;

}

float* m3_unroll(float* mtx1, float** mtx2, float*& result) {
    //循环展开
    long long freq, head, tail;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int t = 0; t < TEST; t++) {
        for (int i = 0; i < N; i++) {
            result[i] = 0;
        }
        //测试主程序
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i += 4) {
                int a,b,c,d;
                a = mtx1[j] * mtx2[j][i];
                b = mtx1[j] * mtx2[j][i + 1];
                c = mtx1[j] * mtx2[j][i + 2];
                d = mtx1[j] * mtx2[j][i + 3];
                result[i] += a;
                result[i + 1] += b;
                result[i + 2] += c;
                result[i + 3] += d;
            }
        }
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << (tail - head) * 1000.0 / (freq * TEST) << endl;
    return result;
}
float** m4_unroll(float** mtx1, float** mtx2, float**& result) {
    //循环展开
    long long freq, head, tail;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int t = 0; t < TEST; t++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                result[i][j] = 0;
            }
        }
        //测试主程序
        for (int i = 0; i < N; i++) {
            for (int j1 = 0; j1 < N; j1++) {
                for (int j2 = 0; j2 < N; j2 += 2) {//j2是真正的列
                    int a, b, c, d, e, f, g, h, k, l;
                    //result[i][j2]+=(mtx1[i][j1]*mtx2[j1][j2]);
                    a = mtx1[i][j1] * mtx2[j1][j2];
                    b = mtx1[i][j1] * mtx2[j1][j2 + 1];
                    /*c = mtx1[i][j1] * mtx2[j1][j2 + 2];
                    d = mtx1[i][j1] * mtx2[j1][j2 + 3];*/
                    //e = mtx1[i][j1] * mtx2[j1][j2 + 4];
                    //f = mtx1[i][j1] * mtx2[j1][j2 + 5];

                    result[i][j2] += a;
                    result[i][j2 + 1] += b;
                    /*result[i][j2 + 2] += c;
                    result[i][j2 + 3] += d;*/
                    //result[i][j2 + 4] += e;
                    //result[i][j2 + 5] += f;
                }
            }
        }
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "m4_time: " << (tail - head) * 1000.0 / (freq * TEST) << "ms" << endl;
    return result;
}

int main()
{
    float** mtx2;
    initialize_mtx(mtx2);
    generate_imatrix(mtx2);
    float* mtx1;
    mtx1 = new float[N];
    for (int i = 0; i < N; i++) {
        mtx1[i] = rand() % 100;
    }
    float* result;
    result = new float[N];

    result = m1(mtx1, mtx2, result);
    result = m2(mtx1, mtx2, result);
    result = m3_unroll(mtx1, mtx2, result);
    //result = m4_unroll(mtx1, mtx1, result);


    //print_mtx(mtx1);
    //cout << "---------------------------------------------------------------------------------------" << endl;
    //print_mtx(result);
}
