#include "readdata.h"

MuonData::MuonData(vec3 ri1, vec3 ri2, vec3 rf1, vec3 rf2, double z1, double z2)
    : ri1(ri1), ri2(ri2), rf1(rf1), rf2(rf2), zi(z1), zf(z2) {}

MuonImage MuonData::get_muon_image() {
    vec3 dri = ri2 - ri1;
    double t1  = (zi - ri1[2]) / dri[2];
    vec3 ri  = ri1 + t1 * dri;
    vec3 vi = dri.normalized();

    vec3 drf = rf2 - rf1;
    double t2  = (zf - rf2[2]) / drf[2];
    vec3 rf  = rf2 + t2 * drf;
    vec3 vf  = drf.normalized();
    return MuonImage{ri, rf, vi, vf};
}

std::vector<MuonImage> read_data(const std::string& filename, vec6 zs) {
    std::ifstream in;
    std::vector<MuonImage> images;
    in.open(filename);
    if (!in.good()) {
        exit(1);
    }

    std::string buffer;
    while (std::getline(in, buffer)) {

        std::stringstream line{buffer};
        vec3 ri1, ri2, rf1, rf2;
        int i;
        double p;
        line >> i >> ri1[0] >> ri1[1] >> ri2[0] >> ri2[1] >> rf1[0] >> rf1[1] >> rf2[0] >> rf2[1] >>
            p;
        ri1[2] = zs[0];
        ri2[2] = zs[1];
        rf1[2] = zs[2];
        rf2[2] = zs[3];

        MuonData data{ri1, ri2, rf1, rf2, zs[4], zs[5]};
        auto image = data.get_muon_image();
        images.emplace_back(image);
    }

    return images;
}
