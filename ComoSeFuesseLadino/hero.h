#ifndef HERO_H
#define HERO_H
#include "Arduino.h"
#include "map.h"

//classes
#define RANGER         0
#define BARBARIAN      1
#define WIZARD         2
#define MONSTER        2

//initial lifes
#define R_INIT_HP      45
#define B_INIT_HP      70
#define W_INIT_HP      35
#define M_INIT_HP      4

//initial mana
#define R_INIT_MP      13
#define B_INIT_MP      12
#define W_INIT_MP      15


//speed of actions
#define SPD_RANGER     11
#define SPD_BARBARIAN  11
#define SPD_WIZARD     11
#define SPD_MONSTER    10

#define UP            0 
#define LEFT          1  
#define DOWN          2 
#define RIGHT         3 

#define TICKET        15

extern int num_monster;

class Position {
public:
  byte i;
  byte j;
  byte d;
};

extern Position camH;
extern Position camV;

class Projetil {
  public:
    byte SPD;
    byte ATK;
    Position POS;
    int TKT;
    byte A;
    
    Projetil() {
      A = 0;
    }
};

class Hero {
public:
  Position POS;
  int HP,MP,MAX_MP, MAX_HP;
  byte ATK, HERO_CLASS, LVL, SPD, TKT, KILLS, AGRS;
  Hero(byte);
  Hero();

  byte damage(byte atk){
    if(HP<=atk){
      POS.i = 200;
      POS.j = 200;
      HP = 0;
      return 0;
    }
    HP -= atk;
    return 1;
  }

  void levelUp() {
    HP = MAX_HP;
    MP = MAX_MP;
    if(HERO_CLASS==RANGER){
      SPD += LVL%5 == 4 ? 1 : 0;
      HP  += MAX_HP/7 + LVL;
      MP  += MAX_MP/7 + LVL;
      MAX_HP = HP;
      ATK += LVL%2 == 1 ? 1 : 0;
      LVL++;
    }
    else if(HERO_CLASS==BARBARIAN){
      SPD += LVL%6 == 4 ? 1 : 0;
      HP  += MAX_HP/7 + (LVL+2);
      MP  += MAX_MP/7 + LVL;
      ATK +=  LVL%2 == 1 ? 1 : 0;
      MAX_HP = HP;
      LVL++;
    }
    else if(HERO_CLASS==WIZARD){
      SPD += LVL%6 == 4 ? 1 : 0;
      HP  += MAX_HP/7 + LVL;
      MP  += MAX_MP/7 + LVL;
      ATK += LVL%2 == 1 ? 2 : 0;
      MAX_HP = HP;
      LVL++;
    }
    else if(HERO_CLASS==MONSTER){
      SPD += LVL/6;
      HP  += M_INIT_HP/2 * LVL;
      ATK += LVL/2;
      MAX_HP = HP;
      LVL++;
    }
    if(SPD > 14) SPD = 14;
    MAX_MP = MP;
  }

  byte checkLevel() {
    if(KILLS%(LVL*5) == (LVL*5)-1) {
      levelUp();
      return 1;
    }
    return 0;
  }

  void setPos(byte, byte);
};

int has_monster(byte, byte);
Hero* get_monster(byte, byte);

extern Hero monsters[100];
extern Projetil projeteis[200];
extern byte num_projeteis;
extern Hero hero;

#endif



