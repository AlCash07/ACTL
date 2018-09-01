#pragma once

#include <chrono>

namespace cp {

class stopwatch {
public:
    void start() { start_ = clock_type::now(); }

    double seconds() const {
        auto duration = clock_type::now() - start_;
        return start_ != clock_type::time_point() 
                ? std::chrono::duration_cast<std::chrono::duration<double>>(duration).count() : 0.0;
    }

private:
    using clock_type = std::chrono::high_resolution_clock;

    clock_type::time_point start_;
};

}  // nanmespace cp