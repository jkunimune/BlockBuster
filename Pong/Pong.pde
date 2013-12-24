/*
  BlockBuster.pde
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
   A classic pong game.  Fight to the death with your opponent.  First one to
   drop the ball loses his head.  Okay, maybe just his dignity.
   First person to get four points becomes the Ultimate Block Buster!
 
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
point paddleBlue = {3, 0};
point paddleRed = {4, 7};
int level;
int difficulty;
boolean blueMoved;
boolean redMoved;
boolean started;
boolean oneup;
int onfire;
int waittime;
int LED1;
int LED2;
int timer;
int failures;
int redScore;
int blueScore;
int turn;


void setup()
{
  MeggyJrSimpleSetup(); 
  Serial.begin(9600);
  EditColor(White, 13, 4, 3);
  EditColor(CustomColor0, 15, 0, 0);
  EditColor(CustomColor1, 13, 0, 15);
  EditColor(CustomColor2, 13, 0, 15);
  EditColor(CustomColor3, 13, 0, 15);
  EditColor(CustomColor4, 10, 0, 10);
  failures = 0;
  level = 1;
  difficulty = 1;
  blueScore = 0;
  redScore = 0;
  turn = 1;
  MPReset();
}


void loop() 
{ 
  MPEraseObjects();
  
  MPUpdateBall();
  
  MPUpdatePaddle();

  MPDrawObjects();
  
  delay(waittime);
  
  if (Button_Up || Button_Down)
    started = true;
    
  timer++;
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
  paddleBlue.x = 3;
  paddleRed.x = 4;
  blueMoved = false;
  redMoved = false;
  started = false;
  oneup = false;
  onfire = 0;
  waittime = 5;
  EditColor(White, 13, 4, 3);
  timer = 0;
  
  if (blueScore >= 4)
    BlueWin();
  if (redScore >= 4)
    RedWin();
}


void MPEraseObjects()
{
  for (int v = 0; v < 3; v++)
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
  
  LED1 = 0;
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
            ball[n].y++;
      }
  }
  
  else
  {
    if (turn)
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
      Tone_Start(ToneB4, 50);
    }
    
    if (ball[index].x == paddleBlue.x - 2 && ball[index].y == 0)
    {
      ball[index].slope = -ball[index].slope;
      Tone_Start(ToneB4, 50);
    }
    
    if (ball[index].x == paddleRed.x - 2 && ball[index].y == 7)
    {
      ball[index].slope = -ball[index].slope;
      Tone_Start(ToneB4, 50);
    }
  }
  
  if (ball[index].slope < 0)
  {
    if (ball[index].x < 1)
      ball[index].slope = abs(ball[index].slope);
      
    if (ball[index].x < 1)
      Tone_Start(ToneB4, 50);
      
    if (ball[index].x == paddleBlue.x + 2 && ball[index].y == 0)
    {
      ball[index].slope = abs(ball[index].slope);
      Tone_Start(ToneB4, 50);
    }
    
    if (ball[index].x == paddleRed.x + 2 && ball[index].y == 7)
    {
      ball[index].slope = abs(ball[index].slope);
      Tone_Start(ToneB4, 50);
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
        Tone_Start(ToneB4, 50);
      }
      break;
    case -1:
      ball[ballindex].yvelocity = 1;
      ball[ballindex].slope++;
      Tone_Start(ToneB4, 50);
      break;
    case 0:
      ball[ballindex].yvelocity = 1;
      Tone_Start(ToneB4, 50);
      break;
    case 1:
      ball[ballindex].yvelocity = 1;
      ball[ballindex].slope--;
      Tone_Start(ToneB4, 50);
      break;
    case 2:
      if (ball[ballindex].slope > 0)
      {
        ball[ballindex].yvelocity = 1;
        ball[ballindex].slope -= 2;
        Tone_Start(ToneB4, 50);
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
        Tone_Start(ToneB4, 50);
      }
      break;
    case -1:
      ball[ballindex].yvelocity = -1;
      ball[ballindex].slope++;
      Tone_Start(ToneB4, 50);
      break;
    case 0:
      ball[ballindex].yvelocity = -1;
      Tone_Start(ToneB4, 50);
      break;
    case 1:
      ball[ballindex].yvelocity = -1;
      ball[ballindex].slope--;
      Tone_Start(ToneB4, 50);
      break;
    case 2:
      if (ball[ballindex].slope > 0)
      {
        ball[ballindex].yvelocity = -1;
        ball[ballindex].slope -= 2;
        Tone_Start(ToneB4, 50);
      }
  }
}


void BlueVictory()
{
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      DrawPx(i,j,Blue);
  DisplaySlate();
  Tone_Start(ToneC5,150);
  delay(150);
  Tone_Start(ToneE5,150);
  delay(150);
  Tone_Start(ToneC6,150);
  delay(150);
  blueScore++;
  turn = 0;
  MPReset();
}


void RedVictory()
{
  for(int i=0;i<8;i++)
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
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
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
  
  while (true)
  {
  }
}


void BlueWin()
{
  for(int i=0;i<8;i++)
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
  
  while (0 != 1)
  {
  }
}
