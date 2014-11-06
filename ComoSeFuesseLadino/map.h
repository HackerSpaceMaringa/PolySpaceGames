#ifndef MAP_H
#define MAP_H

#define sizeX 55
#define sizeY 105
#include "Arduino.h"
//#define byte unsigned char

byte is_out_of_borders(byte, byte);

char get_position(byte, byte);

void createRoom(byte, byte, int);

int canSee(int, int, int, int);

int isColliding(byte, byte);

#endif
