#ifndef PLAYER_H
#define PLAYER_H

#define unsigned int uint

enum Job {
  Ladino, Trombadinha;
};

class Attributes {
  private:
    uint HP;
    uint MP;
    uint stamina;

    uint STR;
    uint DEX;
    uint CON;
    uint WIS;
    uint INT;
    uint LCK;
  public:
    
    void doDamage(uint dmg) {
      if (HP < dmg)
        HP = 0;
      else
        HP -= dmg;
    }

    void doHeal(uint h) {
      HP += h;
      if (HP > getMaxHP()) 
        HP = getMaxHP();
    }

    void reduceMP(uint d) {
      if (MP >= d)
        MP -= d;
    }

    void increaseMP(uint h) {
      MP += h;
      if (MP > getMaxMP()) 
        MP = getMaxMP();
    }

    uint getMaxHP() {
      return CON * 100;
    }

    uint getMaxMP() {
      return INT * 100;
    }

    uint getHP() {
      return HP;
    }

    uint getMP() {
      return MP;
    }

    uint getStr() {
      return STR;
    }

    uint getDex() {
      return DEX;
    }

    uint getCon() {
      return CON;
    }

    uint getInt() {
      return INT;
    }

    uint getWis() {
      return WIS;
    }

    uint getLck() {
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
    string name;
    uint level;
    uint xp;
    LinkedList<Entity&> bag;
    uint bagSize;
    Job job;
    Attributes attr;

    void updateLevel() {
      if (xp > level*1000) {
        level++;
      }
    }
  public:
    Player(string name, Job job) {
      this->name = name;
      this->job = job;
      // TODO! DO ATTRIBUTES SETS
      bagSize = 10;
      level = 1;
    }

    void gainXP(uint xp) {
      this->xp += xp;
      updateLevel();
    }

    int getNumItensInBag() {
      return bag.size();
    }

    Entity& getItemInBag(int i) {
      return bag.get(i);
    }
};

#endif
