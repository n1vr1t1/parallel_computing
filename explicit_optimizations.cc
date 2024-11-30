#ifdef _OPENMP
#include<omp.h>
#endif
#include <algorithm>
#include<iostream>
//best method
bool checkSymOMP_reduction(float** M, int n){
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
bool checkSymOMP_shared(float** M, int n){
    bool sym=true;
    #pragma omp parallel for shared(sym)
    for(int i=0;i<n;i++){
        for(int j=0;j<i;j++){
            if(M[i][j]!=M[j][i]){
                #pragma omp atomic write
                sym=false;
            }
        }
    } 
    return sym;
}
bool checkSymOMP_Private_Variable(float** M, int n){
    bool sym=true;
    #pragma omp parallel 
    {
        bool sym_private=true;
        #pragma omp for
        for(int i=0;i<n;i++){
            for(int j=0;j<i;j++){
                if(M[i][j]!=M[j][i]){
                    sym_private=false;
                }
            }
        } 
        #pragma omp critical
        sym &= sym_private;
    }
    return sym;
}
bool checkSymOMP_SIMD(float** M, int n){
    bool sym=true;
    #pragma omp simd
    for(int i=0;i<n;i++){
        for(int j=0;j<i;j++){
            if(M[i][j]!=M[j][i]){
                sym=false;
            }
        }
    } 
    return sym;
}
void matTransposeOMP_Static_schedule(float** M, float** T, int n) {
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[j][i] = M[i][j];
        }
    }
}
void matTransposeOMP_Dynamic_Schedule(float** M, float** T, int n) {
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[j][i] = M[i][j];
        }
    }
}
//best method
void matTransposeOMP_1D_access(float** M, float** T, int n) {
    #pragma omp parallel for
        for (int i = omp_get_thread_num(); i < n; i+=omp_get_num_threads()){
            for (int j = 0; j < n; j++) {
                T[j][i] = M[i][j];
            }
        }
}
void matTransposeOMP_2D_access(float** M, float** T, int n) {
    int block = 16;
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i=0; i<n; i+=block) {
        for (int j=0; j<n; j+=block) {
            for (int ii=i; ii<std::min(i+block, n); ++ii) {
                for (int jj=j; jj<std::min(j+block, n); ++jj) {
                    T[jj][ii] = M[ii][jj];
                }
            }
        }
    }
}