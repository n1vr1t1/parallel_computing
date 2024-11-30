# Optimization using Implicit and Explicit Parallelism with OpenMP

Author: Nivriti Raisingh, 239132, nivriti.raisingh@studenti.unitn.it

This project explores optimization techniques for matrix transposition using implicit and explicit parallelization strategies, leveraging OpenMP. The goal is to evaluate and enhance the efficiency, scalability, and computational performance of matrix operations compared to a sequential baseline.

---

## Overview

Matrix transposition is a fundamental computational operation with applications in fields like numerical simulations, graphics processing, and linear regression. This project investigates both low-level optimizations and parallelization methods to overcome common bottlenecks such as inefficient memory access patterns and cache miss penalties. The results of different approaches are plotted and analyzed in the report.

---

## Features

- **Sequential Implementation:** Provides a baseline for performance analysis.
- **Implicit Parallelism:** Optimizes memory access patterns, reducing latency and improving arithmetic intensity.
- **Explicit Parallelism:** Implements OpenMP directives to scale matrix operations across multiple threads, effectively increasing speedup and effective bandwidth.

### Performance Metrics
- Speedup
- Scalability
- Effective memory bandwidth
- Roofline Model analysis

---

## Methodology

### Optimization Approaches
- **Implicit Parallelization:** Utilizes techniques like blocking, vectorization, and loop unrolling.
- **Explicit Parallelization:** Employs OpenMP for workload partitioning and thread management.

### Challenges Addressed
- Cache misses and memory inefficiencies.
- Load balancing and thread contention.
- Minimization of false sharing and overhead.

### Tools & Libraries
- **OpenMP, GCC Compiler:** 9.3.0 on computer and 9.1.0 on the HPC cluster
- **Standard C++ libraries:** `chrono`, `cstdlib`, `fstream`, `iostream`, `algorithm`, `omp.h`.
- **Optimization flags:** `-O2` and `-march=native` (for implicit implementaion)

---

## Experimental Setup

### Test Systems
1. **Dell Inspiron** with AMD Ryzen 5500U CPU, 6 cores, 12 threads.
2. **High-Performance Cluster** with Intel Xeon Gold 5218 CPU, 64 cores, 128 threads.

### Matrix Sizes
From `2^4` to `2^12`.

### Data Collection
Each test is run 10 times, and average results are analyzed.

---

## Results

### Implicit Parallelization
- Achieves speedups up to **4x** using techniques like blocking and vectorization.
- Reduces cache misses with optimized memory access patterns.

### Explicit Parallelization
- Scales effectively for large matrices with speedups up to **18x**.
- Demonstrates strong scalability with thread and workload management.

### Roofline Analysis
- Reveals memory-bound limitations and emphasizes the importance of cache efficiency.

---

## Conclusion

The project demonstrates the effectiveness of combining implicit and explicit parallelization techniques to optimize matrix transposition. While implicit approaches improve performance across all matrix sizes, explicit methods excel with larger datasets. The findings underscore the need for hardware-specific optimizations to approach theoretical performance limits.

---

## How to Use

### Clone the Repository
```bash
git clone https://github.com/n1vr1t1/intro_to_parallel_computing.git
```
### Go to the main folder 
## Either
### Convert from DOS to Unix endlines
```bash
dos2unix matrix_transpose.pbs
```

### Run the pbs script
```bash
qsub matrix_transpose.pbs
```
## OR
### To run an interactive session
```bash
qsub -I -q short_cpuQ -l ncpus=64
```
### Load modules
```bash
module load gcc91
module load python-3.10.14
```
### Go to code files
```bash
cd ./src
```
### Compile the Code using GCC
```bash
g++-9.1.0 -O0 -c sequential.cc
g++-9.1.0 -O2 -march=native -c implicit_optimizations.cc
g++-9.1.0 -O0 -fopenmp -c explicit_optimizations.cc
g++-9.1.0 -O0 -fopenmp -o matrix_transpose matrix_transpose.cc sequential.o implicit_optimizations.o explicit_optimizations.o
```
### Run the Executable as many times as you would like
```bash
for i in {1..n} #change n to the desired number
do
    ./matrix_transpose
done
```
### Visualize Performance Results
```bash
python3 speedup.py symmetry.csv symmetry.png 
python3 speedup.py transpose.csv transpose.png
python3 implicit_bandwidth.py
python3 explicit_bandwidth.py
```
---

## References
- **OpenMP Documentation:** https://www.openmp.org/documentation/
- **Roofline Model:** A Practical Guide to Understanding Memory-Bound Performance.
- **GCC Compiler Optimization Manual:** https://gcc.gnu.org/onlinedocs/
- References to research papers are provided in the report 
