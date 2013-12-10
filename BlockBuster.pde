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
int level = 0;
int timer = 0;


void setup() 
{
  MeggyJrSimpleSetup(); 
  reset();
}


void loop() 
{
  DrawPx(ball.x, ball.y, Dark);
  
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
  
  DrawPx(ball.x, ball.y, White);
  DisplaySlate();
  
  delay(5);
  
  timer++;
}


void reset()
{
  ClearSlate();
  ball.x = 3;
  ball.y = 2;
  paddle.x = 3;
  paddle.y = 0;
  slope = 5;
  yvelocity = 1;
  timer = 0;
  switch (level)
  {
    case 1:
      level1();
  }
}


void BounceY()
{
  if (ball.y > 6)
    yvelocity = -1;
  if (ball.y < 1)
    yvelocity = 1;
}

void BounceX()
{
  if (ball.x > 6)
    slope = 0 - slope;
  if (ball.x < 1)
    slope = 0 - slope;
}


void level1()
{
  for (int i = 2; i < 6; i++)
    for (int k = 4; k < 8; k++)
       DrawPx(i, k, Red);
}
