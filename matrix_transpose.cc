#include "sequential.h"
#include "implicit_optimizations.h"
#include "explicit_optimizations.h"

#include<iostream>
#include<chrono>
#include<cstdlib>
#include<fstream>

#ifdef _OPENMP
#include<omp.h>
#endif
void check_transpose(float** M, float** T, int n){
    #pragma omp parallel for collapse(2)
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if (M[i][j]!=T[j][i]){
                std::cout<<"Error in transpose"<<std::endl;
            }
        }
    }
}
int main(){

    std::ofstream symmetry_out("symmetry.csv", std::ios::app);
    std::ofstream transpose_out("transpose.csv", std::ios::app);
    std::ofstream bandwidth_out("bandwidth.csv", std::ios::app);
    if(!(symmetry_out.is_open() && transpose_out.is_open())){
        std::cout<<"Error opening file"<<std::endl;
        return 1;
    }
    
    for (int i = 4; i < 13; i++) {
        int n = 1 << i; // 2^4 to 2^12
        float** M = new float* [n];
        float** T =  new float* [n];

        double data_size = 2 * n * n * sizeof(float);
        if (M==NULL || T==NULL){
            std::cout<<"Memory allocation failed"<<std::endl;
            return 1;
        }

        //initialzing matrix with random numbers
        for (int i=0;i<n;i++){
            M[i] = new float[n];
            T[i] = new float[n];
            if (M[i]==NULL || T[i]==NULL){
                std::cout<<"Memory allocation failed"<<std::endl;
                return 1;
            }
            for (int j=0;j<n;j++){
                M[i][j]=rand()%100;
            }
        }
        /***************************************************************************************************\
        *                                                                                                   *
        *                                          Sequential/Naive                                         *
        *                                                                                                   *
        \***************************************************************************************************/
        
        std::cout<<std::endl;
        std::cout<<"====================== Sequential method ======================"<<std::endl;
        std::cout<<std::endl;


        /***************************************************************************************************\
        *                                                                                                   *
        *                                         Checking symmetry                                         *
        *                                                                                                   *
        \***************************************************************************************************/


        auto start = std::chrono::high_resolution_clock::now();
        bool sym = checkSym(M,n);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> sym_time = end - start;

        std::cout<<"Time taken for checking symmetry: "<<sym_time.count()<<" seconds"<<std::endl;


        /****************************************************************************************************\
        *                                                                                                    *
        *                                           Transposing                                              *
        *                                                                                                    *      
        \****************************************************************************************************/


        start = std::chrono::high_resolution_clock::now();
        matTranspose(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        check_transpose(M,T,n);
        std::chrono::duration<double> transpose_time = end - start;
        std::cout<<"Time taken for transposing:       "<<transpose_time.count()<<" seconds"<<std::endl;

        bandwidth_out<<data_size / transpose_time.count()<<", ";
        
        /***************************************************************************************************\
        *                                                                                                   *
        *                                     Implicit Parallelism                                          *
        *                                                                                                   *
        \***************************************************************************************************/

        std::cout<<std::endl;
        std::cout<<"====================== Implicit parallelism ======================"<<std::endl;
        std::cout<<std::endl;

        /**************************************************************************************************\
        *                                                                                                  *
        *                           Checking Symmetry with Implicit Parallelism                            *
        *                                                                                                  *
        \**************************************************************************************************/

        start = std::chrono::high_resolution_clock::now();
        bool sym_imp = checkSymImp_ivdep(M,n);
        end = std::chrono::high_resolution_clock::now();
        if(sym!=sym_imp){
            std::cout<<"Error in checkSymImp"<<std::endl;
        }
        std::chrono::duration<double> sym_time_imp = end - start;
        std::cout<<"Time taken for checkImp method 1: "<<sym_time_imp.count()<<" seconds"<<std::endl;

        symmetry_out<<sym_time.count() / sym_time_imp.count()<<", ";
        
        start = std::chrono::high_resolution_clock::now();
        sym_imp = checkSymImp_Blocking(M,n);
        if(sym!=sym_imp){
            std::cout<<"Error in checkSymImp"<<std::endl;
        }
        end = std::chrono::high_resolution_clock::now();
        sym_time_imp = end - start;
        std::cout<<"Time taken for checkImp method 2: "<<sym_time_imp.count()<<" seconds"<<std::endl;
        
        symmetry_out<<sym_time.count() / sym_time_imp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        sym = checkSymImp_ivdep_unroll(M,n);
        end = std::chrono::high_resolution_clock::now();
        if(sym!=sym_imp){
            std::cout<<"Error in checkSymImp"<<std::endl;
        }
        sym_time_imp = end - start;
        std::cout<<"Time taken for checkImp method 3: "<<sym_time_imp.count()<<" seconds"<<std::endl;
        

        symmetry_out<<sym_time.count() / sym_time_imp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        sym = checkSymImp_unroll(M,n);
        end = std::chrono::high_resolution_clock::now();
        if(sym!=sym_imp){
            std::cout<<"Error in checkSymImp"<<std::endl;
        }
        sym_time_imp = end - start;
        std::cout<<"Time taken for checkImp method 4: "<<sym_time_imp.count()<<" seconds"<<std::endl;
        

        symmetry_out<<sym_time.count() / sym_time_imp.count()<<", ";


        /***************************************************************************************************\
        *                                                                                                   *
        *                              Transposing with Implicit Parallelism                                *
        *                                                                                                   *
        \***************************************************************************************************/


        start = std::chrono::high_resolution_clock::now();
        matTransposeImp_blocking(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        check_transpose(M,T,n);
        std::chrono::duration<double> transpose_time_imp = end-start;
        std::cout<<"Time taken for matTransposeImp method 1: "<<transpose_time_imp.count()<<" seconds"<<std::endl;

        transpose_out<<transpose_time.count() / transpose_time_imp.count()<<", ";
        bandwidth_out<<data_size / transpose_time_imp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        matTransposeImp_ivdep(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        check_transpose(M,T,n);
        transpose_time_imp = end-start;
        std::cout<<"Time taken for matTransposeImp method 2: "<<transpose_time_imp.count()<<" seconds"<<std::endl;

        transpose_out<<transpose_time.count() / transpose_time_imp.count()<<", ";
        bandwidth_out<<data_size / transpose_time_imp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        matTransposeImp_unroll(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        check_transpose(M,T,n);
        transpose_time_imp = end - start;
        std::cout<<"Time taken for matTransposeImp method 3: "<<transpose_time_imp.count()<<" seconds"<<std::endl;

        transpose_out<<transpose_time.count() / transpose_time_imp.count()<<", ";
        bandwidth_out<<data_size / transpose_time_imp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        matTransposeImp_unroll_ivdep(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        check_transpose(M,T,n);
        transpose_time_imp = end - start;
        std::cout<<"Time taken for matTransposeImp method 4: "<<transpose_time_imp.count()<<" seconds"<<std::endl;

        transpose_out<<transpose_time.count() / transpose_time_imp.count()<<", ";
        bandwidth_out<<data_size / transpose_time_imp.count()<<", ";


        /***************************************************************************************************\
        *                                                                                                   *
        *                                    OpenMP (Explicit Paralelism)                                   *
        *                                                                                                   *
        \***************************************************************************************************/
        
        #ifdef _OPENMP
        std::cout<<std::endl;
        std::cout<<"====================== Explicit parallelism ======================"<<std::endl;
        std::cout<<std::endl;


        /***************************************************************************************************\
        *                                                                                                   *
        *                                   Checking Symmetry with OpenMP                                   *
        *                                                                                                   *
        \***************************************************************************************************/


        start = std::chrono::high_resolution_clock::now();
        bool sym_omp = checkSymOMP_Private_Variable(M,n);
        end = std::chrono::high_resolution_clock::now();
        if(sym!=sym_omp){
            std::cout<<"Error in checkSymOMP"<<std::endl;
        }
        std::chrono::duration<double> sym_time_omp = end - start;
        std::cout<<"Time taken for checkSymOMP method 1: "<<sym_time_omp.count()<<" seconds"<<std::endl;
                
        symmetry_out<<sym_time.count() / sym_time_omp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        sym_omp = checkSymOMP_shared(M,n);
        end = std::chrono::high_resolution_clock::now();
        if(sym!=sym_omp){
            std::cout<<"Error in checkSymOMP"<<std::endl;
        }
        sym_time_omp = end - start;
        std::cout<<"Time taken for checkSymOMP method 2: "<<sym_time_omp.count()<<" seconds"<<std::endl;
        
        symmetry_out<<sym_time.count() / sym_time_omp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        sym_omp = checkSymOMP_reduction(M,n);
        end = std::chrono::high_resolution_clock::now();
        if(sym!=sym_omp){
            std::cout<<"Error in checkSymOMP"<<std::endl;
        }
        sym_time_omp = end - start;
        std::cout<<"Time taken for checkSymOMP method 3: "<<sym_time_omp.count()<<" seconds"<<std::endl;
        symmetry_out<<"8"<<std::endl;
        symmetry_out<<sym_time.count() / sym_time_omp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        sym_omp = checkSymOMP_SIMD(M,n);
        end = std::chrono::high_resolution_clock::now();
        if(sym!=sym_omp){
            std::cout<<"Error in checkSymOMP"<<std::endl;
        }
        sym_time_omp = end - start;
        std::cout<<"Time taken for checkSymOMP method 4: "<<sym_time_omp.count()<<" seconds"<<std::endl;
        
        symmetry_out<<sym_time.count() / sym_time_omp.count()<<std::endl;


        /***************************************************************************************************\
        *                                                                                                   *
        *                                      Transposing with OpenMP                                      *
        *                                                                                                   *
        \***************************************************************************************************/

        
        start = std::chrono::high_resolution_clock::now();
        matTransposeOMP_schedule(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> transpose_time_omp = end-start;
        std::cout<<"Time taken for matTranspose method 1: "<<transpose_time_omp.count()<<" seconds"<<std::endl;
        
        transpose_out<<transpose_time.count() / transpose_time_omp.count()<<", ";
        bandwidth_out<<data_size / transpose_time_omp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        matTransposeOMP_Dynamic_Schedule(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        transpose_time_omp = end-start;
        std::cout<<"Time taken for matTranspose method 2:"<<transpose_time_omp.count()<<" seconds"<<std::endl;
        
        transpose_out<<transpose_time.count() / transpose_time_omp.count()<<", ";
        bandwidth_out<<data_size / transpose_time_omp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        matTransposeOMP_1D_access(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        transpose_time_omp = end-start;
        std::cout<<"Time taken for matTranspose method 3:"<<transpose_time_omp.count()<<" seconds"<<std::endl;
        
        transpose_out<<transpose_time.count() / transpose_time_omp.count()<<", ";
        bandwidth_out<<data_size / transpose_time_omp.count()<<", ";

        start = std::chrono::high_resolution_clock::now();
        matTransposeOMP_2D_access(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        transpose_time_omp = end-start;
        std::cout<<"Time taken for matTranspose method 4:"<<transpose_time_omp.count()<<" seconds"<<std::endl;
        
        transpose_out<<transpose_time.count() / transpose_time_omp.count()<<std::endl;
        bandwidth_out<<data_size / transpose_time_omp.count()<<std::endl;

        #endif

        //releasing memory 
        for(int i = 0; i < n; i++){
            delete[] T[i];
            delete[] M[i];
        }
        delete[] T;
        delete[] M;
    }

    symmetry_out.close();
    transpose_out.close();

    return 0;
}