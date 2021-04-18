#pragma once

#include <cmath>
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
        auto vi = poca - ri;
        auto vf = poca - rf;
        return MuonImage::angle(vi, vf);
    }

    inline static double angle(vec3 vi, vec3 vf) {
        auto inner_product = vi.dot(vf);
        return std::acos(std::abs(inner_product));
    }
};
