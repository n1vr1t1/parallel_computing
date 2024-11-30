#ifndef IMPLICIT_OPTIMIZATIONS_H
#define IMPLICIT_OPTIMIZATIONS_H
//checkSymImp
bool checkSymImp_ivdep_unroll(float** M, int n);
bool checkSymImp_Blocking(float** M, int n);
bool checkSymImp_ivdep(float** M, int n);
bool checkSymImp_unroll(float** M, int n);

//matTransposeImp
void matTransposeImp_blocking(float** M, float** T, int n);
void matTransposeImp_ivdep(float** M, float** T, int n);
void matTransposeImp_unroll(float** M, float** T, int n);
void matTransposeImp_unroll_ivdep(float** M, float** T, int n);

#endif