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

// checkBlockCollisions marks blocks dead and inverts velocity
TEST(ArkanoidCollision, CheckBlockCollisions) {
    std::vector<Block> blocks;
    blocks.push_back(Block{ 50.f, 50.f, true });

    Rect collision{ 52.f, 52.f, 6.f, 6.f };
    float vel = 4.f;

    checkBlockCollisions(blocks, collision, 16.f, 8.f, vel);

    EXPECT_FALSE(blocks[0].alive);
    EXPECT_FLOAT_EQ(-4.f, vel);
}

// updatePhysics bounces on window edges
TEST(ArkanoidCollision, UpdatePhysicsBounceOnEdges) {
    std::vector<Block> blocks;

    float blockX = 515.f;
    float blockY = 10.f;
    float velocityX = 10.f;
    float velocityY = -3.f;

    updatePhysics(blockX, blockY, velocityX, velocityY, blocks, 16.f, 8.f,
        0.f, 0.f, 6.f, 6.f,
        520, 450);

    EXPECT_EQ(-10.f, velocityX);
    EXPECT_EQ(-3.f, velocityY);
}

// updatePhysics handles horizontal and vertical block collisions separately
TEST(ArkanoidCollision, UpdatePhysicsSeparateCollisions) {
    std::vector<Block> blocks;
    blocks.push_back(Block{ 100.f, 50.f, true });
    blocks.push_back(Block{ 120.f, 120.f, true });

    float blockX = 90.f;
    float blockY = 110.f;
    float velocityX = 10.f;
    float velocityY = 10.f;

    updatePhysics(blockX, blockY, velocityX, velocityY, blocks, 16.f, 8.f,
        0.f, 0.f, 6.f, 6.f,
        800, 600);

    EXPECT_FALSE(blocks[0].alive);
    EXPECT_FALSE(blocks[1].alive);
    EXPECT_EQ(-10.f, velocityX);
    EXPECT_EQ(-10.f, velocityY);
}