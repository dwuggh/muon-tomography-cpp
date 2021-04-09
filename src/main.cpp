#include <armadillo>
#include <vector>

#include "grid.h"
#include "image.h"
#include "muontomography.h"
#include "poca.h"

int main(int argc, char** argv) {
    Grid grid(vec3{0.0, 0.0, 0.0}, vec3{10.0, 10.0, 10.0});
    grid.gen_voxel_matrix();
    auto scattering_density = arma::cube(10, 10, 10);
    scattering_density.randu();
    scattering_density = scattering_density / scattering_density.max();
    MuonTomography app({argc, argv}, grid, scattering_density);
    return app.exec();
}
