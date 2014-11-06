#include <TVout.h>
#include <fontALL.h>
#include "schematic.h"
#include "TVOlogo.h"

TVout TV;

#define  HRES  144      //Resolucao TV
#define  VRES  96       //
#define  TV_MODE NTSC
#define  REFRESH_RATE 5
#define  FONT8 font8x8  //Fonte padrao
#define  PLAYER1 2      //Potenciometro player 1
#define  PLAYER2 1      //Potenciometro player 2

void setup() {
  TV.begin(TV_MODE,HRES,VRES);
  randomSeed(analogRead(0));
  TV.select_font(FONT8);
}

void loop() {

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
    }
    prev = option;
  }
}
