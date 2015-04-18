#ifndef POSITION_H
#define POSITION_H

class Position {
  private:
    int x, y;
  public:
    void setX(int x) {
      this->x = x;
    }

    void setY(int y) {
      this->y = y;
    }

    int getX() {
      return x;
    }

    int getY() {
      return y;
    }

    bool equal(Position pos) {
      return pos.getX() == x && pos.getY() == y;
    }

    void setPosition(Position pos) {
      x = pos.getX();
      y = pos.getY();
    }
};

#endif
