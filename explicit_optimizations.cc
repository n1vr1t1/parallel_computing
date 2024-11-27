#ifdef _OPENMP
#include<omp.h>
#endif
#include <algorithm>
#include<iostream>
//n={16,256)->1 threads, n={257,512}->8 threads, n={513,2048}->32 threads, n>2048->128 threads
bool checkSymOMP(float** M, int n){
    bool sym=true;
    int threads = n > 2048 ? 128 : (n>512 ? 32 : (n>256 ? 8 : 1));
    omp_set_num_threads(threads);
    #pragma omp parallel for reduction(&:sym)
    for(int i=0;i<n;i++){
        for(int j=0;j<i;j++){
            if(M[i][j]!=M[j][i]){
                sym=false;
            }
        }
    } 
    return sym;
}
\\(16<n<128)->1 threads, n=256->4 threads, n=512->16 threads, n=1024->32 threads, n>1024->128 threads
void matTransposeOMP(float** M, float** T, int n) {
    int threads = n > 1024 ? 128 : (n>512 ? 32 : (n > 256 ? 16 : (n > 128 ? 4 :1)));
    omp_set_num_threads(threads);
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[j][i] = M[i][j];
        }
    }
}