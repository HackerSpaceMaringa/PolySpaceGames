#include <TVout.h>
#include <fontALL.h>
#include "schematic.h"
#include "TVOlogo.h"

#define  HRES  144      //Resolucao TV
#define  VRES  96       //
#define  TV_MODE NTSC
#define  REFRESH_RATE 5
#define  FONT8 font8x8  //Fonte padrao
#define  PLAYER1 2      //Potenciometro player 1
#define  PLAYER2 1      //Potenciometro player 2

TVout TV;

int xb = 60;
int yb = 60;
int xj1 = 0;
int xj2 = 0;

int i = 1;
int j = 1;

int ponto1 = 0;
int ponto2 = 0;

void menu() { 
     
  int prev = -1;
  int option = analogRead(1) + analogRead(1) + analogRead(1);
  option = map(option/100,0,20,3,8);
  
  while(true){
       
    TV.println(0,0 ,"*---------------*\n");
    TV.println(0,8 ,"|  Hackstation  |\n");
    TV.println(0,16,"*---------------*\n");
    TV.println(0,24,"| Pog-Pong      |\n");
    TV.println(0,32,"|               |\n");
    TV.println(0,40,"|               |\n");
    TV.println(0,48,"|               |\n");
    TV.println(0,56,"|               |\n");
    TV.println(0,64,"| Exit          |\n");
    TV.println(0,72,"*---------------*\n");
    
    TV.set_cursor(8,option*8);
    TV.print(">");
    
    while(option == prev){
      option = analogRead(PLAYER1) + analogRead(PLAYER1) + analogRead(PLAYER1);
      option = map(option/100,0,20,3,8);
      delay(REFRESH_RATE);
      
      if(option == 3 && digitalRead(2) == HIGH) {
        return;
      }
      
      if(option == 8 && digitalRead(2) == HIGH) {
        TV.clear_screen();
        TV.println(30,0 ,"The zueira");
        TV.println(30,10 ,"NEVER");
        TV.println(30,20 ,"ENDS");
       
        delay(5000);
        TV.select_font(font4x6);
        for(int k=0; k < 80; k++){
          TV.select_font(font4x6);
          TV.println(17, k, "HACKERSPACE MARINGA");
          TV.select_font(font8x8);

          TV.println(24, k+10, "POG PONG");
    
          delay(5);
          TV.clear_screen();
  
        }
        break;
      }
      
    }
    prev = option;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
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

  
  menu();

}

boolean pausado = false;

int speed = 25;
void loop() {
    
    if(pausado) {
      if (digitalRead(2) == HIGH) { 
        pausado = false;
        delay(100);
      }
      return;
    }
    
    if (digitalRead(2) == HIGH) pausado = true;
    
    if (digitalRead(3) == HIGH) {
      xb = 60;
      yb = 60;
      xj1 = 0;
      xj2 = 0;
      i = 1;
      j = 1;
      ponto2 = 0;
      ponto1 = 0;
      speed = 25;
      menu();
      return;
    }
    
    int a = analogRead(1) + analogRead(1) + analogRead(1);
    delay(1);
    a += analogRead(1) + analogRead(1) + analogRead(1);
    
    int b = analogRead(2) + analogRead(2) + analogRead(2);
    
    delay(1);
    b += analogRead(2) + analogRead(2) + analogRead(2);
    
    if(b <= 0) b = 0;
    if(a <= 0) a = 0;
    
    TV.clear_screen();
    TV.println(xb, yb,".");
    TV.println(0, (a/6)/8,"|");
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
