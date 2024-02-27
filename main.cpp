#include <stddef.h>
#include <string>
#include <iostream>
#include <chrono>

#include "case1.hpp"
#include "case2.hpp"

int main(int argc, char* argv[]) {
    
    int num_threads = 20;
    size_t size = 1024 * 1024 * 64; // 1 GB
    size_t page_size = 1;
    size_t num_reps_c1 = 128;
    size_t num_reps_c2 = 1024*1024*1024;
    
    int choice = -1;
    if (argc > 1) {
        choice = std::stoi(argv[1]);
    }

    std::chrono::high_resolution_clock::duration duration;

    switch (choice)
    {
    case 0:
        std::cout << "Running page-fault case (single-threaded)\n";
        duration = case1::case1_inner(size, num_reps_c1);
        duration /= num_reps_c1;
        break;
    case 1:
        std::cout << "Running page-fault case (" << num_threads << "-threaded)\n";
        duration = case1::case1_parallel(size, num_reps_c1, num_threads);
        duration /= num_reps_c1;
        break;
    case 2:
        std::cout << "Running random read case (single-threaded)\n";
        duration = case2::case2_single(page_size, size*num_threads, num_reps_c2);
        duration /= num_reps_c2;
        break;
    case 3:
        std::cout << "Running random read case (" << num_threads << "-threaded)\n";
        duration = case2::case2_parallel(page_size, size*num_threads, num_reps_c2, num_threads);
        duration /= num_reps_c2;
        break;
    
    default:
        std::cerr << "Invalid choice" << std::endl;
    }

    std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() << " ns" << std::endl;

    return 0;
}
