
#include <gtest/gtest.h>

#include "poca.h"
#include "readdata.h"

template <typename T>
bool vecEql(const std::vector<T>& vec1, const std::vector<T>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    } else {
        for (T ele : vec1) {
            if (std::find(vec2.begin(), vec2.end(), ele) == vec2.end()) {
                return false;
            }
        }
        return true;
    }
}

TEST(TestUtils, TestSum) {
    std::vector<double> vec1 = {1, 2, 3, 4};
    auto val1                = MT::sum(vec1);

    auto a             = 10.0;
    MT::Func<double> f = [a](double i) { return a * i; };
    auto val2          = MT::sum(vec1, f);
    ASSERT_EQ(val1, 10.0);
    ASSERT_EQ(val2, 100.0);
}

TEST(TestUtils, TestSum2) {
    std::vector<double> vec1                   = {1, 2, 3, 4};
    std::vector<double> vec2                   = {3, 4, 5, 6};
    std::vector<std::vector<double>> vec_outer = {vec1, vec2};

    auto val1 = MT::sum(vec_outer[0]);
    ASSERT_EQ(val1, 10.0);

    auto a             = 10.0;
    MT::Func<double> f = [a](double i) { return a * i; };
    auto val2          = MT::sum(vec_outer[1], f);
    ASSERT_EQ(val2, 180.0);
}

TEST(TestPoCA, FindingPoCA) {
    vec3 ri{0., 0., 0.};
    vec3 rf{50., 50., 100.};
    vec3 vi{0., 0., 1.};
    vec3 vf{0., 1., 1.};
    vec3 poca{25., 0., 50.};
    MuonImage image{ri, rf, vi, vf};
    vec3 poca_calc = PoCA(image);

    ASSERT_TRUE(isclose(poca, poca_calc));
}

TEST(TestPoCA, GetPassingVoxels) {
    vec3 ri{0., 0., 0.};
    vec3 rf{50., 50., 100.};
    vec3 vi{0., 0., 1.};
    vec3 vf{0., 1., 1.};
    MuonImage image{ri, rf, vi, vf};

    Grid grid{vec3{0, 0, 0}, vec3{100, 100, 100}, 10};
    PoCAData pocadata(grid);

    std::vector<int> path = {0, 1, 102, 103, 204, 205, 316, 327, 438, 449, 559};
    auto path_calc        = pocadata.getPassingVoxels(image, PoCA(image));
    for (auto p: path_calc) {
        std::cout << p << " ";
    }

    ASSERT_TRUE(vecEql(path, path_calc));
}

TEST(TestGrid, GetVoxelIndex3D) {

    Grid grid{vec3{0, 0, 0}, vec3{100, 100, 100}, 10};
    vec3 r1 = {0, 0, 0};
    vec3 r2 = {43., 35., 78.};
    vec3 r3 = {100, 100, 100};
    vec3 r4 = {143., 34., 8.};
    ASSERT_EQ(grid.get_voxel_index_3d(r1), (vec3i{0, 0, 0}));
    ASSERT_EQ(grid.get_voxel_index_3d(r2), (vec3i{4, 3, 7}));
    ASSERT_EQ(grid.get_voxel_index_3d(r3), (vec3i{9, 9, 9}));
    ASSERT_EQ(grid.get_voxel_index_3d(r4).has_value(), false);
}

TEST(TestGrid, GetVoxelIndex3DCornerCase) {

    Grid grid{vec3{0, 0, 0}, vec3{100, 100, 100}, 10};
    vec3 r1 = {0, 1E-8, 0};
    vec3 r2 = {100, 100, 100 + 1E-7};
    ASSERT_EQ(grid.get_voxel_index_3d(r1), (vec3i{0, 0, 0}));
    ASSERT_EQ(grid.get_voxel_index_3d(r2), (vec3i{9, 9, 9}));
}

TEST(TestGrid, GetVoxelIndex1D) {

    Grid grid{vec3{0, 0, 0}, vec3{100, 100, 100}, 10};
    vec3 r1 = {0, 0, 0};
    vec3 r2 = {43., 35., 78.};
    vec3 r3 = {100, 100, 100};
    ASSERT_EQ(grid.get_voxel_index_1d(r1).value(), 0);
    ASSERT_EQ(grid.get_voxel_index_1d(r2).value(), 437);
    ASSERT_EQ(grid.get_voxel_index_1d(r3).value(), 999);
}

TEST(TestGrid, VoxelIndexConversion) {

    Grid grid{vec3{0, 0, 0}, vec3{100, 100, 100}, 10};
    int i1 = 0, i2 = 437, i3 = 999;
    vec3i r1 = {0, 0, 0};
    vec3i r2 = {4, 3, 7};
    vec3i r3 = {9, 9, 9};
    ASSERT_EQ(grid.from_voxel_index_1d(i1), r1);
    ASSERT_EQ(grid.from_voxel_index_1d(i2), r2);
    ASSERT_EQ(grid.from_voxel_index_1d(i3), r3);
}

TEST(TESTPoCA, CalcScatteringDensity) {
    auto buffer = "1  11.130020  86.390010  58.830020  53.530010  190.270040  -38.690010  "
                  "236.910061  -71.550030  2.143730\n 2  -191.330020  -82.150010  -142.570020  "
                  "-69.430010  -7.950000  -34.449990  40.810020  -21.730010  6.410872\n";
    std::stringstream ss{buffer};
    vec6 zs;
    zs << -200, -120, 120, 200, -120, 120;
    auto images = read_data(ss, zs);
    // for (auto image : images) {
    //     std::cout << image;
    // }
    Grid grid{vec3{-300.0, -120.0, -120.0}, vec3{300.0, 120.0, 120.0}, 20};
    auto density = calcScatteringDensity(images, grid);
}
