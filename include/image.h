#pragma once
#include <armadillo>
#include <cmath>

class MuonImage {
  public:
    arma::vec3 ri;
    arma::vec3 rf;
    arma::vec3 vi;
    arma::vec3 vf;

    MuonImage(arma::vec3 ri, arma::vec3 rf, arma::vec3 vi, arma::vec3 vf) : ri(ri), rf(rf) {
        this->vi = arma::normalise(vi);
        this->vf = arma::normalise(vf);
    };

    double angle() const {
        auto inner_product = arma::dot(this->vi, this->vf);
        return std::acos(std::abs(inner_product));
    }
};
