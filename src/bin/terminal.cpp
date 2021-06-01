#include "readdata.h"
#include "grid.h"
#include "poca.h"

#include <fstream>
#include <iostream>
#include <cstdio>


int main(int argc, char** argv) {
    const int voxel_len = 2;
    const vec3 r1 = vec3{-25.0, -25.0, -11.0};
    const vec3 r2 = - r1;
    Grid grid{r1, r2, voxel_len};
    std::string input_filename;
    if (argc > 1) {
        input_filename = argv[1];
    } else {
        input_filename = "test.txt";
    }
    vec6 zs;
    double z1 = 15, z2 = 11;
    double z3 = 11;
    zs << -z1, -z2, z2, z1, -z3, z3;
    auto images = read_data(input_filename, zs);
    auto density = calcScatteringDensity(images, grid);


    const char* output_filename = "result.txt";
    FILE* p = std::fopen(output_filename, "w");
    for (auto val : density) {
        std::fprintf(p, "%.6f ", val);
    }
    std::fprintf(p, "\n");
    return 0;
}
