/*
  BlockBuster.pde
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
  Play BlockBuster story mode or verse your friends in pong.
 
 Version 1.25 - 12/2/2008
 Copyright (c) 2008 Windell H. Oskay.  All right reserved.
 http://www.evilmadscientist.com/
 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.
 	  
 */

 
 
 
 

/*
 * Adapted from "Blink,"  The basic Arduino example.  
 * http://www.arduino.cc/en/Tutorial/Blink
 */

#include <MeggyJrSimple.h>

int stopwatch;    // seeds the random generator and controls animation
int frame;      // decides what image will be displayed

struct point {int x; int y;};
struct projectile {int x; int y; int slope; int yvelocity; boolean inplay;};  // remembers position + velocity
projectile ball[3] = {{4, 2, 3, 1, true}};    // balls are projectiles
point paddle = {3, 0};      // singleplayer paddle
point paddleBlue = {3, 0};      // multiplayer paddles
point paddleRed = {4, 7};
int level;        // level and difficulty are identical, but there is a weird glitch
int difficulty;   // that occurs when there is only one
boolean moved;      // allows for motion by pressing or holding buttons
boolean blueMoved;      // multiplayer versions
boolean redMoved;
boolean started;      // determines whether the ball has been launched yet
boolean oneup;      // keeps track of oneup powerups
int onfire;      // keeps track of how long you will be on fire
int waittime;      // controls speed
int LED;      // handles the auxLEDs
int LED1;      // combines the scores of red and blue to determine the multiplayer LED value
int LED2;
int timer;      // controls the motion and updates of various elements
int failures;      // counts the number of times you have died
int redScore;       // keeps track of score
int blueScore;
int turn;      // allows the last person to lose a point to serve the next ball
int songt;


void setup()
{
  MeggyJrSimpleSetup();
  Serial.begin(9600);
  
  for (int j = 0; j < 2; j++)      // This all draws out the stationary parts of
    for (int k = 4; k < 8; k++)    // the menu screen
      DrawPx(j, k, White);
  DrawPx(2, 5, White);
  DrawPx(2, 6, White);
  for (int j = 5; j < 8; j++)
    for (int k = 4; k < 7; k++)
      DrawPx(j, k, White);
  DrawPx(6, 4, Dark);
  DrawPx(6, 6, Dark);
  DrawPx(6, 7, White);
  
  stopwatch = 0;
  frame = 0;
}


void loop() 
{
  stopwatch++;
  if (stopwatch % 400 == 0)
    frame++;        // There is 1/400 of a frame per milisecond, or 2.5 fps.
  if (frame > 5)      // There are 5 frames total, or 2 seconds of animation.
    frame = 0;
  
  CheckButtonsDown();
  
  if (Button_A)      // 'A' activates multiplayer mode (pong).
  {
    randomSeed(stopwatch);
    Multiplayer();
  }
  if (Button_B)      // 'B' activates singleplayer mode (blockbuster).
  {
    randomSeed(stopwatch);
    Singleplayer();
  }
  
  for (int j = 0; j < 8; j++)      // erases the last frame
    for (int k = 0; k < 4; k++)
      DrawPx(j, k, Dark);
  DrawPx(0, 3, Red);
  DrawPx(1, 3, Orange);
  DrawPx(2, 3, Violet);
  DrawPx(3, 3, Red);
  switch (frame)                   // and then draws the next one
  {
    case 0:
      DrawPx(2, 1, White);
      DrawPx(2, 0, Blue);
      DrawPx(3, 0, Blue);
      
      DrawPx(5, 2, White);
      DrawPx(6, 0, Blue);
      DrawPx(7, 0, Blue);
      DrawPx(4, 3, Red);
      DrawPx(5, 3, Red);
      break;
      
    case 1:
      DrawPx(1, 2, White);
      DrawPx(1, 0, Blue);
      DrawPx(2, 0, Blue);
      
      DrawPx(6, 1, White);
      DrawPx(6, 0, Blue);
      DrawPx(7, 0, Blue);
      DrawPx(5, 3, Red);
      DrawPx(6, 3, Red);
      break;
      
    case 2:
      DrawPx(0, 1, White);
      DrawPx(0, 0, Blue);
      DrawPx(1, 0, Blue);
      
      DrawPx(6, 2, White);
      DrawPx(5, 0, Blue);
      DrawPx(6, 0, Blue);
      DrawPx(6, 3, Red);
      DrawPx(7, 3, Red);
      break;
      
    case 3:
      DrawPx(1, 2, White);
      DrawPx(1, 0, Blue);
      DrawPx(2, 0, Blue);
      
      DrawPx(6, 1, White);
      DrawPx(5, 0, Blue);
      DrawPx(6, 0, Blue);
      DrawPx(6, 3, Red);
      DrawPx(7, 3, Red);
      break;
      
    case 4:
      DrawPx(2, 1, White);
      DrawPx(2, 0, Blue);
      DrawPx(3, 0, Blue);
      
      DrawPx(7, 2, White);
      DrawPx(5, 0, Blue);
      DrawPx(6, 0, Blue);
      DrawPx(6, 3, Red);
      DrawPx(7, 3, Red);
      break;
      
    case 5:
      DrawPx(2, 2, White);
      DrawPx(2, 0, Blue);
      DrawPx(3, 0, Blue);
      
      DrawPx(6, 1, White);
      DrawPx(6, 0, Blue);
      DrawPx(7, 0, Blue);
      DrawPx(5, 3, Red);
      DrawPx(6, 3, Red);
  }
  DisplaySlate();
  
  delay(1);      // a delay of 1 msec allows for more presice randomseeding
}


void Singleplayer()      // This is BlockBuster.
{
  EditColor(White, 13, 4, 3);      // setup
  EditColor(CustomColor0, 15, 0, 0);
  EditColor(CustomColor1, 15, 0, 15);
  EditColor(CustomColor2, 15, 0, 15);
  EditColor(CustomColor3, 15, 0, 15);
  EditColor(CustomColor4, 8, 0, 8);
  failures = 0;
  level = 1;
  difficulty = 1;
  reset();
  
  delay(1000);
  CheckButtonsPress();
  
  while (true)
  {
    EraseObjects();      // erase the paddle and ball
  
    UpdateBall();      // move the ball along trajectory
    
    UpdatePaddle();      // move the paddle
    
    UpdatePowerups();      // drop powerups
    
    DrawObjects();      // redraw the paddle and ball in their new positions
    
    delay(waittime);
    
    if (timer % (8 - level) == 0)
      Music();      // Play moozeek
    
    if (Button_A || Button_B || Button_Up)      // launch the ball if necessary
      started = true;
      
    while (Button_Down)      // down pauses the game
    {
      CheckButtonsDown();
    }
    
    if (YouHaveWon())      // check for victory
      RunVictory();
    
    timer++;
  }
}


void reset()      // resets variables, clears slate, and initiates next level
{
  ClearSlate();
  for (int i = 0; i < 3; i++)      // I use different letters for all of my
  {                                // loop variables to prevent conflicts
    ball[i].x = 3;      // the balls are an array for when the player gets multiball
    ball[i].y = 1;
    ball[i].slope = random(2)*4-2;      // slope is actually half the number of blocks travelled right for every block up or down
    ball[i].yvelocity = 1;      // yvelocity is always 1 or -1
    ball[i].inplay = false;      // keeps track of which balls are active
  }
  ball[0].inplay = true;
  paddle.x = 3;
  paddle.y = 0;
  moved = false;
  started = false;
  oneup = false;
  onfire = 0;
  waittime = 8;
  EditColor(White, 13, 4, 3);
  timer = 0;
  songt = 0;
  switch (difficulty)
  {
    case 1:
      Level1();
      break;
    case 2:
      Level2();
      break;
    case 3:
      Level3();
      break;
    case 4:
      Level4();
      break;
    case 5:
      Level5();
  }
  DrawPx(random(8), random(5)+3, CustomColor4);      // places the powerup
}


void EraseObjects()
{
  for (int v = 0; v < 3; v++)      // only erase balls that are in play
    if (ball[v].inplay)
      DrawPx(ball[v].x, ball[v].y, Dark);
      
    DrawPx(paddle.x, 0, Dark);
  
  for(int j = -1; j < 2; j++)      // paddle.x is the center of the paddle
    DrawPx(paddle.x + j, paddle.y, Dark);
}


void DrawObjects()
{
  for (int j = -1; j < 2; j++)
    DrawPx(paddle.x + j, paddle.y, Blue);
  
  DrawPx(paddle.x, 0, Blue);      // draws the base when paddle is up
    
  for (int u = 0; u < 3; u++)      // only draw balls that are in play
    if (ball[u].inplay)
      DrawPx(ball[u].x, ball[u].y, White);
      
  DisplaySlate();
  
  LED = 0;      // calculates the LED value based on the level
  for(int e = 0; e < difficulty; e++)
    LED = 2*LED + 1;
  SetAuxLEDs(LED);
}


void UpdatePowerups()
{
  for(int b = 0; b < 8; b++)      // plays a tone when a powerup first becomes
    for(int c = 0; c < 8; c++)    // active (gets hit)
      if(ReadPx(b, c) == CustomColor3)
      {
        DrawPx(b, c, CustomColor2);
//        Tone_Start(ToneB5, 50);
      }
      
  if (timer % 40 == 0)
  {
    for(int b = 0; b < 8; b++)      // makes powerups at the bottom of the screen disappear
      if(ReadPx(b, 0) == CustomColor2)
        DrawPx(b, 0, Dark);
    
    for(int b = 0; b < 8; b++)      // makes powerups fall at a rate of 25 px/sec
      for(int c = 1; c < 8; c++)
        if(ReadPx(b, c) == CustomColor2 && ReadPx(b, c - 1) == 0)
        {
          DrawPx(b, c, Dark);
          DrawPx(b, c - 1, CustomColor2);
        }
      
    for(int b = 0; b < 8; b++)      // allows the ball to destroy the powerup
      for(int c = 0; c < 8; c++)
        if(ReadPx(b, c) == CustomColor1)
          DrawPx(b, c, Dark);
  }
  
  for (int v = -1; v < 2; v++)
    if (ReadPx(paddle.x + v, paddle.y) == CustomColor2)
      switch (random(5))
      {                  // randomly selects a powerup if any part of the paddle
        case 0:          // touches a falling powerup.
          MultiBall();      // gifts two extra balls
          break;
        case 1:
          InstaLaser();      // fires a laser straight up from the paddle
          break;
        case 2:
          OneUp();      // allows you to drop the ball once
          break;
        case 3:
          Fireball();      // makes the ball temporarily phase through blocks
          break;
        default:
          Accelerate();      // speeds up the game (powerups aren't always helpful)
      }  
}



void UpdateBall()
{
  if (started)
  {
    for (int n = 0; n < 3; n++)      // update all three balls, if they are in play
      if (ball[n].inplay)
      {
        if (timer % (60/ball[n].slope) == 0)
          BounceX(n);      // change velocity horizontally at a rate based on slope
    
        if (timer % 30 == 0)
          BounceY(n);      // change velocity vertically at a rate based on yvelocity
    
        if (timer % (60/ball[n].slope) == 0 && timer % 30 == 0)
          BounceDiagonal(n);      // bounce off of the corners of blocks
  
        if(timer % (60/ball[n].slope) == 0)
        {
          if (ball[n].slope > 0)      // move according to slope
            ball[n].x++;
          if (ball[n].slope < 0)
            ball[n].x--;
        }

        if (timer % 30 == 0)
        {
          ball[n].y += ball[n].yvelocity;      // move according to yvelocity
        }
  
        StopTeleport(n);      // prevent glitches in which the ball goes off screen
          
        if (ball[n].x == paddle.x - 2)          // makes the ball bounce off the
          if (ball[n].y == 0 && paddle.y == 0)  // left side of the paddle
          {
            ball[n].x--;
            ball[n].slope = -abs(ball[n].slope);
          }
            
        if (ball[n].x == paddle.x + 2)          // makes the ball bounce off the
          if (ball[n].y == 0 && paddle.y == 0)  // right side of the paddle
          {
            ball[n].x++;
            ball[n].slope = abs(ball[n].slope);
          }
            
        if (ball[n].x - paddle.x < 2 && ball[n].x - paddle.x > -2)
          if (ball[n].y == paddle.y)    // makes the paddle push the ball up
          {                             // when going up, as well as prevents
            ball[n].y++;                // glitches in which the ball phases
            SlopeChange(n);             // through the paddle
          }
      }
    
    if (onfire > 0)
      onfire--;
    else if (oneup)      // gives the ball a color based on powerups
      EditColor(White, 15, 12, 4);
    else
      EditColor(White, 13, 4, 3);
  
    if (!ball[0].inplay & !ball[1].inplay & !ball[2].inplay)
      GameOver();      // if no balls are in play, you have lost
  }
  
  else      // this is if you still have not launched the ball
  {
    ball[0].x = paddle.x;
    ball[0].y = paddle.y + 1;
  }
}


void StopTeleport(int index)
{
  if (ball[index].x > 7)
    ball[index].x = 7;
  if (ball[index].x < 0)
    ball[index].x = 0;
  if (ball[index].y > 7)
    ball[index].y = 7;
  if (ball[index].y < 0)
    ball[index].y = 0;
}


void UpdatePaddle()
{
  MovePress();      // moves when you press the buttons
  
  if (timer % 20 == 0)      // moves as the buttons are held
    MoveHold();
  
  CheckButtonsDown();
    
  if (Button_Up)      // raises the paddle up when Up is held
    paddle.y = 1;
  else
    paddle.y = 0;
}


void MovePress()
{
  CheckButtonsPress();
  
  if (Button_Right && paddle.x < 6)
  {
    paddle.x++;
    moved = true;      // setting moved here makes it so you must wait at least
  }                    // 20 msec after pressing a button before the paddle slides
    
  if (Button_Left && paddle.x > 1)
  {
    paddle.x--;
    moved = true;
  }
}


void MoveHold()
{
  CheckButtonsDown();
  
  if (!moved)
  {
    if (Button_Right && paddle.x < 6)
      paddle.x++;
  
    if (Button_Left && paddle.x > 1)
      paddle.x--;
  }
    
  moved = false;
}


void BounceY(int index)
{
  if (ball[index].yvelocity > 0)
  {
    if (ball[index].y > 6)
    {
      ball[index].yvelocity = -1;      // bounces ball off top of screen
//      Tone_Start(ToneB4, 50);
    }
    
    if (ReadPx(ball[index].x, ball[index].y+1) > 0 && onfire == 0)
    {
      ball[index].yvelocity = -1;      // bounces ball off top of blocks
      DrawPx(ball[index].x, ball[index].y+1, ReadPx(ball[index].x, ball[index].y+1) - 1);  // removes health from blocks
//      Tone_Start(ToneB3, 50);
    }
  }
  
  if (ball[index].yvelocity < 0)
  {
    if (ReadPx(ball[index].x, ball[index].y-1) > 0 && onfire == 0)
    {
      ball[index].yvelocity = 1;      // bounces ball off bottom of blocks
      DrawPx(ball[index].x, ball[index].y-1, ReadPx(ball[index].x, ball[index].y-1) - 1);
//      Tone_Start(ToneB3, 50);
    }
    
    if (ball[index].y < paddle.y + 2)      // bounces ball off paddle (and changes slope)
      SlopeChange(index);
  }
  
  if (ball[index].y < 1)
    ball[index].inplay = false;      // removes ball from play if it goes out of bounds
}


void BounceX(int index)
{
  if (ball[index].slope > 0)
  {
    if (ball[index].x > 6)
    {
      ball[index].slope = -ball[index].slope;  // bounces ball off right of screen
//      Tone_Start(ToneB4, 50);
    }
    
    if (ball[index].x == paddle.x - 2 && ball[index].y == paddle.y)
    {
      ball[index].slope = -ball[index].slope;    // bounces ball off right of paddle
//      Tone_Start(ToneB4, 50);
    }
    
    if (ReadPx(ball[index].x+1, ball[index].y) > 0 && onfire == 0)
    {
      ball[index].slope = -ball[index].slope;  // bounces ball off right of blocks
      DrawPx(ball[index].x+1, ball[index].y, ReadPx(ball[index].x+1, ball[index].y) - 1);
//      Tone_Start(ToneB3, 50);
    }  
  }
  
  if (ball[index].slope < 0)
  {
    if (ball[index].x < 1)
      ball[index].slope = abs(ball[index].slope);  // bounces ball off left of screen
      
    if (ReadPx(ball[index].x-1, ball[index].y) > 0 && onfire == 0)
    {
      ball[index].slope = abs(ball[index].slope);  // bounces ball off left of blocks
      DrawPx(ball[index].x-1, ball[index].y, ReadPx(ball[index].x-1, ball[index].y) - 1);
//      Tone_Start(ToneB3, 50);
    }
    
    if (ball[index].x < 1)      // plays the tone for bouncing off the left wall
//      Tone_Start(ToneB4, 50);   // (it was buggy, so I moved it here)
      
    if (ball[index].x == paddle.x + 2 && ball[index].y == paddle.y)
    {
      ball[index].slope = abs(ball[index].slope);  // bounces ball off left of paddle
//      Tone_Start(ToneB4, 50);
    }
  }
}


void BounceDiagonal(int index)
{
  if (ball[index].slope > 0 && ball[index].yvelocity > 0)
    if (ReadPx(ball[index].x+1, ball[index].y+1) > 0 && onfire == 0)
    {
      ball[index].slope = -ball[index].slope;
      ball[index].yvelocity = -1;
      DrawPx(ball[index].x+1, ball[index].y+1, ReadPx(ball[index].x+1, ball[index].y+1)-1);
//      Tone_Start(ToneB3, 50);
    }      // each of these makes the ball bounce from a different corner of blocks
  
  if (ball[index].slope > 0 && ball[index].yvelocity < 0)
    if (ReadPx(ball[index].x+1, ball[index].y-1) > 0 && onfire == 0)
    {
      ball[index].slope = -ball[0].slope;
      ball[index].yvelocity = 1;
      DrawPx(ball[index].x+1, ball[index].y-1, ReadPx(ball[index].x+1, ball[index].y-1)-1);
//      Tone_Start(ToneB3, 50);
    }
  
  if (ball[index].slope < 0 && ball[index].yvelocity > 0)
    if (ReadPx(ball[index].x-1, ball[index].y+1) > 0 && onfire == 0)
    {
      ball[index].slope = abs(ball[index].slope);
      ball[index].yvelocity = -1;
      DrawPx(ball[index].x-1, ball[index].y+1, ReadPx(ball[index].x-1, ball[index].y+1)-1);
//      Tone_Start(ToneB3, 50);
    }
  
  if (ball[index].slope < 0 && ball[index].yvelocity < 0)
    if (ReadPx(ball[index].x-1, ball[index].y-1) > 0 && onfire == 0)
    {
      ball[index].slope = abs(ball[index].slope);
      ball[index].yvelocity = 1;
      DrawPx(ball[index].x-1, ball[index].y-1, ReadPx(ball[index].x-1, ball[index].y-1)-1);
//      Tone_Start(ToneB3, 50);
    }
}
  

void SlopeChange(int ballindex)
{
  switch (paddle.x - ball[ballindex].x)
  {
    case -2:      // changes slope based on what part of the paddle the ball hits
      if (ball[ballindex].slope < 0)
      {
        ball[ballindex].yvelocity = 1;
        ball[ballindex].slope += 2;
//        Tone_Start(ToneB4, 50);
      }
      break;
    case -1:
      ball[ballindex].yvelocity = 1;
      ball[ballindex].slope++;
//      Tone_Start(ToneB4, 50);
      break;
    case 0:
      ball[ballindex].yvelocity = 1;
//      Tone_Start(ToneB4, 50);
      break;
    case 1:
      ball[ballindex].yvelocity = 1;
      ball[ballindex].slope--;
//      Tone_Start(ToneB4, 50);
      break;
    case 2:
      if (ball[ballindex].slope > 0)
      {
        ball[ballindex].yvelocity = 1;
        ball[ballindex].slope -= 2;
//        Tone_Start(ToneB4, 50);
      }
  }
}


void MultiBall()
{
  for (int g = 0; g < 3; g++)
    if (ball[g].inplay)      // puts all balls into play and gives them differing
    {                        // velocities to make them spread out
      ball[(g+1)%3].x = ball[g].x;
      ball[(g+1)%3].y = ball[g].y;
      ball[(g+1)%3].slope = -ball[g].slope;
      ball[(g+1)%3].yvelocity = ball[g].yvelocity;
      ball[(g+1)%3].inplay = true;
      ball[(g+2)%3].x = ball[g].x;
      ball[(g+2)%3].y = ball[g].y;
      ball[(g+2)%3].slope = ball[g].slope;
      ball[(g+2)%3].yvelocity = -ball[g].yvelocity;
      ball[(g+2)%3].inplay = true;
    }
    
  Tone_Start(ToneB5,300);
  delay(50);
  Tone_Start(ToneB4,300);
  delay(50);
  Tone_Start(ToneB5,50);  
}


void InstaLaser()
{
  DrawObjects();
  
  Tone_Start(ToneD7, 200);
  for (int h = 1; h < 8; h++)      // draws a column of FullOn from the paddle
  {
    DrawPx(paddle.x, h, FullOn);
    DisplaySlate();
    delay(15);
  }
  
  Tone_Start(ToneD6, 200);
  for (int h = 1; h < 8; h++)      // erases the column
  {
    DrawPx(paddle.x, h, Dark);
    DisplaySlate();
    delay(15);
  }
  
  Tone_Start(ToneB5, 200);
}


void OneUp()
{
  EditColor(White, 15, 12, 4);      // turns the ball green
  oneup = true;      // setting oneup prevents death
  Tone_Start(ToneC6,150);
  delay(100);
  Tone_Start(ToneE6,150);
  delay(100);
  Tone_Start(ToneC6,150);
  delay(100);
  Tone_Start(ToneE6,150);
  delay(100);
  Tone_Start(ToneG6,200);
}


void Fireball()
{
  EditColor(White, 25, 1, 0);      // turns the ball orange
  onfire = 1500;      // while onfire>0 the ball will not collide with blocks
  Tone_Start(ToneF5,200);
  delay(100);
  Tone_Start(ToneE5,200);
  delay(100);
  Tone_Start(ToneC5,200);
  delay(100);
  Tone_Start(ToneD5,100);
}


void Accelerate()
{
  waittime = 4;      // shortens the delay to make the game more challenging
  Tone_Start(ToneC5,300);
  delay(100);
  Tone_Start(ToneCs5,300);
  delay(100);
  Tone_Start(ToneD5,200);
}


boolean YouHaveWon()      // checks to see if there are any red, orange, or
{                         // yellow blocks left.  If not, it is true.
  for (int p = 0; p < 8; p++)
    for (int o = 0; o < 8; o++)
      switch (ReadPx(o, p))
      {
        case 1:
          return false;
          break;
        case 2:
          return false;
          break;
        case 3:
          return false;
      }
  return true;
}


void RunVictory()
{
  for(int i=0;i<8;i++)      // flashes green, fanfares, resets on the next level
    for(int j=0;j<8;j++)
      DrawPx(i,j,Green);
  DisplaySlate();
  Tone_Start(ToneC5,150);
  delay(150);
  Tone_Start(ToneE5,150);
  delay(150);
  Tone_Start(ToneC6,150);
  delay(150);
  level++;
  difficulty++;
  reset();
}


void GameOver()
{
  if (oneup)      // dying with a oneup respawns the ball in the middle of the
  {               // screen
    oneup = false;
    EditColor(White, 13, 4, 3);
    ball[0].inplay = true;
    ball[0].yvelocity = 1;
    ball[0].x = 2 + random(4);
    ball[0].y = 2;
  }
  else
  {
    failures++;      // flashes red, buzzes, starts the level over
    DisplaySlate();
    delay(200);
    for(int i=0;i<8;i++)
      for(int j=0;j<8;j++)
        DrawPx(i, j, CustomColor0);
    DisplaySlate();
    Tone_Start(ToneB2,400);
    delay(500);
    Tone_Start(ToneB2,400);
    delay(500);
    Tone_Start(ToneB2,500);
    delay(500);
    reset();
  }
}


void Level1()      // These draw each level.  There are four total.
{
  for (int i = 2; i < 6; i++)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Red);
}


void Level2()
{
  for (int i = 2; i < 6; i++)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Red);
  for (int i = 2; i < 6; i++)
    DrawPx(i, 4, Orange);
}


void Level3()
{
  for (int i = 2; i < 6; i++)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Red);
  for (int i = 2; i < 6; i+=3)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Orange);
  for (int i = 2; i < 6; i++)
    DrawPx(i, 4, Yellow);
  DrawPx(3, 3, Yellow);
  DrawPx(4, 3, Yellow);
}


void Level4()
{
  for (int i = 2; i < 6; i++)
    for (int k = 6; k < 8; k++)
       DrawPx(i, k, Yellow);
  for (int i = 0; i < 3; i++)
    for (int k = 3; k < 5; k++)
       DrawPx(i, k, Red);
  for (int i = 5; i < 8; i++)
    for (int k = 3; k < 5; k++)
       DrawPx(i, k, Red);
  DrawPx(2, 4, Dark);
  DrawPx(5, 4, Dark);
  DrawPx(2, 6, Dark);
  DrawPx(5, 6, Dark);
}


void Level5()
{
  for(int i=0;i<8;i++)      // plays extended fanfare and shows fail count
    for(int j=0;j<8;j++)
      DrawPx(i,j,Green);
  DisplaySlate();
  delay(600);
  Tone_Start(ToneC5,150);
  delay(150);
  Tone_Start(ToneE5,150);
  delay(150);
  Tone_Start(ToneG5,150);
  delay(150);
  Tone_Start(ToneF5,150);
  delay(150);
  Tone_Start(ToneA5,150);
  delay(150);
  Tone_Start(ToneC6,150);
  delay(150);
  Tone_Start(ToneG5,150);
  delay(150);
  Tone_Start(ToneB5,150);
  delay(150);
  Tone_Start(ToneD6,150);
  delay(150);
  Tone_Start(ToneC6,300);
  delay(600);
  ClearSlate();
  ShowNumeral(0, (failures - failures%10) /10);    // shows tens place
  ShowNumeral(4, failures%10);    // shows ones place
  while (true)      // stops the game
  {
  }
}


void ShowNumeral(int column, int digit)
{
  int color;          // the color changes from green to orange to yellow to red
  if (failures < 15)  // as the fail count increases
    color = 4 - failures/3;
  else
    color = 1;
    
  switch(digit)      // draws the number in the appropriate place and color
  {
    case 0:
      for(int q = 0; q < 4; q += 3)
        for(int w = 1; w < 6; w++)
          DrawPx(q + column, w, color);
      for(int q = 0; q < 8; q += 6)
        for(int w = 1; w < 3; w++)
          DrawPx(w + column, q, color);
      break;
      
    case 1:
      for (int w = 0; w < 7; w++)
        DrawPx(2 + column, w, color);
      break;
      
    case 2:
      for (int w = 0; w < 4; w++)
        DrawPx(w + column, 0, color);
      DrawPx(column, 1, color);
      DrawPx(column, 2, color);
      DrawPx(column + 1, 3, color);
      DrawPx(column + 2, 3, color);
      DrawPx(column + 3, 4, color);
      DrawPx(column + 3, 5, color);
      DrawPx(column + 1, 6, color);
      DrawPx(column + 2, 6, color);
      DrawPx(column, 5, color);
      break;
      
    case 3:
      for (int w = 1; w < 6; w++)
        DrawPx(column + 3, w, color);
      DrawPx(column + 3, 3, Dark);
      for (int q = 1; q < 3; q++)
        for (int w = 0; w < 7; w += 3)
          DrawPx(q + column, w, color);
      DrawPx(column, 1, color);
      DrawPx(column, 5, color);
      break;
      
    case 4:
      for (int w = 0; w < 7; w++)
        DrawPx(column + 3, w, color);
      for (int w = 0; w < 4; w++)
        DrawPx(column + w, 3, color);
      for (int w = 3; w < 7; w++)
        DrawPx(column, w, color);
      break;
      
    case 5:
      for (int q = 1; q < 3; q++)
        for (int w = 0; w < 7; w += 3)
          DrawPx(column + q, w, color);
      for (int w = 1; w < 7; w++)
        DrawPx(column, w, color);
      DrawPx(column, 2, Dark);
      DrawPx(column + 3, 1, color);
      DrawPx(column + 3, 2, color);
      DrawPx(column + 3, 6, color);
      break;
      
    case 6:
      for (int w = 1; w < 6; w++)
        DrawPx(column, w, color);
      for (int q = 1; q < 3; q++)
        for (int w = 0; w < 7; w += 3)
          DrawPx(column + q, w, color);
      DrawPx(column + 3, 1, color);
      DrawPx(column + 3, 2, color);
      DrawPx(column + 3, 5, color);
      break;
      
    case 7:
      for (int w = 0; w < 4; w++)
        DrawPx(column + w, 6, color);
      for (int q = 0; q < 2; q++)
        for (int w = 0; w < 3; w++)
          DrawPx(column + 1 + w, 2*w + q, color);
      break;
      
    case 8:
      for (int q = 0; q < 4; q += 3)
        for (int w = 1; w < 6; w++)
          DrawPx(column + q, w, color);
      for (int q = 1; q < 3; q++)
        for (int w = 0; w < 7; w += 3)
          DrawPx(column + q, w, color);
      DrawPx(column, 3, Dark);
      DrawPx(column + 3, 3, Dark);
      break;
      
    case 9:
      for (int w = 1; w < 6; w++)
        DrawPx(column + 3, w, color);
      for (int q = 1; q < 3; q++)
        for (int w = 0; w < 7; w += 3)
          DrawPx(column + q, w, color);
      DrawPx(column, 1, color);
      DrawPx(column, 4, color);
      DrawPx(column, 5, color);
  }
  DisplaySlate();
}



void Multiplayer()        // This is Pong.
{
  blueScore = 0;      // setup
  redScore = 0;
  turn = 1;
  MPReset();
  
  delay(1000);
  CheckButtonsPress();
  
  while (true)      // main loop
  {
    MPEraseObjects();      // erase both paddles and ball
  
    MPUpdateBall();      // move the ball along trajectory
    
    MPUpdatePaddle();      // move the paddles based on buttons
  
    MPDrawObjects();      // redraw paddles and ball
    
    delay(waittime);
    
    if (timer%(11 - (blueScore+redScore)/2) == 0)
      Music();
    
    if (Button_Up || Button_Down)      // either up or down will start the game
      started = true;
      
    timer++;
  }
}


void MPReset()
{
  ClearSlate();
  for (int i = 0; i < 3; i++)
  {
    ball[i].x = 3;
    ball[i].y = 1;
    ball[i].slope = random(2)*4-2;
    ball[i].yvelocity = 1;
    ball[i].inplay = false;
  }
  ball[0].inplay = true;
  paddleBlue.x = 3;      // the paddles do not move up or down in multiplayer,
  paddleRed.x = 4;       // so the y values are not needed
  blueMoved = false;
  redMoved = false;
  started = false;
  waittime = 5;      // MP is slightly faster than SP because the ball has to
  timer = 0;         // travel farther.
  songt = 0;
  
  if (blueScore >= 4)      // first to 4 points wins
    BlueWin();
  if (redScore >= 4)
    RedWin();
}


void MPEraseObjects()          // these methods are nearly identical to the SP
{                              // counterparts, but there are two paddles, and
  for (int v = 0; v < 3; v++)  // block collisions are omitted.
    if (ball[v].inplay)
      DrawPx(ball[v].x, ball[v].y, Dark);
  
  for(int j = -1; j < 2; j++)
    DrawPx(paddleBlue.x + j, 0, Dark);
    
  for(int j = -1; j < 2; j++)
    DrawPx(paddleRed.x + j, 7, Dark);
}


void MPDrawObjects()
{
  for (int j = -1; j < 2; j++)
    DrawPx(paddleBlue.x + j, 0, Blue);
    
  for (int j = -1; j < 2; j++)
    DrawPx(paddleRed.x + j, 7, Red);
    
  for (int u = 0; u < 3; u++)
    if (ball[u].inplay)
      DrawPx(ball[u].x, ball[u].y, White);
      
  DisplaySlate();
  
  LED1 = 0;      // to have the LEDs come in from both sides, I have two variables
  LED2 = 0;
  for (int e = 0; e < blueScore; e++)
    LED1 = 2*LED1 + 1;
  for (int e = 0; e < redScore; e++)
    LED2 = LED2/2 + 128;
  SetAuxLEDs(LED1 + LED2);
}


void MPUpdateBall()
{
  if (started)
  {
    for (int n = 0; n < 3; n++)
      if (ball[n].inplay)
      {
        if(timer % (60/ball[n].slope) == 0)
          MPBounceX(n);
    
        if (timer % 30 == 0)
          MPBounceY(n);
    
        if(timer % (60/ball[n].slope) == 0)
        {
          if (ball[n].slope > 0)
            ball[n].x++;
          if (ball[n].slope < 0)
            ball[n].x--;
        }

        if (timer % 30 == 0)
        {
          ball[n].y += ball[n].yvelocity;
        }
  
        StopTeleport(n);
        
        if (ball[n].x - paddleBlue.x < 2 && ball[n].x - paddleBlue.x > -2)
          if (ball[n].y == 0)
            ball[n].y++;
            
        if (ball[n].x - paddleRed.x < 2 && ball[n].x - paddleRed.x > -2)
          if (ball[n].y == 7)
            ball[n].y--;
      }
  }
  
  else
  {
    if (turn)      // the ball is served by whoever lost the previous point
    {
      ball[0].x = paddleBlue.x;
      ball[0].y = 1;
    }
    else
    {
      ball[0].x = paddleRed.x;
      ball[0].y = 6;
    }
  }
}


void MPUpdatePaddle()
{
  MPMovePress();
  
  if (timer % 20 == 0)
    MPMoveHold();
}


void MPMovePress()
{
  CheckButtonsPress();
  
  if (Button_Right && paddleBlue.x < 6)
  {
    paddleBlue.x++;
    blueMoved = true;
  }
    
  if (Button_Left && paddleBlue.x > 1)
  {
    paddleBlue.x--;
    blueMoved = true;
  }
  
  if (Button_A && paddleRed.x < 6)
  {
    paddleRed.x++;
    redMoved = true;
  }
    
  if (Button_B && paddleRed.x > 1)
  {
    paddleRed.x--;
    redMoved = true;
  }
}


void MPMoveHold()
{
  CheckButtonsDown();
  
  if (!blueMoved)
  {
    if (Button_Right && paddleBlue.x < 6)
      paddleBlue.x++;
  
    if (Button_Left && paddleBlue.x > 1)
      paddleBlue.x--;
  }
  
  if (!redMoved)
  {
    if (Button_A && paddleRed.x < 6)
      paddleRed.x++;
  
    if (Button_B && paddleRed.x > 1)
      paddleRed.x--;
  }
    
  blueMoved = false;
  redMoved = false;
}


void MPBounceY(int index)
{
  if (ball[index].yvelocity > 0)
    if (ball[index].y > 5)
      SlopeChangeRed(index);
  
  if (ball[index].yvelocity < 0)
    if (ball[index].y < 2)
      SlopeChangeBlue(index);
      
  if (ball[index].y < 1)
  {
    ball[index].inplay = false;
    RedVictory();
  }
  
  if (ball[index].y > 6)
  {
    ball[index].inplay = false;
    BlueVictory();
  }
}


void MPBounceX(int index)
{
  if (ball[index].slope > 0)
  {
    if (ball[index].x > 6)
    {
      ball[index].slope = -ball[index].slope;
//      Tone_Start(ToneB4, 50);
    }
    
    if (ball[index].x == paddleBlue.x - 2 && ball[index].y == 0)
    {
      ball[index].slope = -ball[index].slope;
//      Tone_Start(ToneB4, 50);
    }
    
    if (ball[index].x == paddleRed.x - 2 && ball[index].y == 7)
    {
      ball[index].slope = -ball[index].slope;
//      Tone_Start(ToneB4, 50);
    }
  }
  
  if (ball[index].slope < 0)
  {
    if (ball[index].x < 1)
      ball[index].slope = abs(ball[index].slope);
      
    if (ball[index].x < 1)
//      Tone_Start(ToneB4, 50);
      
    if (ball[index].x == paddleBlue.x + 2 && ball[index].y == 0)
    {
      ball[index].slope = abs(ball[index].slope);
//      Tone_Start(ToneB4, 50);
    }
    
    if (ball[index].x == paddleRed.x + 2 && ball[index].y == 7)
    {
      ball[index].slope = abs(ball[index].slope);
//      Tone_Start(ToneB4, 50);
    }
  }
}
  

void SlopeChangeBlue(int ballindex)
{
  switch (paddleBlue.x - ball[ballindex].x)
  {
    case -2:
      if (ball[ballindex].slope < 0)
      {
        ball[ballindex].yvelocity = 1;
        ball[ballindex].slope += 2;
//        Tone_Start(ToneB4, 50);
      }
      break;
    case -1:
      ball[ballindex].yvelocity = 1;
      ball[ballindex].slope++;
//      Tone_Start(ToneB4, 50);
      break;
    case 0:
      ball[ballindex].yvelocity = 1;
      Tone_Start(ToneB4, 50);
      break;
    case 1:
      ball[ballindex].yvelocity = 1;
      ball[ballindex].slope--;
//      Tone_Start(ToneB4, 50);
      break;
    case 2:
      if (ball[ballindex].slope > 0)
      {
        ball[ballindex].yvelocity = 1;
        ball[ballindex].slope -= 2;
//        Tone_Start(ToneB4, 50);
      }
  }
}


void SlopeChangeRed(int ballindex)
{
  switch (paddleRed.x - ball[ballindex].x)
  {
    case -2:
      if (ball[ballindex].slope < 0)
      {
        ball[ballindex].yvelocity = -1;
        ball[ballindex].slope += 2;
//        Tone_Start(ToneB4, 50);
      }
      break;
    case -1:
      ball[ballindex].yvelocity = -1;
      ball[ballindex].slope++;
//      Tone_Start(ToneB4, 50);
      break;
    case 0:
      ball[ballindex].yvelocity = -1;
//      Tone_Start(ToneB4, 50);
      break;
    case 1:
      ball[ballindex].yvelocity = -1;
      ball[ballindex].slope--;
//      Tone_Start(ToneB4, 50);
      break;
    case 2:
      if (ball[ballindex].slope > 0)
      {
        ball[ballindex].yvelocity = -1;
        ball[ballindex].slope -= 2;
//        Tone_Start(ToneB4, 50);
      }
  }
}


void BlueVictory()
{
  for(int i=0;i<8;i++)      // run a blue fanefare
    for(int j=0;j<8;j++)
      DrawPx(i,j,Blue);
  DisplaySlate();
  Tone_Start(ToneC5,150);
  delay(150);
  Tone_Start(ToneE5,150);
  delay(150);
  Tone_Start(ToneC6,150);
  delay(150);
  blueScore++;      // give blue a point
  turn = 0;      // give red the ball
  MPReset();
}


void RedVictory()
{
  for(int i=0;i<8;i++)      // same thing but red
    for(int j=0;j<8;j++)
      DrawPx(i,j,Red);
  DisplaySlate();
  Tone_Start(ToneC5,150);
  delay(150);
  Tone_Start(ToneE5,150);
  delay(150);
  Tone_Start(ToneC6,150);
  delay(150);
  redScore++;
  turn = 1;
  MPReset();
}


void RedWin()
{
  for(int i=0;i<8;i++)    // run an extended fanfare, update the LEDs, and put
    for(int j=0;j<8;j++)  // a big red 'R' on the screen
      DrawPx(i,j,Red);
  DisplaySlate();
  LED1 = 0;
  LED2 = 0;
  for (int e = 0; e < blueScore; e++)
    LED1 = 2*LED1 + 1;
  for (int e = 0; e < redScore; e++)
    LED2 = LED2/2 + 128;
  SetAuxLEDs(LED1 + LED2);
  
  delay(600);
  Tone_Start(ToneC5,150);
  delay(150);
  Tone_Start(ToneE5,150);
  delay(150);
  Tone_Start(ToneG5,150);
  delay(150);
  Tone_Start(ToneF5,150);
  delay(150);
  Tone_Start(ToneA5,150);
  delay(150);
  Tone_Start(ToneC6,150);
  delay(150);
  Tone_Start(ToneG5,150);
  delay(150);
  Tone_Start(ToneB5,150);
  delay(150);
  Tone_Start(ToneD6,150);
  delay(150);
  Tone_Start(ToneC6,300);
  delay(600);
  
  ClearSlate();
  for (int q = 1; q < 7; q++)
    DrawPx(2, q, Red);
  for (int q = 1; q < 6; q++)
    DrawPx(5, q, Red);
  DrawPx(5, 3, Dark);
  for (int q = 3; q < 5; q++)
    for (int w = 3; w < 7; w += 3)
      DrawPx(q, w, Red);
  DisplaySlate();
  
  while (true)      // stop the game
  {
  }
}


void BlueWin()
{
  for(int i=0;i<8;i++)      // same thing, but blue
    for(int j=0;j<8;j++)
      DrawPx(i,j,Blue);
  DisplaySlate();
  LED1 = 0;
  LED2 = 0;
  for (int e = 0; e < blueScore; e++)
    LED1 = 2*LED1 + 1;
  for (int e = 0; e < redScore; e++)
    LED2 = LED2/2 + 128;
  SetAuxLEDs(LED1 + LED2);
  
  delay(600);
  Tone_Start(ToneC5,150);
  delay(150);
  Tone_Start(ToneE5,150);
  delay(150);
  Tone_Start(ToneG5,150);
  delay(150);
  Tone_Start(ToneF5,150);
  delay(150);
  Tone_Start(ToneA5,150);
  delay(150);
  Tone_Start(ToneC6,150);
  delay(150);
  Tone_Start(ToneG5,150);
  delay(150);
  Tone_Start(ToneB5,150);
  delay(150);
  Tone_Start(ToneD6,150);
  delay(150);
  Tone_Start(ToneC6,300);
  delay(600);
  
  ClearSlate();
  for (int q = 1; q < 7; q++)
    DrawPx(2, q, Blue);
  for (int q = 2; q < 6; q++)
    DrawPx(5, q, Blue);
  DrawPx(5, 4, Dark);
  for (int q = 3; q < 5; q++)
    for (int w = 4; w < 7; w += 2)
      DrawPx(q, w, Blue);
  DrawPx(3, 1, Blue);
  DrawPx(4, 1, Blue);
  DisplaySlate();
  
  while (true)
  {
  }
}


void Music()
{
  songt ++;
  if (songt%384 <= 256)
    switch (songt%128)
    {
      case 1:
        Tone_Start(ToneD5, 30);
        break;
      case 2:
        
        break;
      case 3:
        Tone_Start(ToneD5, 30);
        break;
      case 4:
        
        break;
      case 5:
        Tone_Start(ToneD5, 30);
        break;
      case 6:
        
        break;
      case 7:
        Tone_Start(ToneD5, 30);
        break;
      case 8:
        
        break;
      case 9:
        Tone_Start(ToneD5, 30);
        break;
      case 10:
        
        break;
      case 11:
        
        break;
      case 12:
        
        break;
      case 13:
        Tone_Start(ToneD5, 30);
        break;
      case 14:
        
        break;
      case 15:
        Tone_Start(ToneD5, 30);
        break;
      case 16:
        
        break;
      case 17:
        Tone_Start(ToneD5, 30);
        break;
      case 18:
        
        break;
      case 19:
        Tone_Start(ToneD5, 30);
        break;
      case 20:
        
        break;
      case 21:
        Tone_Start(ToneD5, 30);
        break;
      case 22:
        
        break;
      case 23:
        
        break;
      case 24:
        
        break;
      case 25:
        Tone_Start(ToneD5, 30);
        break;
      case 26:
        
        break;
      case 27:
        
        break;
      case 28:
        
        break;
      case 29:
        Tone_Start(ToneG5, 30);
        break;
      case 30:
        
        break;
      case 31:
        Tone_Start(ToneG5, 30);
        break;
      case 32:
        
        break;
      case 33:
        Tone_Start(ToneG5, 30);
        break;
      case 34:
        
        break;
      case 35:
        Tone_Start(ToneG5, 30);
        break;
      case 36:
        
        break;
      case 37:
        Tone_Start(ToneG5, 30);
        break;
      case 38:
        
        break;
      case 39:
        
        break;
      case 40:
        
        break;
      case 41:
        Tone_Start(ToneG5, 30);
        break;
      case 42:
        
        break;
      case 43:
        
        break;
      case 44:
        
        break;
      case 45:
        Tone_Start(ToneF5, 30);
        break;
      case 46:
        
        break;
      case 47:
        Tone_Start(ToneF5, 30);
        break;
      case 48:
        
        break;
      case 49:
        Tone_Start(ToneF5, 30);
        break;
      case 50:
        
        break;
      case 51:
        Tone_Start(ToneF5, 30);
        break;
      case 52:
        
        break;
      case 53:
        Tone_Start(ToneF5, 30);
        break;
      case 54:
        
        break;
      case 55:
        
        break;
      case 56:
        
        break;
      case 57:
        Tone_Start(ToneF5, 30);
        break;
      case 58:
        
        break;
      case 59:
        
        break;
      case 60:
        
        break;
      case 61:
        Tone_Start(ToneC5, 30);
        break;
      case 62:
        
        break;
      case 63:
        
        break;
      case 64:
        
        break;
      case 65:
        Tone_Start(ToneD5, 30);
        break;
      case 66:
        
        break;
      case 67:
        Tone_Start(ToneD5, 30);
        break;
      case 68:
        
        break;
      case 69:
        Tone_Start(ToneD5, 30);
        break;
      case 70:
        
        break;
      case 71:
        Tone_Start(ToneD5, 30);
        break;
      case 72:
        
        break;
      case 73:
        Tone_Start(ToneD5, 30);
        break;
      case 74:
        
        break;
      case 75:
        
        break;
      case 76:
        
        break;
      case 77:
        Tone_Start(ToneD5, 30);
        break;
      case 78:
        
        break;
      case 79:
        Tone_Start(ToneD5, 30);
        break;
      case 80:
        
        break;
      case 81:
        Tone_Start(ToneD5, 30);
        break;
      case 82:
        
        break;
      case 83:
        Tone_Start(ToneD5, 30);
        break;
      case 84:
        
        break;
      case 85:
        Tone_Start(ToneD5, 30);
        break;
      case 86:
        
        break;
      case 87:
        
        break;
      case 88:
        
        break;
      case 89:
        Tone_Start(ToneD5, 30);
        break;
      case 90:
        
        break;
      case 91:
        
        break;
      case 92:
        
        break;
      case 93:
        Tone_Start(ToneF5, 30);
        break;
      case 94:
        
        break;
      case 95:
        
        break;
      case 96:
        
        break;
      case 97:
        Tone_Start(ToneD5, 30);
        break;
      case 98:
        
        break;
      case 99:
        Tone_Start(ToneD5, 30);
        break;
      case 100:
        
        break;
      case 101:
        Tone_Start(ToneD5, 30);
        break;
      case 102:
        
        break;
      case 103:
        Tone_Start(ToneD5, 30);
        break;
      case 104:
        
        break;
      case 105:
        Tone_Start(ToneD5, 30);
        break;
      case 106:
        
        break;
      case 107:
        
        break;
      case 108:
        
        break;
      case 109:
        Tone_Start(ToneD5, 30);
        break;
      case 110:
        
        break;
      case 111:
        Tone_Start(ToneD5, 30);
        break;
      case 112:
        
        break;
      case 113:
        Tone_Start(ToneD5, 30);
        break;
      case 114:
        
        break;
      case 115:
        Tone_Start(ToneD5, 30);
        break;
      case 116:
        
        break;
      case 117:
        Tone_Start(ToneD5, 30);
        break;
      case 118:
        
        break;
      case 119:
        
        break;
      case 120:
        
        break;
      case 121:
        Tone_Start(ToneF5, 30);
        break;
      case 122:
        
        break;
      case 123:
        
        break;
      case 124:
        
        break;
      case 125:
        Tone_Start(ToneF5, 30);
        break;
      case 126:
        
        break;
      case 127:
        
        break;
      case 0:
        
        break;
    }
    
  else
    switch (songt%128)
    {
      case 1:
        Tone_Start(ToneD5, 30);
        break;
      case 2:
        
        break;
      case 3:
        
        break;
      case 4:
        
        break;
      case 5:
        Tone_Start(ToneD5, 30);
        break;
      case 6:
        
        break;
      case 7:
        
        break;
      case 8:
        
        break;
      case 9:
        Tone_Start(ToneF5, 30);
        break;
      case 10:
        
        break;
      case 11:
        
        break;
      case 12:
        
        break;
      case 13:
        Tone_Start(ToneF5, 30);
        break;
      case 14:
        
        break;
      case 15:
        
        break;
      case 16:
        
        break;
      case 17:
        Tone_Start(ToneD5, 30);
        break;
      case 18:
        
        break;
      case 19:
        
        break;
      case 20:
        
        break;
      case 21:
        Tone_Start(ToneD5, 30);
        break;
      case 22:
        
        break;
      case 23:
        
        break;
      case 24:
        
        break;
      case 25:
        Tone_Start(ToneF5, 30);
        break;
      case 26:
        
        break;
      case 27:
        
        break;
      case 28:
        
        break;
      case 29:
        Tone_Start(ToneF5, 30);
        break;
      case 30:
        
        break;
      case 31:
        
        break;
      case 32:
        
        break;
      case 33:
        Tone_Start(ToneD5, 30);
        break;
      case 34:
        
        break;
      case 35:
        
        break;
      case 36:
        
        break;
      case 37:
        Tone_Start(ToneD5, 30);
        break;
      case 38:
        
        break;
      case 39:
        
        break;
      case 40:
        
        break;
      case 41:
        Tone_Start(ToneF5, 30);
        break;
      case 42:
        
        break;
      case 43:
        
        break;
      case 44:
        
        break;
      case 45:
        Tone_Start(ToneF5, 30);
        break;
      case 46:
        
        break;
      case 47:
        
        break;
      case 48:
        
        break;
      case 49:
        Tone_Start(ToneD5, 30);
        break;
      case 50:
        
        break;
      case 51:
        
        break;
      case 52:
        
        break;
      case 53:
        Tone_Start(ToneD5, 30);
        break;
      case 54:
        
        break;
      case 55:
        
        break;
      case 56:
        
        break;
      case 57:
        Tone_Start(ToneF5, 30);
        break;
      case 58:
        
        break;
      case 59:
        
        break;
      case 60:
        
        break;
      case 61:
        Tone_Start(ToneF5, 30);
        break;
      case 62:
        
        break;
      case 63:
        
        break;
      case 64:
        
        break;
      case 65:
        Tone_Start(ToneD5, 30);
        break;
      case 66:
        
        break;
      case 67:
        
        break;
      case 68:
        
        break;
      case 69:
        Tone_Start(ToneF5, 30);
        break;
      case 70:
        
        break;
      case 71:
        
        break;
      case 72:
        
        break;
      case 73:
        Tone_Start(ToneD5, 30);
        break;
      case 74:
        
        break;
      case 75:
        
        break;
      case 76:
        
        break;
      case 77:
        Tone_Start(ToneF5, 30);
        break;
      case 78:
        
        break;
      case 79:
        
        break;
      case 80:
        
        break;
      case 81:
        Tone_Start(ToneD5, 30);
        break;
      case 82:
        
        break;
      case 83:
        
        break;
      case 84:
        
        break;
      case 85:
        Tone_Start(ToneF5, 30);
        break;
      case 86:
        
        break;
      case 87:
        
        break;
      case 88:
        
        break;
      case 89:
        Tone_Start(ToneD5, 30);
        break;
      case 90:
        
        break;
      case 91:
        
        break;
      case 92:
        
        break;
      case 93:
        Tone_Start(ToneF5, 30);
        break;
      case 94:
        
        break;
      case 95:
        
        break;
      case 96:
        
        break;
      case 97:
        Tone_Start(ToneD5, 30);
        break;
      case 98:
        
        break;
      case 99:
        Tone_Start(ToneF5, 30);
        break;
      case 100:
        
        break;
      case 101:
        Tone_Start(ToneD5, 30);
        break;
      case 102:
        
        break;
      case 103:
        Tone_Start(ToneF5, 30);
        break;
      case 104:
        
        break;
      case 105:
        Tone_Start(ToneD5, 30);
        break;
      case 106:
        
        break;
      case 107:
        Tone_Start(ToneF5, 30);
        break;
      case 108:
        
        break;
      case 109:
        Tone_Start(ToneD5, 30);
        break;
      case 110:
        
        break;
      case 111:
        Tone_Start(ToneF5, 30);
        break;
      case 112:
        
        break;
      case 113:
        Tone_Start(ToneD5, 30);
        break;
      case 114:
        
        break;
      case 115:
        Tone_Start(ToneD5, 30);
        break;
      case 116:
        Tone_Start(ToneF5, 30);
        break;
      case 117:
        Tone_Start(ToneD5, 30);
        break;
      case 118:
        Tone_Start(ToneF5, 30);
        break;
      case 119:
        Tone_Start(ToneD5, 30);
        break;
      case 120:
        Tone_Start(ToneF5, 30);
        break;
      case 121:
        Tone_Start(ToneD5, 30);
        break;
      case 122:
        Tone_Start(ToneF5, 30);
        break;
      case 123:
        Tone_Start(ToneD5, 30);
        break;
      case 124:
        Tone_Start(ToneF5, 30);
        break;
      case 125:
        Tone_Start(ToneD5, 30);
        break;
      case 126:
        Tone_Start(ToneF5, 30);
        break;
      case 127:
        Tone_Start(ToneD5, 30);
        break;
      case 0:
        Tone_Start(ToneF5, 30);
        break;
    }
}
