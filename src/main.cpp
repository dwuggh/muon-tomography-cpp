#include <Corrade/Utility/Arguments.h>
#include <iostream>
#include <vector>

#include "grid.h"
#include "image.h"
#include "muontomography.h"
#include "poca.h"
#include "readdata.h"
#include <random>

int main(int argc, char** argv) {

    // Grid grid{vec3{0.0, 0.0, 0.0}, vec3{10.0, 10.0, 10.0}};
    // // grid.gen_data_array_1d();
    const int voxel_len = 2;
    // const vec3 r1 = vec3{-295.0, -120.0, -120.0};
    const vec3 r1 = vec3{-50.0, -50.0, -11.0};
    // const vec3 r1 = vec3{-50.0, -50.0, -50.0};
    const vec3 r2 = - r1;
    Grid grid{r1, r2, voxel_len};
    std::vector<double> scattering_density;
    if (argc > 1) {
        scattering_density = loadScatteringDensity(argv[1]);
    } else {

        scattering_density = std::vector<double>(SIZE3(grid.grain));
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        std::uniform_real_distribution<double> dist{0, 1};
        auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };

        std::generate(std::begin(scattering_density), std::end(scattering_density), gen);
        for (auto i : scattering_density) {
            std::cout << i << " ";
        }
    }

    // scattering_density = scattering_density / scattering_density.max();
    MuonTomography app({argc, argv}, grid, scattering_density);
    return app.exec();
}
