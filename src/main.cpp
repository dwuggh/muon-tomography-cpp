#include <armadillo>
#include <vector>

#include "grid.h"
#include "image.h"
#include "muontomography.h"
#include "poca.h"

int main(int argc, char** argv) {
    // vec3 a = {1, 2, 3};
    // vec3 b = {4, 5, 6};
    // vec3 c = {7, 8, 9};
    // arma::mat33 mat = arma::join_rows(a, b, c);
    // mat.print();
    Grid grid(vec3{0.0, 0.0, 0.0}, vec3{10.0, 10.0, 10.0});
    grid.gen_voxel_matrix();
    // calc_scattering_density({}, grid);
    auto scattering_density = arma::cube(10, 10, 10);
    scattering_density.randu();
    scattering_density = scattering_density / scattering_density.max();
    scattering_density.print();
    MuonTomography app({argc, argv}, grid, scattering_density);
    return app.exec();
}
