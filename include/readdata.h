#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include "image.h"


typedef Eigen::Matrix<double, 6, 1> vec6;

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


std::vector<MuonImage> read_data(const std::string& filename, vec6 zs);
