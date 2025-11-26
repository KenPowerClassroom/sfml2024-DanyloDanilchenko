#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

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

    Texture t1,t2,t3,t4; // textures for blocks, background, ball, paddle
    t1.loadFromFile("images/arkanoid/block01.png");
    t2.loadFromFile("images/arkanoid/background.jpg");
    t3.loadFromFile("images/arkanoid/ball.png");
    t4.loadFromFile("images/arkanoid/paddle.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    sPaddle.setPosition(PADDLE_START_X, PADDLE_START_Y); // initial paddle position

    Sprite block[MAX_BLOCKS]; // array of sprites used to draw blocks (max)

    int n=0; // number of blocks created
    for (int i=1;i<=BLOCK_COLUMNS;i++) // create a grid of blocks: columns
    for (int j=1;j<=BLOCK_ROWS;j++) // and rows
      {
         block[n].setTexture(t1); // set block texture
         block[n].setPosition(i*BLOCK_SPACING_X, j*BLOCK_SPACING_Y); // place block at position
         n++; // increment block count
      }

    float dx = BALL_VELOCITY_X, dy = BALL_VELOCITY_Y; // ball velocity x and y
    float x = BALL_START_X, y = BALL_START_Y; // ball position

    while (app.isOpen()) // main game loop runs until window is closed
    {
       Event e;
       while (app.pollEvent(e)) // process all window events
       {
         if (e.type == Event::Closed)
             app.close(); // exit
       }

    x+=dx; // move ball horizontally by its velocity
    for (int i=0;i<n;i++) // check horizontal collisions of ball with each block
        if ( FloatRect(x+BALL_COLL_OFFSET_X, y+BALL_COLL_OFFSET_Y, BALL_COLL_WIDTH, BALL_COLL_HEIGHT).intersects(block[i].getGlobalBounds()) ) 
             {block[i].setPosition(BLOCK_REMOVED_X,0); dx=-dx;} // if hit, "remove" block by moving offscreen and reverse horizontal velocity

    y+=dy; // move ball vertically by its velocity
    for (int i=0;i<n;i++) // check vertical collisions of ball with each block
        if ( FloatRect(x+BALL_COLL_OFFSET_X, y+BALL_COLL_OFFSET_Y, BALL_COLL_WIDTH, BALL_COLL_HEIGHT).intersects(block[i].getGlobalBounds()) ) 
             {block[i].setPosition(BLOCK_REMOVED_X,0); dy=-dy;} // if hit, "remove" block and reverse vertical velocity

    if (x<0 || x>WINDOW_WIDTH)  dx=-dx; // bounce on left/right edges
    if (y<0 || y>WINDOW_HEIGHT)  dy=-dy; // bounce on top/bottom edges

    // player input
    if (Keyboard::isKeyPressed(Keyboard::Right)) sPaddle.move(PADDLE_MOVE_SPEED,0);
    if (Keyboard::isKeyPressed(Keyboard::Left)) sPaddle.move(-PADDLE_MOVE_SPEED,0);

    // if ball intersects paddle, change vertical velocity to a negative random value
    if ( FloatRect(x, y, BALL_DRAW_WIDTH, BALL_DRAW_HEIGHT).intersects(sPaddle.getGlobalBounds()) ) dy=-(rand()%BOUNCE_RAND_RANGE + BOUNCE_RAND_MIN);

    sBall.setPosition(x,y); // update sprite position

    app.clear();
    app.draw(sBackground); // draw background
    app.draw(sBall); // draw ball
    app.draw(sPaddle); // draw paddle

    for (int i=0;i<n;i++) // draw all block sprites
     app.draw(block[i]);

    app.display();
    }

  return 0;
}
