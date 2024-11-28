#ifndef _EXPLICIT_OPTIMIZATIONS_H_
#define _EXPLICIT_OPTIMIZATIONS_H_
//checkSymOMP
bool checkSymOMP_reduction(float** M, int n);
bool checkSymOMP_shared(float** M, int n);
bool checkSymOMP_Private_Variable(float** M, int n);
bool checkSymOMP_SIMD(float** M, int n);

//matTransposeOMP
void matTransposeOMP_schedule(float** M, float** T, int n);
void matTransposeOMP_Dynamic_Schedule(float** M, float** T, int n);
void matTransposeOMP_1D_access(float** M, float** T, int n);
void matTransposeOMP_2D_access(float** M, float** T, int n);

#endif