#pragma once

#include <armadillo>
#include <cmath>

using vec3 = arma::vec3;

inline bool isclose(double x, double target, double threshold = 1E-6) {
    return std::abs(x - target) < threshold;
}

class Grid {
  public:
    vec3 r1;
    vec3 r2;
    uint grain;
    vec3 d;

    Grid(vec3 r1, vec3 r2, uint grain = 10) : r1(r1), r2(r2), grain(grain) {
        this->d = arma::abs(r2 - r1) / grain;
    }

    bool contains(vec3 r) const {
        return this->get_voxel_index(r).has_value();
    }

    std::optional<vec3> get_voxel_index(vec3& r) const;

    arma::field<std::vector<double>> gen_voxel_matrix() const;


  private:
    std::optional<uint64_t> _get_voxel_index_1(double x, double xi, double xf) const;
};
