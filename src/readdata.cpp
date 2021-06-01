#include "readdata.h"

MuonData::MuonData(vec3 ri1, vec3 ri2, vec3 rf1, vec3 rf2, double z1, double z2)
    : ri1(ri1), ri2(ri2), rf1(rf1), rf2(rf2), zi(z1), zf(z2) {}

MuonImage MuonData::get_muon_image() {
    vec3 dri  = ri2 - ri1;
    double t1 = (zi - ri1[2]) / dri[2];
    vec3 ri   = ri1 + t1 * dri;
    vec3 vi   = dri.normalized();

    vec3 drf  = rf2 - rf1;
    double t2 = (zf - rf2[2]) / drf[2];
    vec3 rf   = rf2 + t2 * drf;
    vec3 vf   = drf.normalized();
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
    while (true) {

        // std::stringstream line{buffer};
        // vec3 ri1, ri2, rf1, rf2;
        // int i;
        double x1, y1, x2, y2, x3, y3, x4, y4;
        // double p;
        // line >> i >> ri1[0] >> ri1[1] >> ri2[0] >> ri2[1] >> rf1[0] >> rf1[1] >> rf2[0] >> rf2[1] >> p;
        in >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

        if (in.good()) {
            vec3 ri1 = {x1, y1, zs[0]};
            vec3 ri2 = {x2, y2, zs[1]};
            vec3 rf1 = {x3, y3, zs[2]};
            vec3 rf2 = {x4, y4, zs[3]};
            MuonData data{ri1, ri2, rf1, rf2, zs[4], zs[5]};
            auto image = data.get_muon_image();
            // std::cout << image;
            images.emplace_back(image);
        } else {
            break;
        }



        // ri1[2] = zs[0];
        // ri2[2] = zs[1];
        // rf1[2] = zs[2];
        // rf2[2] = zs[3];

    }

    return images;
}

std::vector<MuonImage> read_data(std::istream& in, vec6 zs) {
    std::vector<MuonImage> images;
    if (!in.good()) {
        exit(1);
    }

    std::string buffer;
    while (std::getline(in, buffer)) {
        if (buffer.find_first_not_of(' ') == std::string::npos) {
            continue;
        }
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

std::vector<double> loadScatteringDensity(std::istream& in) {
    std::vector<double> density;
    double d;
    while (true) {
        in >> d;
        if (in.good()) {
            density.emplace_back(d);
        } else {
            break;
        }
    }
    return density;
}

std::vector<double> loadScatteringDensity(const std::string& filename) {
    std::ifstream in;
    in.open(filename);
    if (!in.good()) {
        exit(1);
    }

    return loadScatteringDensity(in);
}
