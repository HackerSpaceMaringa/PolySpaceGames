#ifndef MAP_H
#define MAP_H

#include "Position.h"

class Map {
  private:
    LinkedList actor<Actor&>;
    int height, width;
    char *m, *mLum;

    int getMapPosition(Position p) {
      int position;

      position = p.getX() * height + p.getY();
      return position;
    }

    void getPositionFromMap(int p, Position p) {
      p.setX(p / height);
      p.setY(p % height);
    }

  public:
    Map(int height, int width) {
      int i;
      
      this->height = height;
      this->width = width;

      m = new char[height * width];
      mLum = new char[height * width];
    }

    char getElement(Position p) {
      return m[getMapPosition(p)];
    }

    char getLuminosity(Position p) {
      return mLum[getMapPosition(p)];
    }
}

#endif
