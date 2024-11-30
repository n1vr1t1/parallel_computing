#include<algorithm>

bool checkSymImp_ivdep_unroll(float** M, int n){
    bool sym=true;
    #pragma GCC ivdep
    for(int i=1;i<n;i++){
        #pragma GCC ivdep
        #pragma GCC unroll 8
        for(int j=0;j<i;j++){
            if(M[i][j]!=M[j][i]){
                sym=false;
            }
        }
    }
    return sym;
}
bool checkSymImp_Blocking(float** M, int n){
    bool sym=true;
    int block = 16;
    for (int i=0; i<n; i+=block) {
        for (int j=0; j<n; j+=block) {
            for (int ii=i; ii<std::min(i+block, n); ++ii) {
                for (int jj=j; jj<std::min(j+block, n); ++jj) {
                    if(M[ii][jj]!=M[jj][ii]){
                        sym=false;
                    }
                }
            }
        }
    }
    return sym;
}
//best approach
bool checkSymImp_ivdep(float** M, int n){
    bool sym=true;
    #pragma GCC ivdep
    for(int i=1;i<n;i++){
        #pragma GCC ivdep
        for(int j=0;j<i;j++){
            if(M[i][j]!=M[j][i]){
                sym=false;
            }
        }
    }
    return sym;
}
bool checkSymImp_unroll(float** M, int n){
    bool sym=true;
    for(int i=1;i<n;i++){
        #pragma GCC unroll 8
        for(int j=0;j<i;j++){
            if(M[i][j]!=M[j][i]){
                sym=false;
            }
        }
    }
    return sym;
}
//best approach
void matTransposeImp_blocking(float** M, float** T, int n){
    const int block = 16;
    for (int i=0; i<n; i+=block) {
        for (int j=0; j<n; j+=block) {
            for (int ii=i; ii<std::min(i+block, n); ++ii) {
                #pragma GCC unroll 16
                for (int jj=j; jj<std::min(j+block, n); ++jj) {
                    T[jj][ii] = M[ii][jj];
                }
            }
        }
    }
}
void matTransposeImp_ivdep(float** M, float** T, int n){
    const int unroll = 16;
    #pragma GCC ivdep
    for (int i=0; i<n; i++) {
        #pragma GCC ivdep
        for (int j=0; j<n; j++) {
            T[j][i] = M[i][j];
        }
    }
}
void matTransposeImp_unroll(float** M, float** T, int n){
    for (int i=0; i<n; i++) {
        #pragma GCC unroll 16
        for (int j=0; j<n; j++) {
            T[j][i] = M[i][j];
        }
    }
}
void matTransposeImp_unroll_ivdep(float** M, float** T, int n){
    #pragma GCC ivdep
    for (int i=0; i<n; i++) {
        #pragma GCC ivdep
        #pragma GCC unroll 16
        for (int j=0; j<n; j++) {
            T[j][i] = M[i][j];
        }
    }
}