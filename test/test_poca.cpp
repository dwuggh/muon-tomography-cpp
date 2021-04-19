
#include <gtest/gtest.h>

#include "poca.h"

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
    auto path_calc = pocadata.getPassingVoxels(image, PoCA(image));

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
