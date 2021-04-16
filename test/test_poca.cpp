
#include <gtest/gtest.h>

#include "poca.h"



TEST(TestPoCA, BasicAssertions) {
    vec3 ri{0., 0., 0.};
    vec3 rf{50., 50., 100.};
    vec3 vi{0., 0., 1.};
    vec3 vf{0., 1., 1.};
    vec3 poca{25., 0., 50.};
    MuonImage image{ri, rf, vi, vf};
    vec3 poca_alg = PoCA(image);

    ASSERT_TRUE(isclose(poca, poca_alg));
}
