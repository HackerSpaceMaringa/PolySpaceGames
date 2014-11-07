#include "hero.h"
//Construtor Hero
Hero::Hero(int h_class) {
    POS.i = 20;
    POS.j = 20;
    POS.d = 0;
    
    ATK = 0;
    HERO_CLASS = h_class;
    if(HERO_CLASS==RANGER){
        SPD = SPD_RANGER;
        HP  = R_INIT_HP;
    }else if(HERO_CLASS==BARBARIAN){
        SPD = SPD_BARBARIAN;
        HP  = B_INIT_HP;
    }else if(HERO_CLASS==WIZARD){
        SPD = SPD_WIZARD;
        HP  = W_INIT_HP;
        ATK++;
    }

    TKT = 0;
    ATK++;
    LVL = 1;
    
}

Hero::Hero() {
    POS.i = 20;
    POS.j = 20;
    POS.d = 0;
    HERO_CLASS = BARBARIAN;
    
    ATK = 0;
    
    if(HERO_CLASS==RANGER){
        SPD = SPD_RANGER;
        HP  = R_INIT_HP;
    }else if(HERO_CLASS==BARBARIAN){
        SPD = SPD_BARBARIAN;
        HP  = B_INIT_HP;
    }else if(HERO_CLASS==WIZARD){
        SPD = SPD_WIZARD;
        HP  = W_INIT_HP;
        ATK++;
    }

    TKT = 0;
    ATK++;
    LVL = 1;
    
}

int Hero::damage(int atk){
  HP -= atk;
  if(HP <= 0) return 0;
  else return 1;
}
