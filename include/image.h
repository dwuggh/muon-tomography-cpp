#pragma once

#include <cmath>
#include <ostream>
#include <eigen3/Eigen/Dense>

using vec3 = Eigen::Vector3d;


class MuonImage {
  public:
    vec3 ri;
    vec3 rf;
    vec3 vi;
    vec3 vf;

    MuonImage(vec3 ri, vec3 rf, vec3 vi, vec3 vf) : ri(ri), rf(rf), vi(vi), vf(vf) {
        this->vi.normalize();
        this->vf.normalize();
    };

    // return radian scattering angle, between original vi and vf
    double angle() const {
        return MuonImage::angle(vi, vf);
    }

    // return radian scattering angle, with new vi and vf
    double angle(vec3 poca) const {
        auto vi = (poca - ri).normalized();
        auto vf = (rf - poca).normalized();
        return MuonImage::angle(vi, vf);
    }

    inline static double angle(vec3 vi, vec3 vf) {
        auto inner_product = vi.dot(vf);
        return std::acos(inner_product);
    }

    friend std::ostream& operator<<(std::ostream& os, const MuonImage& image) {
        os << "image" << std::endl;
        os << "ri: "<< image.ri[0] << " " << image.ri[1] << " " << image.ri[2] << "\t"
           << "rf: "<< image.rf[0] << " " << image.rf[1] << " " << image.rf[2] << "\n"
           << "vi: "<< image.vi[0] << " " << image.vi[1] << " " << image.vi[2] << "\t"
           << "vf: "<< image.vf[0] << " " << image.vf[1] << " " << image.vf[2] << std::endl;
        return os;
    }
};
