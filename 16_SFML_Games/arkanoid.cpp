#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

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

// create a grid of block sprites
void createBlocks(Sprite blocks[], int &blockCount, Texture &blockTex,
                  int cols, int rows, int spacingX, int spacingY)
{
    blockCount = 0;
    for (int i = 1; i <= cols; i++)
        for (int j = 1; j <= rows; j++)
        {
            blocks[blockCount].setTexture(blockTex);
            blocks[blockCount].setPosition(i * spacingX, j * spacingY);
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

// move ball and detect collisions with blocks and screen bounds
void updatePhysics(float &ballPosX, float &ballPosY, float &ballVelX, float &ballVelY,
                   Sprite blocks[], int blockCount,
                   int blockRemovedX,
                   float ballCollOffsetX, float ballCollOffsetY, float ballCollW, float ballCollH,
                   int windowWidth, int windowHeight)
{
    // Horizontal movement step
    ballPosX += ballVelX; // move ball horizontally
    FloatRect ballCollisionRectH(ballPosX + ballCollOffsetX, ballPosY + ballCollOffsetY, ballCollW, ballCollH);
    for (int i = 0; i < blockCount; i++) // check horizontal collisions
        if (ballCollisionRectH.intersects(blocks[i].getGlobalBounds()))
        {
            blocks[i].setPosition(blockRemovedX, 0);
            ballVelX = -ballVelX;
        }

    // Vertical movement step
    ballPosY += ballVelY; // move ball vertically
    FloatRect ballCollisionRectV(ballPosX + ballCollOffsetX, ballPosY + ballCollOffsetY, ballCollW, ballCollH);
    for (int i = 0; i < blockCount; i++) // check vertical collisions
        if (ballCollisionRectV.intersects(blocks[i].getGlobalBounds()))
        {
            blocks[i].setPosition(blockRemovedX, 0);
            ballVelY = -ballVelY;
        }

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
void drawFrame(RenderWindow &app, Sprite &sBackground, Sprite &sBall, Sprite &sPaddle, Sprite blocks[], int blockCount)
{
    app.clear();
    app.draw(sBackground);
    app.draw(sBall);
    app.draw(sPaddle);

    for (int i = 0; i < blockCount; i++)
        app.draw(blocks[i]);

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
    const int BLOCK_REMOVED_X = -100;

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

    Sprite block[MAX_BLOCKS]; // array of sprites used to draw blocks (max)

    int blockCount = 0; // number of blocks created
    createBlocks(block, blockCount, t1, BLOCK_COLUMNS, BLOCK_ROWS, BLOCK_SPACING_X, BLOCK_SPACING_Y);

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
                      block, blockCount,
                      BLOCK_REMOVED_X,
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
        drawFrame(app, sBackground, sBall, sPaddle, block, blockCount);
    }

    return 0;
}
