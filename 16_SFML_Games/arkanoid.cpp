#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

// data clump for a block
struct Block { float x; float y; bool alive; };

// load textures from disk
void loadTextures(Texture &blockTex, Texture &bgTex, Texture &ballTex, Texture &paddleTex)
{
    blockTex.loadFromFile("images/arkanoid/block01.png");
    bgTex.loadFromFile("images/arkanoid/background.jpg");
    ballTex.loadFromFile("images/arkanoid/ball.png");
    paddleTex.loadFromFile("images/arkanoid/paddle.png");
}

// configure sprites and initial paddle position
void setupSprites(Sprite &sBackground, Sprite &sBall, Sprite &sPaddle,
                  Texture &bgTex, Texture &ballTex, Texture &paddleTex,
                  float paddleStartX, float paddleStartY)
{
    sBackground.setTexture(bgTex);
    sBall.setTexture(ballTex);
    sPaddle.setTexture(paddleTex);
    sPaddle.setPosition(paddleStartX, paddleStartY);
}

// create a grid of block data entries
void createBlocksData(Block blocks[], int &blockCount,
                      int cols, int rows, int spacingX, int spacingY)
{
    blockCount = 0;
    for (int i = 1; i <= cols; i++)
        for (int j = 1; j <= rows; j++)
        {
            blocks[blockCount].x = i * spacingX;
            blocks[blockCount].y = j * spacingY;
            blocks[blockCount].alive = true;
            blockCount++;
        }
}

void handleInput(Sprite &sPaddle, float paddleMoveSpeed)
{
    if (Keyboard::isKeyPressed(Keyboard::Right))
        sPaddle.move(paddleMoveSpeed, 0);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        sPaddle.move(-paddleMoveSpeed, 0);
}

// check collisions between a collision rectangle and all block data
// if a block is hit, mark it not alive and invert velocity
void checkBlockCollisions(Block blocks[], int blockCount, const FloatRect &collisionRect, float blockW, float blockH, float &vel)
{
    for (int i = 0; i < blockCount; i++)
    {
        if (!blocks[i].alive) continue;
        FloatRect blockRect(blocks[i].x, blocks[i].y, blockW, blockH);
        if (collisionRect.intersects(blockRect))
        {
            blocks[i].alive = false;
            vel = -vel;
        }
    }
}

// move ball and detect collisions with blocks and screen bounds
void updatePhysics(float &ballPosX, float &ballPosY, float &ballVelX, float &ballVelY,
                   Block blocks[], int blockCount,
                   float blockW, float blockH,
                   float ballCollOffsetX, float ballCollOffsetY, float ballCollW, float ballCollH,
                   int windowWidth, int windowHeight)
{
    // Horizontal movement step
    ballPosX += ballVelX; // move ball horizontally
    FloatRect ballCollisionRectH(ballPosX + ballCollOffsetX, ballPosY + ballCollOffsetY, ballCollW, ballCollH);
    checkBlockCollisions(blocks, blockCount, ballCollisionRectH, blockW, blockH, ballVelX);

    // Vertical movement step
    ballPosY += ballVelY; // move ball vertically
    FloatRect ballCollisionRectV(ballPosX + ballCollOffsetX, ballPosY + ballCollOffsetY, ballCollW, ballCollH);
    checkBlockCollisions(blocks, blockCount, ballCollisionRectV, blockW, blockH, ballVelY);

    // screen boundary checks
    if (ballPosX < 0 || ballPosX > windowWidth)
        ballVelX = -ballVelX; // bounce on left/right edges
    if (ballPosY < 0 || ballPosY > windowHeight)
        ballVelY = -ballVelY; // bounce on top/bottom edges
}

void handlePaddleCollision(float &ballPosX, float &ballPosY, float &ballVelY,
                           const Sprite &sPaddle,
                           float ballDrawW, float ballDrawH,
                           int bounceRandRange, int bounceRandMin)
{
    FloatRect paddleBounds = sPaddle.getGlobalBounds();
    FloatRect ballDrawRect(ballPosX, ballPosY, ballDrawW, ballDrawH);
    if (ballDrawRect.intersects(paddleBounds))
        ballVelY = -(rand() % bounceRandRange + bounceRandMin);
}

// Draw the current frame
void drawFrame(RenderWindow &app, Sprite &sBackground, Sprite &sBall, Sprite &sPaddle,
               Sprite &blockSpriteTemplate, Block blocks[], int blockCount)
{
    app.clear();
    app.draw(sBackground);
    app.draw(sBall);
    app.draw(sPaddle);

    for (int i = 0; i < blockCount; i++)
    {
        if (!blocks[i].alive) continue;
        blockSpriteTemplate.setPosition(blocks[i].x, blocks[i].y);
        app.draw(blockSpriteTemplate);
    }

    app.display();
}

int arkanoid()
{
    srand(time(0)); // seed random number generator

    // Named constants to replace magic numbers
    const int WINDOW_WIDTH = 520;
    const int WINDOW_HEIGHT = 450;
    const int FRAME_RATE = 60;

    const int BLOCK_COLUMNS = 10;
    const int BLOCK_ROWS = 10;
    const int BLOCK_SPACING_X = 43;
    const int BLOCK_SPACING_Y = 20;
    const int MAX_BLOCKS = 1000;

    const float PADDLE_START_X = 300.f;
    const float PADDLE_START_Y = 440.f;
    const float PADDLE_MOVE_SPEED = 6.f;

    const float BALL_VELOCITY_X = 6.f;
    const float BALL_VELOCITY_Y = 5.f;
    const float BALL_START_X = 300.f;
    const float BALL_START_Y = 300.f;

    // collision/drawing rectangle for the ball
    const float BALL_COLL_OFFSET_X = 3.f;
    const float BALL_COLL_OFFSET_Y = 3.f;
    const float BALL_COLL_WIDTH = 6.f;
    const float BALL_COLL_HEIGHT = 6.f;
    const float BALL_DRAW_WIDTH = 12.f;
    const float BALL_DRAW_HEIGHT = 12.f;

    // bounce randomization
    const int BOUNCE_RAND_RANGE = 5;
    const int BOUNCE_RAND_MIN = 2;

    RenderWindow app(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid!"); // create game window with size
    app.setFramerateLimit(FRAME_RATE); // cap frame rate

    // Textures
    Texture t1, t2, t3, t4; // textures for blocks, background, ball, paddle
    loadTextures(t1, t2, t3, t4);

    // Sprites
    Sprite sBackground, sBall, sPaddle;
    setupSprites(sBackground, sBall, sPaddle, t2, t3, t4, PADDLE_START_X, PADDLE_START_Y);

    // Single sprite template for drawing blocks
    Sprite blockSpriteTemplate(t1);
    Vector2u blockTexSize = t1.getSize();
    float blockWidth = static_cast<float>(blockTexSize.x);
    float blockHeight = static_cast<float>(blockTexSize.y);

    // Block data array
    Block blocksData[MAX_BLOCKS];
    int blockCount = 0; // number of blocks created
    createBlocksData(blocksData, blockCount, BLOCK_COLUMNS, BLOCK_ROWS, BLOCK_SPACING_X, BLOCK_SPACING_Y);

    float ballVelX = BALL_VELOCITY_X, ballVelY = BALL_VELOCITY_Y; // ball velocity x and y
    float ballPosX = BALL_START_X, ballPosY = BALL_START_Y; // ball position

    while (app.isOpen()) // main game loop runs until window is closed
    {
        Event e;
        while (app.pollEvent(e)) // process all window events
        {
            if (e.type == Event::Closed)
                app.close(); // exit
        }

        // Update movement and block collisions
        updatePhysics(ballPosX, ballPosY, ballVelX, ballVelY,
                      blocksData, blockCount,
                      blockWidth, blockHeight,
                      BALL_COLL_OFFSET_X, BALL_COLL_OFFSET_Y, BALL_COLL_WIDTH, BALL_COLL_HEIGHT,
                      WINDOW_WIDTH, WINDOW_HEIGHT);

        // player input 
        handleInput(sPaddle, PADDLE_MOVE_SPEED);

        // Handle ball-paddle collision
        handlePaddleCollision(ballPosX, ballPosY, ballVelY, sPaddle,
                              BALL_DRAW_WIDTH, BALL_DRAW_HEIGHT,
                              BOUNCE_RAND_RANGE, BOUNCE_RAND_MIN);

        sBall.setPosition(ballPosX, ballPosY); // update sprite position

        // draw everything
        drawFrame(app, sBackground, sBall, sPaddle, blockSpriteTemplate, blocksData, blockCount);
    }

    return 0;
}
