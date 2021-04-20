#pragma once

#include <cmath>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <functional>

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

template <typename T>
using Func = std::function<T(T)>;


template <typename T>
T sum(const std::vector<T>& vec) {

    T sum_of_eles = 0;
    for (T i : vec) {
        sum_of_eles += i;
    }
    return sum_of_eles;
}

template <typename T>
T sum(const std::vector<T>& vec, Func<T> f) {

    T sum_of_eles = 0;
    for (auto i : vec) {
        sum_of_eles += f(i);
    }
    return sum_of_eles;
}

template <typename T, int _Rows>
int min_index(const Eigen::Matrix<T, _Rows, 1> vec) {
    auto min_index = 0;
    T minval       = vec[0];
    for (int i = 1; i < _Rows; i++) {
        if (minval > vec[i]) {
            minval    = vec[i];
            min_index = i;
        }
    }
    return min_index;
}

template <typename T, int _Rows>
int max_index(const Eigen::Matrix<T, _Rows, 1> vec) {
    auto max_index = 0;
    T maxval       = vec[0];
    for (int i = 1; i < _Rows; i++) {
        if (maxval < vec[i]) {
            maxval    = vec[i];
            max_index = i;
        }
    }
    return max_index;
}

inline void print(vec3 v) { std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl; }
inline void print(vec3 v, const char* str) {
    std::cout << str << v[0] << " " << v[1] << " " << v[2] << std::endl;
}

} // namespace MT
