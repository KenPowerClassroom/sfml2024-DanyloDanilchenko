#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int arkanoid()
{
    srand(time(0)); // seed random number generator

    RenderWindow app(VideoMode(520, 450), "Arkanoid!"); // create game window with size
    app.setFramerateLimit(60); // cap frame rate to 60

    Texture t1,t2,t3,t4; // textures for blocks, background, ball, paddle
    t1.loadFromFile("images/arkanoid/block01.png");
    t2.loadFromFile("images/arkanoid/background.jpg");
    t3.loadFromFile("images/arkanoid/ball.png");
    t4.loadFromFile("images/arkanoid/paddle.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    sPaddle.setPosition(300,440); // initial paddle position

    Sprite block[1000]; // array of sprites used to draw blocks (max 1000)

    int n=0; // number of blocks created
    for (int i=1;i<=10;i++) // create a grid of blocks: 10 columns
    for (int j=1;j<=10;j++) // and 10 rows
      {
         block[n].setTexture(t1); // set block texture
         block[n].setPosition(i*43,j*20); // place block at position
         n++; // increment block count
      }

	float dx = 6, dy = 5; // ball velocity x and y
    float x=300, y=300; // ball position

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
        if ( FloatRect(x+3,y+3,6,6).intersects(block[i].getGlobalBounds()) ) 
             {block[i].setPosition(-100,0); dx=-dx;}

    y+=dy; // move ball vertically by its velocity
    for (int i=0;i<n;i++) // check vertical collisions of ball with each block
        if ( FloatRect(x+3,y+3,6,6).intersects(block[i].getGlobalBounds()) ) 
             {block[i].setPosition(-100,0); dy=-dy;}

    if (x<0 || x>520)  dx=-dx;
    if (y<0 || y>450)  dy=-dy;

    // player input
    if (Keyboard::isKeyPressed(Keyboard::Right)) sPaddle.move(6,0);
    if (Keyboard::isKeyPressed(Keyboard::Left)) sPaddle.move(-6,0);

    // if ball intersects paddle, change vertical velocity to a negative random value
    if ( FloatRect(x,y,12,12).intersects(sPaddle.getGlobalBounds()) ) dy=-(rand()%5+2);

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
