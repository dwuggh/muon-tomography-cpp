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

std::vector<int> PoCAData::getPassingVoxels(const MuonImage& image) const {
    return getPassingVoxels(image, PoCA(image));
}

// TODO add test
std::vector<int> PoCAData::getPassingVoxels(const MuonImage& image, vec3 poca) const {
    std::vector<int> path;
    vec3 v1 = (poca - image.ri);
    vec3 v2 = (image.rf - poca);

    vec3 vr1 = v1.array() / grid.voxelSize.array();
    int axis = MT::max_index(vr1);
    v1 = v1 / v1[axis];
    _getPassingVoxels(path, image.ri[axis], poca[axis], grid.voxelSize[axis], image.ri, v1);
    vec3 vr2 = v2.array() / grid.voxelSize.array();
    axis     = MT::max_index(vr2);
    v2 = v2 / v2[axis];
    _getPassingVoxels(path, poca[axis], image.rf[axis], grid.voxelSize[axis], poca, v2);

    return path;
}

void PoCAData::_getPassingVoxels(std::vector<int>& path, double r1, double r2, double interval,
                                 vec3 ri, vec3 v) const {
    for (double z = r1; z <= r2; z += interval) {
        vec3 point = ri + v * z;
        auto point_voxel_index_maybe = grid.get_voxel_index_1d(point);
        if (point_voxel_index_maybe.has_value()) {
            auto point_voxel_index = point_voxel_index_maybe.value();
            if (std::find(path.begin(), path.end(), point_voxel_index) == path.end()) {
                path.emplace_back(point_voxel_index_maybe.value());
            }
        }
    }
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
        auto path = getPassingVoxels(image, poca);
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
