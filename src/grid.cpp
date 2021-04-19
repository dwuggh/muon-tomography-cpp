#include "grid.h"

std::optional<vec3i> Grid::get_voxel_index_3d(vec3& r) const {

    vec3i result;
    for (int i = 0; i < 3; i++) {
        auto index = this->_get_voxel_index_1(r[i], this->r1[i], this->r2[i]);
        if (index.has_value()) {
            result[i] = index.value();
        } else {
            return {};
        }
    };
    return result;
}

std::optional<int> Grid::get_voxel_index_1d(vec3& r) const {
    auto index_3d = this->get_voxel_index_3d(r);
    if (index_3d.has_value()) {
        auto a = index_3d.value();
        return a[0] * grain * grain + a[1] * grain + a[2];
    } else {
        return {};
    }
}

vec3i Grid::from_voxel_index_1d(int index) const {
    int x = index / (grain * grain);
    index -= grain * grain * x;
    int y = index / grain;
    index -= y * grain;
    int z = index;
    return {x, y, z};
}

std::optional<uint64_t> Grid::_get_voxel_index_1(double x, double x1, double x2) const {
    double ratio = (x - x1) / (x2 - x1);
    if (isclose(ratio, 0.)) {
        return 0;
    }
    if (isclose(ratio, 1.)) {
        return this->grain - 1;
    }
    if (ratio < 0 || ratio > 1) {
        return {};
    }
    return static_cast<uint64_t>(ratio * static_cast<double>(grain));
}

double Grid::calc_density_from_angle(double angle) const {
    auto dist = this->voxelSize[2];
    return angle * angle / dist;
}

std::vector<std::vector<double>> Grid::gen_data_array_1d() const {
    auto size = grain * grain * grain;
    return std::vector<std::vector<double>>(size);
};
