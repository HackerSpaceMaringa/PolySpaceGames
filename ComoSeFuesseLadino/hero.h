#ifndef HERO_H
#define HERO_H
#include "Arduino.h"
#include "map.h"

//classes
#define RANGER         0
#define BARBARIAN      1
#define WIZARD         2
#define MONSTER         2

//initial lifes
#define R_INIT_HP      6
#define B_INIT_HP      8
#define W_INIT_HP      6
#define M_INIT_HP      3

//speed of actions
#define SPD_RANGER     12
#define SPD_BARBARIAN  10
#define SPD_WIZARD     9
#define SPD_MONSTER    9

#define UP            0 
#define LEFT          1  
#define DOWN          2 
#define RIGHT         3 

#define num_monster   30

#define TICKET        15
class Position
{
public:
  int i;
  int j;
  int d;
};

extern Position camH;
extern Position camV;

class Projetil {
  public:
    int SPD;
    int ATK;
    Position POS;
    int TKT;
    int A;
    
    Projetil() {
      A = 0;
    }
};

class Hero {
public:
  Position POS;
  int HP, ATK, HERO_CLASS, LVL, SPD, TKT, KILLS;
  Hero(int);
  Hero();

  int damage(int atk){
    HP -= atk;
    if(HP<=0){
      POS.i = 1000;
      POS.j = 1000;
      return 0;
    }
    return 1;
  }

  void levelUp() {
    if(HERO_CLASS==RANGER){
      SPD += LVL/4;
      HP  += R_INIT_HP * LVL/4;
      ATK += LVL/3;
      LVL++;
    }
    else if(HERO_CLASS==BARBARIAN){
      SPD += LVL/6;
      HP  += R_INIT_HP * (LVL+1)/4;
      ATK += LVL/3;
      LVL++;
    }
    else if(HERO_CLASS==WIZARD){
      SPD += LVL/6;
      HP  += R_INIT_HP * LVL/4;
      ATK += LVL/3;
      LVL++;
    }
    else if(HERO_CLASS==MONSTER){
      SPD += LVL/6;
      HP  += R_INIT_HP * LVL/4;
      ATK += LVL/2;
      LVL++;
    }
  }

  void checkLevel() {
    if(KILLS%(LVL*4) == 0) {
      levelUp();
    }
  }

  void setPos(int, int);
};

int has_monster(int, int);
Hero* get_monster(int, int);

extern Hero monsters[100];
extern Projetil projeteis[200];
extern int num_projeteis;
extern Hero hero;

#endif



