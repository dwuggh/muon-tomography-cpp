#pragma once

#include <Eigen/Dense>
#include <cmath>

#include "grid.h"
#include "image.h"

vec3 PoCA(const MuonImage& image);

std::vector<int> get_passing_voxels(const MuonImage& image, const Grid& grid);
std::vector<int> get_passing_voxels(const MuonImage& image, const Grid& grid, vec3 poca);

/*
 * perform the entire PoCA reconstruction procedure
 */
std::vector<double> calcScatteringDensity(std::vector<MuonImage> images, const Grid& grid);

class PoCAData {
  public:
    std::vector<std::vector<double>> angles;
    std::vector<std::vector<int>> visited;

    const Grid& grid;

    int image_total = 0;
    int image_valid = 0;

    explicit PoCAData(const Grid& grid);

    std::vector<double> calcScatteringDensity() const;

    void processImage(const MuonImage& image);
};
