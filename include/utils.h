#pragma once

#include <cmath>
#include <armadillo>


inline bool isclose(double x, double target, double threshold = 1E-6) {
    return std::abs(x - target) < threshold;
}

inline bool isclose(arma::vec3 x, arma::vec3 target, double threshold = 1E-6) {
    for (int i = 0; i < 3; i++) {
        bool a = isclose(x[i], target[i], threshold);
        if (!a) return false;
    }
    return true;
}
