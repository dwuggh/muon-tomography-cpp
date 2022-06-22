#include "poca.h"

bool is_inbox(vec3 p) {
    return (p[0] < 50 && p[0] > -50 && p[1] < 50 && p[1] > -50);
}

vec3 PoCA(const MuonImage& image) {
    // vi // vf case

    double threshold = 1E-4;
    if (isclose(image.vi, image.vf, threshold)) {
        // return {};
        return {1E6, 1E6, 1E6};
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

    // double a     = image.vi.dot(image.vi);
    // double b     = image.vi.dot(image.vf);
    // double c     = image.vf.dot(image.vf);
    // vec3 w = image.ri - image.rf;
    // double d     = image.vi.dot(w);
    // double e     = image.vf.dot(w);
    // double delta = a * c - b * b;
    // if (isclose(delta, 0)) {
    //     // std::cout << "delta close to zero: " << image;
    //     return {1E6, 1E6, 1E6};
    // }
    // double t_in  = (b * e - c * d) / delta;
    // double t_out = (a * e - b * d) / delta;
    // vec3 p_in = image.ri + t_in * image.vi;
    // vec3 p_out = image.rf + t_out * image.vf;
    // vec3 poca = (p_in + p_out) / 2;

    // if (is_inbox(p_in) && is_inbox(p_out) && is_inbox(poca)) {
    //     return poca;
    // } else {
    //     return {1E7, 1E7, 1E7};
    // }
    // // MT::print(poca, "poca: ");
    // return poca;
}


std::vector<int> PoCAData::getPassingVoxels(const MuonImage& image) const {
    return getPassingVoxels(image, PoCA(image));
}

// TODO add test
std::vector<int> PoCAData::getPassingVoxels(const MuonImage& image, vec3 poca) const {
    std::vector<int> path;
    vec3 v1 = (poca - image.ri);
    vec3 v2 = (image.rf - poca);

    auto poca_voxel = grid.get_voxel_index_1d(poca);
    if (poca_voxel.has_value()) {
        path.emplace_back(poca_voxel.value());
    } else {
        std::cout << "poca no value: " << poca << std::endl;
        // return path;
    }

    vec3 vr1 = v1.array() / grid.voxelSize.array();
    int axis = MT::max_index(vr1);
    v1       = v1 / v1[axis];
    _getPassingVoxels(path, image.ri[axis], poca[axis], grid.voxelSize[axis], image.ri, v1);
    vec3 vr2 = v2.array() / grid.voxelSize.array();
    axis     = MT::max_index(vr2);
    v2       = v2 / v2[axis];
    _getPassingVoxels(path, poca[axis], image.rf[axis], grid.voxelSize[axis], poca, v2);

    return path;
}

void PoCAData::_getPassingVoxels(std::vector<int>& path, double r1, double r2, double interval,
                                 vec3 ri, vec3 v) const {
    for (double z = r1; z <= r2; z += interval) {
        vec3 point                   = ri + v * z;
        auto point_voxel_index_maybe = grid.get_voxel_index_1d(point);
        if (point_voxel_index_maybe.has_value()) {
            auto point_voxel_index = point_voxel_index_maybe.value();
            if (std::find(path.begin(), path.end(), point_voxel_index) == path.end()) {
                path.emplace_back(point_voxel_index);
            }
        }
    }
}

std::vector<double> calcScatteringDensity(std::vector<MuonImage> images, const Grid& grid) {

    PoCAData data(grid);

    for (auto& image : images) {
        data.processImage(image);
    }

    std::cout << "total image number: " << data.image_total << " valid images: " << data.image_valid
              << std::endl;
    return data.calcScatteringDensity();
}

PoCAData::PoCAData(const Grid& grid) : grid(grid) {
    auto size     = SIZE3(grid.grain);
    this->angles  = std::vector<std::vector<double>>(size);
    this->visited = std::vector<std::vector<int>>(size);
}

void PoCAData::processImage(const MuonImage& image) {
    this->image_total++;

    auto poca              = PoCA(image);
    auto voxel_index_maybe = grid.get_voxel_index_1d(poca);
    if (voxel_index_maybe.has_value()) {
        auto deflection_angle = image.angle(poca);
        if (deflection_angle > 0.0006) {
            auto voxel_index = voxel_index_maybe.value();
            angles[voxel_index].emplace_back(deflection_angle);

            // voxels that passed by
            auto path = getPassingVoxels(image, poca);
            if (std::find(path.begin(), path.end(), voxel_index) == path.end()) {
                std::cout << "fuck" << voxel_index << std::endl;
            }
            for (int index : path) {

                this->visited[index].emplace_back(1);
            }
            image_valid++;
        }
    } else if (isclose(poca[0], 1E6, 1E-4)) {
        // parallel case
        std::cout << "parallel case" << std::endl;
        vec3 poca = (image.ri + image.rf) / 2;
        auto path = getPassingVoxels(image, poca);
        // auto voxel_index_maybe = grid.get_voxel_index_1d(poca).value();
        // auto voxel_index = voxel_index_maybe.value();
        // if (std::find(path.begin(), path.end(), voxel_index) == path.end()) {
        //     std::cout << "fuck" << voxel_index << std::endl;
        // }
        for (int index : path) {

            this->visited[index].emplace_back(1);
        }
        image_valid++;
    }
}

std::vector<double> PoCAData::calcScatteringDensity() const {

    std::vector<double> scatteringDensity(angles.size());
    int data_counter = 0;
    for (auto i = 0; i < angles.size(); i++) {

        auto depth         = grid.voxelSize[2];
        MT::Func<double> f = [depth](double angle) {
            return angle * angle / depth;
        };
        double density     = MT::sum(angles[i], f);
        int visit          = MT::sum(visited[i]);

        // std::cout << density << " " << visit << std::endl;

        if (visit == 0) {
            scatteringDensity[i] = density;
            if (isclose(density, 0)) {
                // std::cout << "density = 0: " << i << "  " << density << std::endl;
            } else {
                std::cout << "density is not 0: " << i << "  " << density << std::endl;
            }
        } else {
            scatteringDensity[i] = density / visit;
        }
        // scatteringDensity[i] = density;
        // scatteringDensity[i] = density / (visit + 1.5);
        data_counter++;
    }
    return scatteringDensity;
}
