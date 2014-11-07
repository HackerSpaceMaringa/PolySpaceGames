#include "hero.h"

Position camH;
Position camV;
//Construtor Hero
Hero::Hero(int h_class) {
  POS.i = 20;
  POS.j = 20;
  POS.d = 0;
  KILLS = 0;
  ATK = 0;
  HERO_CLASS = h_class;
  if(HERO_CLASS==RANGER){
    SPD = SPD_RANGER;
    HP  = R_INIT_HP;
  }
  else if(HERO_CLASS==BARBARIAN){
    SPD = SPD_BARBARIAN;
    HP  = B_INIT_HP;
  }
  else if(HERO_CLASS==WIZARD){
    SPD = SPD_WIZARD;
    HP  = W_INIT_HP;
    ATK++;
  }

  TKT = TICKET - SPD;
  ATK++;
  LVL = 1;

}

Hero::Hero() {
  POS.i = 20;
  POS.j = 20;
  POS.d = 0;
  HERO_CLASS = BARBARIAN;

  ATK = 0;
  KILLS = 0;
  SPD = SPD_MONSTER;
  HP  = M_INIT_HP;
  ATK++;

  TKT = TICKET - SPD;
  ATK++;
  LVL = 1;

}

int has_monster(int x, int y){
  for(int i = 0; i< num_monster; i++){
    if(monsters[i].POS.i == x && monsters[i].POS.j == y){
      return 1;
    }
  }
  return 0;
}

Hero* get_monster(int x, int y){
  for(int i = 0; i< num_monster; i++){
    if(monsters[i].POS.i == x && monsters[i].POS.j == y){
      return &monsters[i];
    }
  }
  return NULL;
}

Hero hero(1);
Hero monsters[100];
Projetil projeteis[200];
int num_projeteis;



void Hero::setPos(int x, int y) {
  if(!(isColliding(x, y) || has_monster(x, y) == 1)) {
    if(!(hero.POS.i == x && hero.POS.j == y)) {
      POS.i = x;
      POS.j = y;
    }
  }
}


