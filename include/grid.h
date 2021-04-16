#pragma once

#include <armadillo>
#include <cmath>
#include <utils.h>

using vec3 = arma::vec3;

using vec3_int = std::array<int, 3>;

using field4 = arma::field<std::vector<double>>;

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
        return this->get_voxel_index_3d(r).has_value();
    }

    std::optional<vec3_int> get_voxel_index_3d(vec3& r) const;
    std::optional<int> get_voxel_index_1d(vec3& r) const;

    vec3_int from_voxel_index_1d(int index) const;

    double calc_density_from_angle(double angle) const;

    field4 gen_voxel_matrix_3d() const;
    field4 gen_voxel_matrix_1d() const;
    arma::mat gen_voxel_matrix_1d_fixed(uint64_t data_size) const;


  private:
    std::optional<uint64_t> _get_voxel_index_1(double x, double xi, double xf) const;
};
