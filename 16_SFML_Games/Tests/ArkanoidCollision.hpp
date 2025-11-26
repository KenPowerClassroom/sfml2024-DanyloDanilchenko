#pragma once

#include <vector>

namespace Arkanoid {

struct Rect {
    float x, y, w, h;
};

inline bool rectsIntersect(const Rect &a, const Rect &b) {
    return !(a.x + a.w <= b.x || b.x + b.w <= a.x ||
             a.y + a.h <= b.y || b.y + b.h <= a.y);
}

struct Block {
    float x;
    float y;
    bool alive;
};

// checks collisions between a collision rectangle and all block data
// if a block is hit, mark it not alive and invert velocity
inline void checkBlockCollisions(std::vector<Block> &blocks, const Rect &collisionRect, float blockW, float blockH, float &vel)
{
    for (auto &b : blocks) {
        if (!b.alive) continue;
        Rect blockRect{b.x, b.y, blockW, blockH};
        if (rectsIntersect(collisionRect, blockRect)) {
            b.alive = false;
            vel = -vel;
        }
    }
}

// move ball and detect collisions with blocks and screen bounds
inline void updatePhysics(float &ballPosX, float &ballPosY, float &ballVelX, float &ballVelY,
                          std::vector<Block> &blocks,
                          float blockW, float blockH,
                          float ballCollOffsetX, float ballCollOffsetY, float ballCollW, float ballCollH,
                          int windowWidth, int windowHeight)
{
    // Horizontal movement step
    ballPosX += ballVelX; // move ball horizontally
    Rect ballCollisionRectH{ballPosX + ballCollOffsetX, ballPosY + ballCollOffsetY, ballCollW, ballCollH};
    checkBlockCollisions(blocks, ballCollisionRectH, blockW, blockH, ballVelX);

    // Vertical movement step
    ballPosY += ballVelY; // move ball vertically
    Rect ballCollisionRectV{ballPosX + ballCollOffsetX, ballPosY + ballCollOffsetY, ballCollW, ballCollH};
    checkBlockCollisions(blocks, ballCollisionRectV, blockW, blockH, ballVelY);

    // screen boundary checks - bounce when leaving window
    if (ballPosX < 0 || ballPosX > windowWidth)
        ballVelX = -ballVelX;
    if (ballPosY < 0 || ballPosY > windowHeight)
        ballVelY = -ballVelY;
}

} // namespace Arkanoid
