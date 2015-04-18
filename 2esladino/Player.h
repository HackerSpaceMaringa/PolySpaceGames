#ifndef PLAYER_H
#define PLAYER_H
#include "Actor.h"

enum Job {
  Ladino
};

class Attributes {
  private:
    unsigned HP;
    unsigned MP;
    unsigned stamina;

    unsigned STR;
    unsigned DEX;
    unsigned CON;
    unsigned WIS;
    unsigned INT;
    unsigned LCK;
  public:
    
    void doDamage(unsigned dmg) {
      if (HP < dmg)
        HP = 0;
      else
        HP -= dmg;
    }

    void doHeal(unsigned h) {
      HP += h;
      if (HP > getMaxHP()) 
        HP = getMaxHP();
    }

    void reduceMP(unsigned d) {
      if (MP >= d)
        MP -= d;
    }

    void increaseMP(unsigned h) {
      MP += h;
      if (MP > getMaxMP()) 
        MP = getMaxMP();
    }

    unsigned getMaxHP() {
      return CON * 100;
    }

    unsigned getMaxMP() {
      return INT * 100;
    }

    unsigned getHP() {
      return HP;
    }

    unsigned getMP() {
      return MP;
    }

    unsigned getStr() {
      return STR;
    }

    unsigned getDex() {
      return DEX;
    }

    unsigned getCon() {
      return CON;
    }

    unsigned getInt() {
      return INT;
    }

    unsigned getWis() {
      return WIS;
    }

    unsigned getLck() {
      return LCK;
    }

    void setStr(int s) {
      STR = s;
    }

    void setDex(int d) {
      DEX = d;
    }

    void setInt(int i) {
      INT = i;
    }

    void setCon(int c) {
      CON = c;
    }

    void setWis(int w) {
      WIS = w;
    }

    void setLck(int l) {
      LCK = l;
    }
};

class Player : public Actor {
  private:
    char* name;
    unsigned level;
    unsigned xp;
    LinkedList<Entity*> bag;
    unsigned bagSize;
    Job job;
    Attributes attr;

    void updateLevel() {
      if (xp > level*1000) {
        level++;
      }
    }

  public:
    Player(char* name, Job job, Position P) 
	: Actor(P, unit, true, true) {
      this->name = name;
      this->job = job;
      // TODO! DO ATTRIBUTES SETS
      bagSize = 10;
      level = 1;
    }

    void gainXP(unsigned xp) {
      this->xp += xp;
      updateLevel();
    }

    int getNumItensInBag() {
      return bag.size();
    }

    Entity& getItemInBag(int i) {
      return *bag.get(i);
    }

    void insertItemInBag(Entity *item) {
      bag.add(item);
    }
};

#endif
