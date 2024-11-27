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

int main(){

    std::ofstream symmetry_out("symmetry.csv", std::ios::app);
    std::ofstream transpose_out("transpose.csv", std::ios::app);
    
    if(!(symmetry_out.is_open() && transpose_out.is_open())){
        std::cout<<"Error opening file"<<std::endl;
        return 1;
    }
    
    for (int i = 4; i < 13; ++i) {
        int n = 1 << i; // 2^4 to 2^12
        float** M = new float* [n];
        float** T =  new float* [n];
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
                M[i][j]=rand()%20;
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
        std::cout<<"Is symmetric:                     "<<sym<<std::endl;
        std::cout<<"Time taken for checking symmetry: "<<sym_time.count()<<" seconds"<<std::endl;

        symmetry_out<<sym_time.count()<<", ";


        /***************************************************************************************************\
        *                                                                                                   *
        *                                         Transposing                                               *
        *                                                                                                   *      
       \****************************************************************************************************/


        start = std::chrono::high_resolution_clock::now();
        matTranspose(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> transpose_time = end- start;
        std::cout<<"Time taken for transposing:       "<<transpose_time.count()<<" seconds"<<std::endl;

        transpose_out<<transpose_time.count()<<", ";
        
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
        sym = checkSymImp(M,n);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> sym_time_imp = end - start;
        std::cout<<"Is symmetric:                     "<<sym<<std::endl;
        std::cout<<"Time taken for checking symmetry: "<<sym_time_imp.count()<<" seconds"<<std::endl;
        
        symmetry_out<<sym_time_imp.count()<<", ";


        /***************************************************************************************************\
        *                                                                                                   *
        *                              Transposing with Implicit Parallelism                                *
        *                                                                                                   *
        \***************************************************************************************************/


        start = std::chrono::high_resolution_clock::now();
        matTransposeImp(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> transpose_time_imp =end-start;
        std::cout<<"Time taken for transposing:       "<<transpose_time_imp.count()<<" seconds"<<std::endl;

        transpose_out<<transpose_time_imp.count()<<", ";

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
        sym = checkSymOMP(M,n);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> sym_time_omp = end - start;
        std::cout<<"Is symmetric:                     "<<sym<<std::endl;
        std::cout<<"Time taken for checking symmetry: "<<sym_time_omp.count()<<" seconds"<<std::endl;
        
        symmetry_out<<sym_time_omp.count()<<std::endl;


        /***************************************************************************************************\
        *                                                                                                   *
        *                                      Transposing with OpenMP                                      *
        *                                                                                                   *
        \***************************************************************************************************/

        
        start = std::chrono::high_resolution_clock::now();
        matTransposeOMP(M,T,n);
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> transpose_time_omp = end-start;
        std::cout<<"Time taken for transposing:       "<<transpose_time_omp.count()<<" seconds"<<std::endl;
        
        transpose_out<<transpose_time_omp.count()<<std::endl;

        #endif

        /*****************************************************************************************************\
        *                                                                                                     *
        *                                Calculating Speedup and Efficiency                                   *
        *                                                                                                     *
        \*****************************************************************************************************/

        
        /*                                            Implicit                                               */
        double speedupSymImp = double(sym_time.count()) / sym_time_imp.count();
        double bandwidthSym = (2 * n * n * sizeof(float)) / sym_time_imp.count();
        double speedupTransImp = double(transpose_time.count()) / transpose_time_omp.count();
        double bandwidthTrans = (2 * n * n * sizeof(float)) / transpose_time_imp.count();
        /*                                                                                                   */

        /*                                            Explicit                                               */
        double speedupSym = double(sym_time.count()) / sym_time_omp.count();
        double efficiencySym = speedupSym / omp_get_num_procs();
        double speedupTrans = double(transpose_time.count()) / transpose_time_omp.count();
        double efficiencyTrans = speedupTrans / omp_get_num_procs();
        /*                                                                                                   */

        std::cout<<std::endl;

        std::cout << "Implicit Symmetry Check Speedup:    " << speedupSym << std::endl;
        std::cout << "Bandwidth for Symmetry Check:       "<< bandwidthSym<<std::endl;
        std::cout << "Implicit Transposition Speedup:     " << speedupTrans << std::endl;
        std::cout << "Bandwidth for Transpose:            "<< bandwidthTrans<<std::endl;

        std::cout<<std::endl;

        std::cout << "Explicit Symmetry Check Speedup:    " << efficiencySym << std::endl;
        std::cout << "Explicit Transposition Speedup:     " << efficiencyTrans << std::endl;
        std::cout << "Explicit Symmetry Check Efficiency: " << speedupSymImp << std::endl;
        std::cout << "Explicit Transposition Efficiency:  " << speedupTransImp << std::endl;

        std::cout<<std::endl;
        std::cout<<std::endl;

        //releasing memory 
        for(int i = 0; i < n; i++){
            delete[] T[i];
            delete[] M[i];
        }
        delete[] T;
        delete[] M;
    }

    return 0;
}