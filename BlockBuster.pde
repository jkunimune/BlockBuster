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
point ball = {4, 2};
point paddle = {3, 0};
int slope;
int yvelocity;
int level;
boolean moved;
int timer;


void setup() 
{
  MeggyJrSimpleSetup(); 
  Serial.begin(9600);
  level = 1;
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
  
  if(timer % 10 == 0)
    MoveHold();
  
  for(int j = -1; j < 2; j++)
    DrawPx(paddle.x + j, paddle.y, Blue);
  DrawPx(ball.x, ball.y, White);
  DisplaySlate();
  
  delay(10);
  
  if (CheckVictory())
    RunVictory();
  
  timer++;
}


void reset()
{
  ClearSlate();
  ball.x = 4;
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
      break;
    case 2:
      level2();
      Serial.println("Initiating level 2");
      break;
    case 3:
      level3();
      Serial.println("Initiating level 3");
      break;
    case 4:
      level4();
      Serial.println("Initiating level 4");
      break;
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
    case -2:
      if (slope < 1)
      {
        yvelocity = 1;
        slope++;
      }
      break;
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
      break;
    case 2:
      if (slope > 1)
      {
        yvelocity = 1;
        slope--;
      }
  }
}


boolean CheckVictory()
{
  for (int p = 0; p < 8; p++)
    for (int o = 0; o < 8; o++)
      if (ReadPx(o, p) == Red || ReadPx(o, p) == Green)
        return false;
  return true;
}


void RunVictory()
{
  for(int i=0;i<8;i++)
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
  reset();
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
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Red);
}


void level2()
{
  for (int i = 2; i < 6; i++)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Red);
  for (int i = 2; i < 6; i++)
    DrawPx(i, 4, Orange);
}


void level3()
{
  for (int i = 2; i < 6; i++)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Red);
  for (int i = 1; i < 7; i+=5)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Orange);
  for (int i = 1; i < 7; i++)
    DrawPx(i, 4, Yellow);
}


void level4()
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
}
