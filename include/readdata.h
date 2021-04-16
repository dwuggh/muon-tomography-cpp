#pragma once

#include <armadillo>
#include <iostream>
#include <string>
#include <vector>


#include "image.h"

using vec4 = arma::vec4;
using vec3 = arma::vec3;


class MuonData {
 public:
    vec3 ri1;
    vec3 ri2;

    vec3 rf1;
    vec3 rf2;

    double zi, zf;

    MuonData(vec3, vec3, vec3, vec3, double, double);

    MuonImage get_muon_image();
};


std::vector<MuonImage> read_data(const std::string& filename, arma::vec6 zs);
