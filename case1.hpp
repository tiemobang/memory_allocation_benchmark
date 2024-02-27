#pragma once

#include <chrono>
#include <stddef.h>

namespace case1 {
    // Case 1: Bottleneck in page-faults during simultaneous memory allocation
    std::chrono::high_resolution_clock::duration case1_inner(size_t size, size_t num_reps) {
        auto start = std::chrono::high_resolution_clock::now();

        for(size_t rep = 0; rep < num_reps; ++rep){
            // Allocate memory
            volatile int* a = (int*)malloc(size * sizeof(int));

            // Access the first element of each page
            size_t page_size = 4096;
            for (size_t i = 0; i < size; i += page_size / sizeof(int)) {
                
                a[i] = 0;
            }

            free((void*)a);
        }

        return std::chrono::high_resolution_clock::now() - start;
    }

    std::chrono::high_resolution_clock::duration case1_parallel(size_t size, size_t num_reps, size_t num_threads) {
        // Parallelize the case1_inner function

        std::chrono::high_resolution_clock::duration duration = std::chrono::high_resolution_clock::duration::zero();    
    #pragma omp parallel num_threads(num_threads)
        {
            #pragma omp barrier
            
            auto duration_inner = case1_inner(size, num_reps);
            
            #pragma omp critical
            {
                if (duration_inner > duration) {
                    duration = duration_inner;
                }
            }
        }

        return duration;
    }
}