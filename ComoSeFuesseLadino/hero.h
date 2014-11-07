#ifndef HERO_H
#define HERO_H
#include "Arduino.h"
#include "map.h"

//classes
#define RANGER         0
#define BARBARIAN      1
#define WIZARD         2

//initial lifes
#define R_INIT_HP      2
#define B_INIT_HP      3
#define W_INIT_HP      1

//speed of actions
#define SPD_RANGER     8
#define SPD_BARBARIAN  6
#define SPD_WIZARD     4

#define UP            1 
#define DOWN          2 
#define LEFT          3  
#define RIGHT         4 

#define num_monster   30

#define TICKET        10
class Position
{
    public:
        int i;
        int j;
        int d;
};

class Hero
{
    public:
        Position POS;
        int HP, ATK, HERO_CLASS, LVL, SPD, TKT, KILLS;
        Hero(int);
        Hero();
        
        void setPos(int x, int y) {
          if(!isColliding(x, y)) {
            POS.i = x;
            POS.j = y;
          }
        }
        
        int damage(int atk){
          HP -= atk;
          if(HP<=0){
            POS.i = 1000;
            POS.j = 1000;
            return 0;
          }
          return 1;
        }
};

int has_monster(int, int);
Hero* get_monster(int, int);

extern Position camH;
extern Position camV;
extern Hero hero;
extern Hero monsters[100];
#endif
