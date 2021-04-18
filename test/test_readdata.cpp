#include "image.h"
#include "readdata.h"
#include "utils.h"
#include <gtest/gtest.h>
#include <string>

TEST(TestReadData, BasicAssertions) {

    vec3 p1{0., 0., 300.};
    vec3 p2{100., 50., 200.};
    vec3 p3{30., -60., -200.};
    vec3 p4{70., -100., -300.};
    double z1 = 100., z2 = -100.;
    vec3 ri{200., 100., 100.};
    vec3 rf{-10., -20., -100.};
    vec3 vi{2. / 3., 1. / 3., -2. / 3.};
    vec3 vf{-10., -20., -100.};

    MuonData data{p1, p2, p3, p4, z1, z2};
    MuonImage image = data.get_muon_image();

    EXPECT_TRUE(isclose(ri, image.ri));
    EXPECT_TRUE(isclose(rf, image.rf));
    EXPECT_TRUE(isclose(vi, image.vi));
}

// TODO add file reading test
