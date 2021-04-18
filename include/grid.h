#pragma once

#include <cmath>
#include "utils.h"

using vec3i = Eigen::Vector3i;

using data_arr = std::vector<double>;


class Grid {
  public:
    vec3 r1;
    vec3 r2;
    uint grain;
    vec3 voxelSize;

    Grid(vec3 r1, vec3 r2, uint grain = 10) : r1(r1), r2(r2), grain(grain) {
        this->voxelSize = (r2 - r1).cwiseAbs() / grain;
    }

    bool contains(vec3 r) const {
        return this->get_voxel_index_3d(r).has_value();
    }

    std::optional<vec3i> get_voxel_index_3d(vec3& r) const;
    std::optional<int> get_voxel_index_1d(vec3& r) const;

    vec3i from_voxel_index_1d(int index) const;

    double calc_density_from_angle(double angle) const;

    std::vector<std::vector<double>> gen_data_array_1d() const;


  private:
    std::optional<uint64_t> _get_voxel_index_1(double x, double xi, double xf) const;
};
