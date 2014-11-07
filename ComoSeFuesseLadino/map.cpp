#include <stdio.h>
#include <Arduino.h>
#include "map.h"

char m[((sizeX-2)*(sizeY-2))/8];

int calculate_pos(byte x, byte y) {
  x -= 2;
  y -= 2;
  return (x*sizeX + y);
}

byte is_out_of_borders(byte x, byte y) {
  return x >= sizeX-1 || x <= 0 || y <= 0 || y >= sizeY-1;
}

char get_position(byte x, byte y) {
  if(is_out_of_borders(x, y)) return char(144); 
  int pos = calculate_pos(x, y);
  if((m[pos/8] >> pos%8)%2 == 0) return '.';
  else return char(144);
}

void set_position(byte x, byte y, byte value) {
  if(is_out_of_borders(x, y)) return; 
  int pos = calculate_pos(x, y);
  if(value == 0)
    m[pos/8] = m[pos/8] | (1 << pos%8);
  else
    m[pos/8] = m[pos/8] & (255 - (1 << pos%8));
}

class Room {
  public:
    int x, y;
    int h, w;
};


int does_collide(Room room, Room *rooms, int num_rooms) {
  for (int i = 0; i < num_rooms; i++) {
    Room check = rooms[i];
    if (!((room.x + room.w < check.x) || (room.x > check.x + check.w) 
          || (room.y + room.h < check.y) || (room.y > check.y + check.h))) return 1;
  }
  return 0;
}

void createRoom(byte minSize, byte maxSize, int num_rooms) {
  Room rooms[num_rooms];
  for(int n = 0; n < num_rooms; n++) {
    int x  = random(2, sizeX - maxSize - 1);
    int y  = random(2, sizeY - maxSize - 1);
    int w  = random(minSize, maxSize);
    int h  = random(minSize, maxSize);

    Room room;
    room.x = x;
    room.y = y;
    room.w = w;
    room.h = h-1;
    rooms[n] = room;

    if(does_collide(room, rooms, n) == 1) {
      n--;
      continue;
    } else {
      for(int i = 0; i <= w; i++) {
        set_position(x+i, y, 0);
        set_position(x+i, y+h, 0);
      }

      for(int j = 1; j < h; j++) { 
        set_position(x, y+j, 0);
        set_position(x+w, y+j, 0);
      }
    }

    byte portSize = 4;

    int p1X = random(x+portSize, x+w-portSize);
    int p2X = random(x+portSize, x+w-portSize);
    for(int i=-portSize; i < portSize; i++) {
      set_position(p1X+i, y, 1);
      set_position(p2X+i, y+h, 1);
    }

    int p1Y = random(y+portSize, y+h-portSize);
    int p2Y = random(y+portSize, y+h-portSize);
    for(int i=-portSize; i < portSize; i++) {
      set_position(x, p1Y+i, 1);
      set_position(x+w, p1Y+i, 1);
    }
  }
}

int canSee(int x0, int y0, int x1, int y1) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    if (x0==x1 && y0==y1) break;
    
    if(get_position(x0, y0) == char(144)) 
      return 1;

    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
  return 0;
}

int isColliding(byte x, byte y) {
  return get_position(x, y) == char(144);
}

void print_map() {
  int x = 10;
  int y = 15;
  int sizeField = 20;
  for(byte i=0; i < sizeX; i++) {
    for(byte j=0; j < sizeY; j++) {
      if(x == i && y == j)
        printf("@");
      else //if(sqrt((x - i)*(x - i) + (y - j)*(y - j)) < sizeField)
        //if(canSee(x, y, i, j) != 1)
          printf("%c", get_position(i, j));
        //else
        //  printf(" ");
//      else 
//        printf(" ");
    }
    printf("\n");
  }
}
