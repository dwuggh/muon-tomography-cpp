#include "readdata.h"
#include "grid.h"
#include "poca.h"

#include <fstream>
#include <iostream>
#include <cstdio>


int main(int argc, char** argv) {
    Grid grid{vec3{0.0, 0.0, 0.0}, vec3{10.0, 10.0, 10.0}};
    std::string input_filename;
    if (argc > 1) {
        input_filename = argv[1];
    } else {
        input_filename = "test.txt";
    }
    vec6 zs;
    zs << -200., -100., 200., 300., 0., 100.;
    auto images = read_data(input_filename, zs);
    auto density = calcScatteringDensity(images, grid);


    const char* output_filename = "result.txt";
    FILE* p = std::fopen(output_filename, "w");
    for (auto val : density) {
        std::fprintf(p, "%.6f ", val);
    }
    std::cout << density[0];
    std::fprintf(p, "\n");
    return 0;
}
