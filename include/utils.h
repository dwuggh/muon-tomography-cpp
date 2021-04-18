#pragma once

#include <cmath>
#include <eigen3/Eigen/Dense>

#define POW3(x) (x) * (x) * (x)

using vec3 = Eigen::Vector3d;

inline bool isclose(double x, double target, double threshold = 1E-6) {
    return std::abs(x - target) < threshold;
}

inline bool isclose(vec3 x, vec3 target, double threshold = 1E-6) {
    for (int i = 0; i < 3; i++) {
        bool a = isclose(x[i], target[i], threshold);
        if (!a)
            return false;
    }
    return true;
}

namespace MT {
template <typename T> T sum(std::vector<T> vec) {

    T sum_of_eles;
    for (T i : vec) {
        sum_of_eles += i;
    }
    return sum_of_eles;
}

template <typename T, typename F> T sum(std::vector<T> vec, F f) {

    T sum_of_eles;
    for (T i : vec) {
        sum_of_eles += f(i);
    }
    return sum_of_eles;
}

} // namespace MT
