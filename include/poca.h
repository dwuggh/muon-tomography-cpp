#pragma once

#include <armadillo>
#include <cmath>

#include "grid.h"
#include "image.h"

using vec3 = arma::vec3;

vec3 PoCA(MuonImage& image) {
    vec3 v = arma::normalise(arma::cross(image.vi, image.vf));

    double threshold = 1E-5;
    // vi // vf case
    if (arma::norm(v) < threshold) {
        return {};
    }
    arma::mat33 coffs = arma::join_rows(image.vi, image.vf, v);
    vec3 vals         = image.rf - image.ri;
    vec3 ts           = arma::solve(coffs.t(), vals);
    return image.ri + image.vi * ts[0] + v * ts[2] / 2;
}

arma::cube calc_scattering_density(std::vector<MuonImage> images, const Grid& grid) {
    auto data_size        = images.size();
    uint64_t data_invalid = 0;
    auto mat              = grid.gen_voxel_matrix();
    for (auto& image : images) {
        auto poca         = PoCA(image);
        auto _voxel_index = grid.get_voxel_index(poca);
        if (_voxel_index.has_value()) {
            auto deflection_angle = image.angle();
            auto voxel_index      = _voxel_index.value();
            mat(voxel_index[0], voxel_index[1], voxel_index[2]).emplace_back(deflection_angle);
        } else {
            data_invalid++;
            continue;
        }
    }

    uint64_t n            = data_size - data_invalid;
    arma::cube result_mat = arma::cube(grid.grain, grid.grain, grid.grain);

    for (int i = 0; i < grid.grain; i++) {
        for (int j = 0; j < grid.grain; j++) {
            for (int k = 0; k < grid.grain; k++) {
                auto voxel_data     = arma::vec(mat(i, j, k));
                auto r              = arma::sum(voxel_data * voxel_data) / n / grid.d[2];
                result_mat(i, j, k) = r;
            }
        }
    }
    return result_mat;
}
