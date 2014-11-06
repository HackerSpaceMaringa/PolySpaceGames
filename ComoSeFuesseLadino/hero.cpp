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

/*void moviment(Hero hero, byte direction)
{
    if(direction==UP)
    {
    }
    else if(direction==DOWN)
    {
    }
    else if(direction==LEFT)
    {
    }
    else if(direction==RIGHT)
    {
    }
    
}*/
