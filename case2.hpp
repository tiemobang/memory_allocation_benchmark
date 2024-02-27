#pragma once

#include <chrono>
#include <stddef.h>
#include <random>

namespace case2 {
    // Case 1: Bottleneck in page-faults during simultaneous memory allocation
    std::chrono::high_resolution_clock::duration case2_inner(volatile char* a, size_t page_size, size_t size, size_t num_reps) {

        // Create a random number generator for uniform integers between 0 and size/page_size
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, size/page_size);

        auto start = std::chrono::high_resolution_clock::now();

        for(size_t rep = 0; rep < num_reps; ++rep){

            auto i = dis(gen);
            a[i*page_size] = i;

        }

        return std::chrono::high_resolution_clock::now() - start;
    }

    std::chrono::high_resolution_clock::duration case2_single(size_t page_size, size_t size, size_t num_reps) {
        // Allocate memory
        volatile char* a = (char*) malloc(size);

        auto duration = case2_inner(a, page_size, size, num_reps);

        free((void*)a);

        return duration;
    }

    std::chrono::high_resolution_clock::duration case2_parallel(size_t page_size, size_t size, size_t num_reps, size_t num_threads) {
        
        // Allocate memory
        volatile char* a = (char*) malloc(size);

        std::chrono::high_resolution_clock::duration duration = std::chrono::high_resolution_clock::duration::zero();    
    #pragma omp parallel num_threads(num_threads)
        {
            #pragma omp barrier
            
            auto duration_inner = case2_inner(a, page_size, size, num_reps);
            
            #pragma omp critical
            {
                if (duration_inner > duration) {
                    duration = duration_inner;
                }
            }
        }

        free((void*)a);

        return duration;
    }
}