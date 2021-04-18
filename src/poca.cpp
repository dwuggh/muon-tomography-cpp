#include "poca.h"

vec3 PoCA(const MuonImage& image) {
    // vi // vf case
    double threshold = 1E-5;
    if (isclose(image.vi, image.vf, threshold)) {
        return {};
    }
    vec3 v = image.vi.cross(image.vf).normalized();

    // Eigen::Matrix3d coffs = arma::join_rows(image.vi, image.vf, v);
    Eigen::Matrix3d coffs;
    coffs.col(0) = image.vi;
    coffs.col(1) = image.vf;
    coffs.col(2) = v;

    vec3 vals = image.rf - image.ri;
    vec3 ts   = coffs.colPivHouseholderQr().solve(vals);
    return image.ri + image.vi * ts[0] + v * ts[2] / 2;
}

std::vector<int> get_passing_voxels(const MuonImage& image, const Grid& grid) {
    return get_passing_voxels(image, grid, PoCA(image));
}

// TODO add test
std::vector<int> get_passing_voxels(const MuonImage& image, const Grid& grid, vec3 poca) {
    std::vector<int> path;
    auto poca_index = grid.get_voxel_index_1d(poca).value();
    vec3 v1         = (poca - image.ri).normalized();
    vec3 v2         = (image.rf - poca).normalized();

    auto depth = grid.voxelSize[2];

    for (auto i = 0;; i++) {
        vec3 point             = (image.ri + v1 * i * depth).array() + depth / 2;
        auto point_voxel_index = grid.get_voxel_index_1d(point);
        if (point_voxel_index.has_value()) {
            if (point_voxel_index.value() == poca_index) {
                path.emplace_back(point_voxel_index.value());
                break;
            } else {
                path.emplace_back(point_voxel_index.value());
            }
        }
    }

    for (auto i = 0;; i++) {
        vec3 point             = (image.rf - v2 * i * depth).array() - depth / 2;
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

std::vector<double> calcScatteringDensity(std::vector<MuonImage> images, const Grid& grid) {

    PoCAData data(grid);

    for (auto& image : images) {
        data.processImage(image);
    }

    return data.calcScatteringDensity();
}

PoCAData::PoCAData(const Grid& grid) : grid(grid) {
    auto size     = POW3(grid.grain);
    this->angles  = std::vector<std::vector<double>>(size);
    this->visited = std::vector<std::vector<int>>(size);
}

void PoCAData::processImage(const MuonImage& image) {
    this->image_total++;

    auto poca              = PoCA(image);
    auto voxel_index_maybe = grid.get_voxel_index_1d(poca);
    if (voxel_index_maybe.has_value()) {
        auto deflection_angle = image.angle(poca);
        auto voxel_index      = voxel_index_maybe.value();
        angles[voxel_index].emplace_back(deflection_angle);

        // voxels that passed by
        auto path = get_passing_voxels(image, grid, poca);
        for (int voxel_index : path) {

            this->visited[voxel_index].emplace_back(1);
        }
        image_valid++;
    } else {
        // invalid data, do nothing
    }
}

std::vector<double> PoCAData::calcScatteringDensity() const {

    std::vector<double> scatteringDensity(image_valid);
    int data_counter = 0;
    for (auto i = 0; i < angles.size(); i++) {

        auto depth     = grid.voxelSize[2];
        auto f         = [depth](double angle) { return angle * angle / depth; };
        double density = MT::sum(angles[i], f);
        int visit      = MT::sum(visited[i]);

        scatteringDensity[data_counter] = density / visit;
        data_counter++;
    }
    return scatteringDensity;
}
