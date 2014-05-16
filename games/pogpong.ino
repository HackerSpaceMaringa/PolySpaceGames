#include <TVout.h>
#include <fontALL.h>
#include "schematic.h"
#include "TVOlogo.h"

TVout TV;

int xb = 60;
int yb = 60;
int xj1 = 0;
int xj2 = 0;

int i = 1;
int j = 1;

int ponto1 = 0;
int ponto2 = 0;

void setup() {
  TV.begin(NTSC,120,96);
  randomSeed(analogRead(0));
  TV.select_font(font4x6);
  for(int k=0; k < 80; k++){
    TV.select_font(font4x6);
    TV.println(17, k, "HACKERSPACE MARINGA");
     TV.select_font(font8x8);

    TV.println(24, k+10, "POG PONG");
    
    delay(5);
    TV.clear_screen();
  
  }
   TV.select_font(font6x8);
}

int speed = 25;
void loop() {

    int a = analogRead(1) + analogRead(1) + analogRead(1);
    delay(1);
    a += analogRead(1) + analogRead(1) + analogRead(1);
    
    int b = analogRead(2) + analogRead(2) + analogRead(2);
    
    delay(1);
    b += analogRead(2) + analogRead(2) + analogRead(2);
    TV.clear_screen();
    TV.println(xb, yb,".");
    TV.println(0,   (a/6)/8,"|");
    TV.println(110, (b/6)/8,"|");
    TV.select_font(font4x6);
    TV.println(33, 1, ponto1);
    TV.println(53, 1, "VS");
    TV.println(78, 1, ponto2);
    TV.select_font(font6x8);
    xb += i;
    yb += j;
    if(yb == 0 || yb == 90) j *= -1;
    if(( xb == 2) && abs(yb-(a/6)/8) <= 7) { 
      i *= -1;
      speed -= 4;
    }else if(( xb == 105) && abs(yb-(b/6)/8) <= 7) { 
      i *= -1;
      speed -= 4;
    } else if(xb == 110) {
      xb = 60;
      yb = 60;
      xj1 = 0;
      xj2 = 0;
      i = 1;
      j = 1;
      speed = 25;
      ponto1++;
    } else if(xb == 0 ) {
      xb = 60;
      yb = 60;
      xj1 = 0;
      xj2 = 0;
      i = 1;
      j = 1;
      speed = 25;
      ponto2++;
    }
    if(speed < 0) speed = 1;
    delay(speed);
    
}
