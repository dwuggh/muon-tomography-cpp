#include "grid.h"

std::optional<vec3> Grid::get_voxel_index(vec3& r) const {

    vec3 result;
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

std::optional<uint64_t> Grid::_get_voxel_index_1(double x, double xi, double xf) const {
    double ratio = (x - xi) / (xf - xi);
    if (isclose(ratio, 0.)) {
        return 0;
    }
    if (isclose(ratio, 1.)) {
        return this->grain - 1;
    }
    if (ratio < 0 || ratio > 1) {
        return {};
    }
    return static_cast<uint64_t>(std::round(ratio * this->grain));
}

arma::field<std::vector<double>> Grid::gen_voxel_matrix() const {
    return arma::field<std::vector<double>>(this->grain, this->grain, this->grain);
}
