//optimize the code using -O2 
#include<algorithm>

bool checkSymImp(float** M, int n){
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
void matTransposeImp(float** M, float** T, int n){
    const int unroll = 16;
    for (int i=0; i<n; i+=unroll) {
        for (int j=0; j<n; j+=unroll) {
            for (int ii=i; ii<std::min(i+unroll, n); ++ii) {
                #pragma GCC unroll 16
                for (int jj=j; jj<std::min(j+unroll, n); ++jj) {
                    T[jj][ii] = M[ii][jj];
                }
            }
        }
    }
}