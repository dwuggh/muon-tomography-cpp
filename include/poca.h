#pragma once

#include <armadillo>
#include <Eigen/Dense>
#include <cmath>

#include "grid.h"
#include "image.h"


arma::vec3 PoCA(const MuonImage& image);

std::vector<int> get_passing_voxels(const MuonImage& image, const Grid& grid);
std::vector<int> get_passing_voxels(const MuonImage& image, const Grid& grid, arma::vec3 poca);

/*
 * perform the entire PoCA reconstruction procedure
*/
arma::cube calc_scattering_density(std::vector<MuonImage> images, const Grid& grid, field4 density_mat);


