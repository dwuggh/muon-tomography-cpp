#pragma once

#include <armadillo>
#include <cmath>

#include "grid.h"
#include "image.h"


arma::vec3 PoCA(MuonImage& image);

arma::cube calc_scattering_density(std::vector<MuonImage> images, const Grid& grid);
