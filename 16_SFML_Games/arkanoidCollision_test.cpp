#include "gtest/gtest.h"
#include "ArkanoidCollision.h"

using namespace Arkanoid;

// Collision detection between rects
TEST(ArkanoidCollision, RectsIntersect) {
    Rect a{ 0.f, 0.f, 10.f, 10.f };
    Rect b{ 5.f, 5.f, 10.f, 10.f };
    Rect c{ 11.f, 11.f, 2.f, 2.f };

    EXPECT_TRUE(rectsIntersect(a, b));
    EXPECT_FALSE(rectsIntersect(a, c));
}