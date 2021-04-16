#include "poca.h"

using vec3 = arma::vec3;

vec3 PoCA(const MuonImage& image) {
    // vi // vf case
    double threshold = 1E-5;
    if (isclose(image.vi, image.vf, threshold)) {
        return {};
    }
    vec3 v = arma::normalise(arma::cross(image.vi, image.vf));

    arma::mat33 coffs = arma::join_rows(image.vi, image.vf, v);
    vec3 vals         = image.rf - image.ri;
    vec3 ts           = arma::solve(coffs, vals);
    return image.ri + image.vi * ts[0] + v * ts[2] / 2;
}

std::vector<int> get_passing_voxels(const MuonImage& image, const Grid& grid) {
    return get_passing_voxels(image, grid, PoCA(image));
}

std::vector<int> get_passing_voxels(const MuonImage& image, const Grid& grid, arma::vec3 poca) {
    std::vector<int> path;
    auto poca_index = grid.get_voxel_index_1d(poca).value();

    for (auto i = 0;; i++) {
        arma::vec3 point       = image.ri + image.vi * i;
        auto point_voxel_index = grid.get_voxel_index_1d(point);
        if (point_voxel_index.has_value()) {
            if (point_voxel_index.value() == poca_index) {
                break;
            } else {
                path.emplace_back(point_voxel_index.value());
            }
        }
    }

    for (auto i = 0;; i++) {
        arma::vec3 point       = image.rf - image.vf * i;
        auto point_voxel_index = grid.get_voxel_index_1d(point);
        if (point_voxel_index.has_value()) {
            if (point_voxel_index.value() == poca_index) {
                break;
            } else {
                path.emplace_back(point_voxel_index.value());
            }
        }
    }

    return path;
}

arma::cube calc_scattering_density(std::vector<MuonImage> images, const Grid& grid, field4 density_mat) {
    auto data_size        = images.size();
    uint64_t data_invalid = 0;
    auto mat              = grid.gen_voxel_matrix_3d();

    for (auto& image : images) {
        auto poca         = PoCA(image);
        auto _voxel_index = grid.get_voxel_index_3d(poca);
        if (_voxel_index.has_value()) {
            auto deflection_angle = image.angle();
            auto voxel_index      = _voxel_index.value();
            // auto density          = grid.calc_density_from_angle(deflection_angle);
            mat(voxel_index[0], voxel_index[1], voxel_index[2]).emplace_back(deflection_angle);
            // voxels that passed by
            auto path = get_passing_voxels(image, grid, poca);
            for (int voxel : path) {
                auto voxel_index = grid.from_voxel_index_1d(voxel);

                mat(voxel_index[0], voxel_index[1], voxel_index[2]).emplace_back(1.0);
            }
        } else {
            data_invalid++;
            continue;
        }
    }

    uint64_t n            = data_size - data_invalid;
    arma::cube result_mat = arma::cube(grid.grain, grid.grain, grid.grain);

    for (uint i = 0; i < grid.grain; i++) {
        for (uint j = 0; j < grid.grain; j++) {
            for (uint k = 0; k < grid.grain; k++) {
                auto voxel_data     = arma::vec(mat(i, j, k));
                auto r              = arma::sum(voxel_data * voxel_data) / n / grid.voxelSize[2];
                result_mat(i, j, k) = r;
                density_mat(i, j, k).emplace_back();
            }
        }
    }
    return result_mat;
}
