#ifndef ACTOR_H
#define ACTOR_H

#include "Position.h"

enum SizeType {
  unit, small, medium, large
};

class Actor {
  private:
    Position pos;
    SizeType size;
    bool visible, colidable;
  
  public:
    Actor(Position P, SizeType S) {
      pos.setPosition(P);
      size = S; 

      visible = true;
      colidable = true;
    }

    Actor(Position P, SizeType S, bool V, bool C) {
      pos.setPosition(P);
      size = S;

      visible = V;
      colidable = C;
    }
    
    Position& getPosition() {
      return pos;  
    }

    SizeType getSize() {
      return size;
    }

    bool isVisible() {
      return visible;
    }

    bool isColidable() {
      return colidable;
    }

    void setVisible(bool V) {
      visible = V;
    }

    void setColidable(bool C) {
      colidable = C;
    }

    bool hasColided(Actor &A) {
      return isColidable() && A.getPosition().equal(getPosition()) 
        && A.isColidable(); 
    }
};

#endif
