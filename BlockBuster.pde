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
struct projectile {int x; int y; int slope; int yvelocity; boolean inplay;};
projectile ball[3] = {{4, 2, 3, 1, true}};
point paddle = {3, 0};
int level;
boolean moved;
int timer;


void setup() 
{
  MeggyJrSimpleSetup(); 
  Serial.begin(9600);
  EditColor(CustomColor0, 15, 0, 0);
  level = 1;
  reset();
}


void loop() 
{
  if (ball[0].inplay)
    DrawPx(ball[0].x, ball[0].y, Dark);
  for(int j = -1; j < 2; j++)
    DrawPx(paddle.x + j, paddle.y, Dark);
  
  if (timer % 30 == 0)
  {
    BounceY();
    ball[0].y += ball[0].yvelocity;
  }
  
  if(timer % (90/ball[0].slope) == 0)
  {
    BounceX();
    if (ball[0].slope > 0)
      ball[0].x++;
    if (ball[0].slope < 0)
      ball[0].x--;
  }
  
  MovePress();
  
  if (timer % 10 == 0)
    MoveHold();
  
  for(int j = -1; j < 2; j++)
    DrawPx(paddle.x + j, paddle.y, Blue);
  if (ball[0].inplay)
    DrawPx(ball[0].x, ball[0].y, White);
  DisplaySlate();
  
  delay(10);
  
  if (CheckVictory())
    RunVictory();
  
  timer++;
}


void reset()
{
  ClearSlate();
  for (int i = 0; i < 3; i++)
  {
    ball[i].x = random(6)+1;
    ball[i].y = 2;
    ball[i].slope = 3;
    ball[i].yvelocity = 1;
    ball[i].inplay = false;
  }
    ball[0].inplay = true;
  paddle.x = 3;
  paddle.y = 0;
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
  if (ball[0].y > 6 || ReadPx(ball[0].x, ball[0].y+1) > 0)
    ball[0].yvelocity = -1;
  if (ReadPx(ball[0].x, ball[0].y-1) > 0)
    ball[0].yvelocity = 1;
  if (ball[0].y < 2)
    SlopeChange();
  if (ball[0].y < 1)
    ball[0].inplay = false;
  if (!ball[0].inplay & !ball[1].inplay & !ball[2].inplay)
    GameOver();
}


void BounceX()
{
  if (ball[0].x > 6 || ReadPx(ball[0].x+1, ball[0].y) > 0)
    ball[0].slope = 0 - abs(ball[0].slope);
  if (ball[0].x < 1 || ReadPx(ball[0].x-1, ball[0].y) > 0)
    ball[0].slope = abs(ball[0].slope);
}


void SlopeChange()
{
  switch (paddle.x - ball[0].x)
  {
    case -2:
      if (ball[0].slope < 1)
      {
        ball[0].yvelocity = 1;
        ball[0].slope += 2;
      }
      break;
    case -1:
      ball[0].yvelocity = 1;
      ball[0].slope++;
      break;
    case 0:
      ball[0].yvelocity = 1;
      break;
    case 1:
      ball[0].yvelocity = 1;
      ball[0].slope--;
      break;
    case 2:
      if (ball[0].slope > 1)
      {
        ball[0].yvelocity = 1;
        ball[0].slope -= 2;
      }
  }
}


boolean CheckVictory()
{
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
      DrawPx(i,j,CustomColor0);
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
