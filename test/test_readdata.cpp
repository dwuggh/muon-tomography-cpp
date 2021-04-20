#include <gtest/gtest.h>

#include "readdata.h"
#include "utils.h"

TEST(TestReadData, GetMuonImage) {

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

TEST(TestReadData, ReadDataByStream) {
    // std::ifstream in;
    auto buffer =
        "1  11.130020  86.390010  58.830020  53.530010  190.270040  -38.690010  236.910061  -71.550030  2.143730\n 2  -191.330020  -82.150010  -142.570020  -69.430010  -7.950000  -34.449990  40.810020  -21.730010  6.410872\n";
    std::stringstream ss{buffer};
    vec6 zs;
    zs << -200, -120, 120, 200, -120, 120;
    auto images = read_data(ss, zs);

    vec3 r1{11.130020, 86.390010, -200};
    vec3 r2{236.910061, -71.550030, 200};
    vec3 ri1{58.830020, 53.530010, -120};
    vec3 rf1{190.270040, -38.690010, 120};
    vec3 vi1 = (ri1 - r1).normalized();
    vec3 vf1 = (r2 - rf1).normalized();

    vec3 r3{-191.330020, -82.150010, -200};
    vec3 r4{40.810020, -21.730016, 200};
    vec3 ri2{-142.570020, -69.430010, -120};
    vec3 rf2{-7.950000, -34.449990, 120};
    vec3 vi2 = (ri2 - r3).normalized();
    vec3 vf2 = (r4 - rf2).normalized();

    EXPECT_TRUE(isclose(ri1, images[0].ri));
    EXPECT_TRUE(isclose(rf1, images[0].rf));
    EXPECT_TRUE(isclose(vi1, images[0].vi));
    EXPECT_TRUE(isclose(vf1, images[0].vf));

    EXPECT_TRUE(isclose(ri2, images[1].ri));
    EXPECT_TRUE(isclose(rf2, images[1].rf));
    EXPECT_TRUE(isclose(vi2, images[1].vi));
    EXPECT_TRUE(isclose(vf2, images[1].vf));

}


