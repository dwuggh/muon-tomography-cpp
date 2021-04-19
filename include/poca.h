#pragma once

#include <eigen3/Eigen/Dense>
#include <cmath>
#include <iostream>

#include "grid.h"
#include "image.h"

vec3 PoCA(const MuonImage& image);

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
    std::vector<int> getPassingVoxels(const MuonImage& image) const;
    std::vector<int> getPassingVoxels(const MuonImage& image, vec3 poca) const;

    void processImage(const MuonImage& image);

  private:
    void _getPassingVoxels(std::vector<int>& path, double r1, double r2, double interval, vec3 ri,
                           vec3 v) const;
};
