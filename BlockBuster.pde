/*
  BlockBuster.pde
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
   Oh, no!  Earth is being invaded by round, stationary spaceships!  Prepare
   to take on the armada with your trusty, deadly... ping-pong paddle?
   Advance through the waves, collect powerups, and save the human race!  Just
   be sure not to lose your ball!
 
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

struct point {int x; int y;};
point ball = {3, 2};
point paddle = {3, 0};
int slope = 1;
int yvelocity = 1;
int level = 1;
boolean moved = false;
int timer = 0;


void setup() 
{
  MeggyJrSimpleSetup(); 
  Serial.begin(9600);
  reset();
}


void loop() 
{
  DrawPx(ball.x, ball.y, Dark);
  for(int j = -1; j < 2; j++)
    DrawPx(paddle.x + j, paddle.y, Dark);
  
  if(timer % 30 == 0)
  {
    BounceY();
    ball.y += yvelocity;
  }
  
  if(timer % (90/slope) == 0)
  {
    BounceX();
    if(slope > 0)
      ball.x++;
    if(slope < 0)
      ball.x--;
  }
  
  MovePress();
  
  if(timer % 30 == 0)
    MoveHold();
  
  for(int j = -1; j < 2; j++)
    DrawPx(paddle.x + j, paddle.y, Blue);
  DrawPx(ball.x, ball.y, White);
  DisplaySlate();
  
  delay(10);
  
  timer++;
}


void reset()
{
  ClearSlate();
  ball.x = 3;
  ball.y = 2;
  paddle.x = 3;
  paddle.y = 0;
  slope = 3;
  yvelocity = 1;
  moved = false;
  timer = 0;
  switch (level)
  {
    case 1:
      level1();
      Serial.println("Initiating level 1");
  }
}


void MovePress()
{
  CheckButtonsPress();
  
  if (Button_Right && paddle.x < 6)
  {
    paddle.x++;
    moved = true;
  }
    
  if (Button_Left && paddle.x > 1)
  {
    paddle.x--;
    moved = true;
  }
}


void MoveHold()
{
  if (!moved)
  {
    CheckButtonsDown();
  
    if (Button_Right && paddle.x < 6)
      paddle.x++;
  
    if (Button_Left && paddle.x > 1)
      paddle.x--;
  }
    
  moved = false;
}


void BounceY()
{
  if (ball.y > 6)
    yvelocity = -1;
  if (ball.y < 2)
    PaddleCollision();
  if (ball.y < 1)
    GameOver();
}


void BounceX()
{
  if (ball.x > 6)
    slope = 0 - slope;
  if (ball.x < 1)
    slope = 0 - slope;
}


void PaddleCollision()
{
  switch (paddle.x - ball.x)
  {
    case -1:
      yvelocity = 1;
      slope++;
      break;
    case 0:
      yvelocity = 1;
      break;
    case 1:
      yvelocity = 1;
      slope--;
  }
}


void GameOver()
{
  DisplaySlate();
  delay(200);
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      DrawPx(i,j,Red);
  DisplaySlate();
  Tone_Start(ToneB2,400);
  delay(500);
  Tone_Start(ToneB2,400);
  delay(500);
  Tone_Start(ToneB2,500);
  delay(500);
  reset();
}


void level1()
{
  for (int i = 2; i < 6; i++)
    for (int k = 4; k < 8; k++)
       DrawPx(i, k, Red);
}
