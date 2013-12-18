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
boolean oneup;
int timer;


void setup() 
{
  MeggyJrSimpleSetup(); 
  Serial.begin(9600);
  EditColor(White, 13, 4, 3);
  EditColor(CustomColor0, 15, 0, 0);
  EditColor(CustomColor1, 10, 0, 10);
  EditColor(CustomColor2, 10, 0, 10);
  EditColor(CustomColor3, 10, 0, 10);
  level = 1;
  reset();
}


void loop() 
{
  EraseObjects();
  
  UpdateBall();
  
  UpdatePowerups();
  
  UpdatePaddle();
  
  DrawObjects();
  
  delay(10);
  
  if (YouHaveWon())
    RunVictory();
  
  timer++;
}


void reset()
{
  Serial.print("Now executing reset for level ");
  Serial.println(level);
  ClearSlate();
  for (int i = 0; i < 3; i++)
  {
    ball[i].x = random(6)+1;
    ball[i].y = 2;
    ball[i].slope = 2;
    ball[i].yvelocity = 1;
    ball[i].inplay = false;
  }
  ball[0].inplay = true;
  paddle.x = 3;
  paddle.y = 0;
  moved = false;
  oneup = false;
  timer = 0;
  switch (level)
  {
    case 1:
      Level1();
      Serial.println("Initiating level 1");
      break;
    case 2:
      Level2();
      Serial.println("Initiating level 2");
      break;
    case 3:
      Level3();
      Serial.println("Initiating level 3");
      break;
    case 4:
      Level4();
      Serial.println("Initiating level 4");
      break;
  }
}


void EraseObjects()
{
  for (int v = 0; v < 3; v++)
    if (ball[v].inplay)
      DrawPx(ball[v].x, ball[v].y, Dark);
  for(int j = -1; j < 2; j++)
    DrawPx(paddle.x + j, paddle.y, Dark);
}


void DrawObjects()
{
  for (int u = 0; u < 3; u++)
    if (ball[u].inplay)
      DrawPx(ball[u].x, ball[u].y, White);
  for(int j = -1; j < 2; j++)
    DrawPx(paddle.x + j, paddle.y, Blue);
  DisplaySlate();
}


void UpdatePowerups()
{
  if (timer % 20 == 0)
  {
    for(int b = 0; b < 8; b++)
      for(int c = 0; c < 8; c++)
        if(ReadPx(b, c) == CustomColor2)
        {
          DrawPx(b, c, Dark);
          DrawPx(b, c - 1, CustomColor2);
        }
      
    for(int b = 0; b < 8; b++)
      for(int c = 0; c < 8; c++)
        if(ReadPx(b, c) == CustomColor1)
          DrawPx(b, c, Dark);
  }
  for (int v = -1; v < 2; v++)
    if (ReadPx(paddle.x + v, paddle.y) == CustomColor2)
      switch (random(4))
      {
        case 0:
          MultiBall();
          break;
        case 1:
          InstaLaser();
          break;
        default:
          OneUp();
      }  
}


void UpdateBall()
{
  for (int n = 0; n < 3; n++)
    if (ball[n].inplay)
    {
      if(timer % (60/ball[n].slope) == 0)
        BounceX(n);
    
      if (timer % 30 == 0)
        BounceY(n);
    
      if (timer % (60/ball[n].slope) == 0 && timer % 30 == 0)
        BounceDiagonal(n);
  
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
    }
  
  if (!ball[0].inplay & !ball[1].inplay & !ball[2].inplay)
    GameOver();
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
  MovePress();
  
  if (timer % 10 == 0)
    MoveHold();
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


void BounceY(int index)
{
  if (ball[index].yvelocity > 0)
  {
    if (ball[index].y > 6)
      ball[index].yvelocity = -1;
    if (ReadPx(ball[index].x, ball[index].y+1) > 0)
    {
      ball[index].yvelocity = -1;
      DrawPx(ball[index].x, ball[index].y+1, ReadPx(ball[index].x, ball[index].y+1) - 1);
    }
  }
  if (ball[index].yvelocity < 0)
  {
    if (ReadPx(ball[index].x, ball[index].y-1) > 0)
    {
      ball[index].yvelocity = 1;
      DrawPx(ball[index].x, ball[index].y-1, ReadPx(ball[index].x, ball[index].y-1) - 1);
    }
    if (ball[index].y < 2)
      SlopeChange(index);
  }
  if (ball[index].y < 1)
    ball[index].inplay = false;
}


void BounceX(int index)
{
  if (ball[index].slope > 0)
  {
    if (ball[index].x > 6)
      ball[index].slope = -ball[index].slope;
    if (ReadPx(ball[index].x+1, ball[index].y) > 0)
    {
      ball[index].slope = -ball[index].slope;
      DrawPx(ball[index].x+1, ball[index].y, ReadPx(ball[index].x+1, ball[index].y) - 1);
    }
  }
  if (ball[index].slope < 0)
  {
    if (ball[index].x < 1)
      ball[index].slope = abs(ball[index].slope);
    if (ReadPx(ball[index].x-1, ball[index].y) > 0)
    {
      ball[index].slope = abs(ball[index].slope);
      DrawPx(ball[index].x-1, ball[index].y, ReadPx(ball[index].x-1, ball[index].y) - 1);
    }
  }
}


void BounceDiagonal(int index)
{
  if (ball[index].slope > 0 && ball[index].yvelocity > 0)
    if (ReadPx(ball[index].x+1, ball[index].y+1) > 0)
    {
      ball[index].slope = -ball[index].slope;
      ball[index].yvelocity = -1;
      DrawPx(ball[index].x+1, ball[index].y+1, ReadPx(ball[index].x+1, ball[index].y+1)-1);
    }
  
  if (ball[index].slope > 0 && ball[index].yvelocity < 0)
    if (ReadPx(ball[index].x+1, ball[index].y-1) > 0)
    {
      ball[index].slope = -ball[0].slope;
      ball[index].yvelocity = 1;
      DrawPx(ball[index].x+1, ball[index].y-1, ReadPx(ball[index].x+1, ball[index].y-1)-1);
    }
  
  if (ball[index].slope < 0 && ball[index].yvelocity > 0)
    if (ReadPx(ball[index].x-1, ball[index].y+1) > 0)
    {
      ball[index].slope = abs(ball[index].slope);
      ball[index].yvelocity = -1;
      DrawPx(ball[index].x-1, ball[index].y+1, ReadPx(ball[index].x-1, ball[index].y+1)-1);
    }
  
  if (ball[index].slope < 0 && ball[index].yvelocity < 0)
    if (ReadPx(ball[index].x-1, ball[index].y-1) > 0)
    {
      ball[index].slope = abs(ball[index].slope);
      ball[index].yvelocity = 1;
      DrawPx(ball[index].x-1, ball[index].y-1, ReadPx(ball[index].x-1, ball[index].y-1)-1);
    }
}
  

void SlopeChange(int ballindex)
{
  switch (paddle.x - ball[ballindex].x)
  {
    case -2:
      if (ball[ballindex].slope < 1)
      {
        ball[ballindex].yvelocity = 1;
        ball[ballindex].slope += 2;
      }
      break;
    case -1:
      ball[ballindex].yvelocity = 1;
      ball[ballindex].slope++;
      break;
    case 0:
      ball[ballindex].yvelocity = 1;
      break;
    case 1:
      ball[ballindex].yvelocity = 1;
      ball[ballindex].slope--;
      break;
    case 2:
      if (ball[ballindex].slope > 1)
      {
        ball[ballindex].yvelocity = 1;
        ball[ballindex].slope -= 2;
      }
  }
}


void MultiBall()
{
  for (int g = 0; g < 3; g++)
    if (ball[g].inplay)
    {
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
}


void InstaLaser()
{
  DrawObjects();
  
  for (int h = 0; h < 8; h++)
  {
    DrawPx(paddle.x, h, FullOn);
    DisplaySlate();
    delay(20);
  }
  
  for (int h = 0; h < 8; h++)
  {
    DrawPx(paddle.x, h, Dark);
    DisplaySlate();
    delay(20);
  }
}


void OneUp()
{
  EditColor(White, 15, 12, 4);
  oneup = true;
}


boolean YouHaveWon()
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
  Serial.print("You beat level ");
  Serial.print(level);
  Serial.println("!");
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
  Serial.print("Incrementing to level ");
  Serial.println(level + 1);
  level++;
  Serial.print("You are now on level ");
  Serial.println(level);
  reset();
}


void GameOver()
{
  if (oneup)
  {
    oneup = false;
    EditColor(White, 13, 4, 3);
    ball[0].inplay = true;
    ball[0].yvelocity = 1;
    ball[0].x = 3;
    ball[0].y = 2;
  }
  else
  {
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


void Level1()
{
  for (int i = 2; i < 6; i++)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Red);
  DrawPx(3, 6, CustomColor3);
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
  for (int i = 1; i < 7; i+=5)
    for (int k = 5; k < 8; k++)
       DrawPx(i, k, Orange);
  for (int i = 1; i < 7; i++)
    DrawPx(i, 4, Yellow);
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
}
