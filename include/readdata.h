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


/*
 * zs are in order of:
 * 1. position of 1st plate
 * 2. position of 2nd plate
 * 3. position of 3rd plate
 * 4. position of 4th plate
 * 5. position of object's lower surface
 * 6. position of object's upper surface
*/
std::vector<MuonImage> read_data(const std::string& filename, vec6 zs);
std::vector<MuonImage> read_data(std::istream& in, vec6 zs);


std::vector<double> loadScatteringDensity(std::istream& in);
std::vector<double> loadScatteringDensity(const std::string& filename);
