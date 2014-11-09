#include <SNESpaduino.h>
#include <TVout.h>
#include <fontALL.h>
#include <MemoryFree.h>
#include "avr/pgmspace.h"
#include "fontRL.h"
#include "map.h"
#include "hero.h"

SNESpaduino player1(20, 21, 19);

Position portal;
Position bau;
int floor_state = 0;

uint16_t btns;

PROGMEM const char history[] = "Aqui vosmece encontrara o mundo de RUP, que foi d7minado por um Diagrametico TROLL.\n Jose, voce foi REQUISITADO a libertar esse mundo e por fim a todo esse desperdicio de papel...PRESS [R]";

TVout tv;

void print_screen(int x, int y, char c){
  tv.set_cursor(y*6,x*8);
  tv.print(c);
}

int print_intro() {
  tv.set_cursor(20, 30);
  tv.select_font(font8x8);
  tv.draw_circle(tv.hres()/2,tv.vres()/2,tv.vres()/3,WHITE);
  tv.println(" Powered by");
  tv.set_cursor(6, 38);
  tv.println("Fosforos Parana");
  tv.select_font(font6x8);
  tv.delay(4000);
  tv.clear_screen();
  tv.clear_screen(); 
  //tv.set_cursor(0,20);
  tv.draw_rect(10,20,100,45,WHITE);
  tv.set_cursor(20, 30);
  tv.println("Como se fuesse");
  tv.set_cursor(43, 45);
  tv.println("LADINO");
  tv.set_cursor(50,62);
  tv.println("v0.1.0");
  tv.delay(5000);
  tv.clear_screen();
}

void loading() {
  tv.set_cursor(40,60);
  tv.select_font(font8x8);
  tv.println("LOADING...");
  tv.select_font(font6x8);
}

void init_cam() {
  camH.i = 0;
  camH.j = 21;

  camV.i = 0;
  camV.j = 11;
}

int moveCam() {  
  if(hero.POS.j >= camH.j - 3) {
    camH.j += 1;
    camH.i += 1;
  }

  if(hero.POS.i >= camV.j - 3) {
    camV.j += 1;
    camV.i += 1;
  }

  if(hero.POS.j <= camH.i + 3 && hero.POS.j >= 4) {
    camH.j -= 1;
    camH.i -= 1;
  }

  if(hero.POS.i <= camV.i + 3 && hero.POS.i >= 4) {
    camV.j -= 1;
    camV.i -= 1;
  }
}

void wait_r() {
  tv.set_cursor(73,81);

  tv.print("PRESS [R]");
  btns = player1.getButtons();
  while(!(btns & BTN_R)) btns = player1.getButtons();
}

void print_sheet(){
  tv.clear_screen();
  tv.draw_rect(0,0,75,35,WHITE);
  tv.draw_line(0,12,75,12,WHITE);
  tv.draw_rect(0,35,75,35,WHITE);
  tv.set_cursor(2,4);
  if(hero.HERO_CLASS==RANGER) tv.println("RANGER");
  else if(hero.HERO_CLASS==BARBARIAN) tv.println("BARBARIAN");
  else if(hero.HERO_CLASS==WIZARD) tv.println("WIZARD");
  tv.set_cursor(2,17);
  tv.print("NAME  : ");
  tv.println("JOSE");
  tv.set_cursor(2,25);
  tv.print("LVL   : ");
  tv.println((int) hero.LVL);
  tv.set_cursor(2,38);
  tv.print("HP    : ");
  tv.println((int) hero.HP);
  tv.set_cursor(2,46);
  tv.print("SPD   : ");
  tv.println((int) hero.SPD);
  tv.set_cursor(2,54);
  tv.print("ATK   : ");
  tv.println((int) hero.ATK);
  tv.set_cursor(2,62);
  tv.print("KILLS : ");
  tv.println((int) hero.KILLS);
  tv.set_cursor(73,81);

  wait_r();
}

void print_field(){
  tv.select_font(fontRL);
  for(byte i = camV.i; i < camV.j; i++) {
    for(byte j = camH.i; j < camH.j; j++) {
      if(hero.POS.i == i && hero.POS.j == j)
        print_screen(i-camV.i, j-camH.i, (char(128 + hero.POS.d)));
      else
        if(canSee(hero.POS.i, hero.POS.j, i, j) != 1){
          if(!has_monster(i,j)){
            if(portal.i == i && portal.j == j)
              print_screen(i-camV.i, j-camH.i, '@');
            else if(bau.i == i && bau.j == j) 
              print_screen(i-camV.i, j-camH.i, char(132));
            else {
              print_screen(i-camV.i, j-camH.i, (get_position(i, j)));
            }
          }
          else
            if(floor_state == 1)
              print_screen(i-camV.i, j-camH.i, '%');
            else if (floor_state == 2) 
              print_screen(i-camV.i, j-camH.i, '$');
            else if (floor_state == 3)
              print_screen(i-camV.i, j-camH.i, char(134));
        }
        else
          print_screen(i-camV.i, j-camH.i, ' ');
    }
  }
}

void print_botton() {
  tv.select_font(fontRL);
  tv.set_cursor(0*6,11*8);
  tv.print("           ");

  if(hero.HP > 0) {
    tv.set_cursor(0*6,11*8);
    tv.print(" h:");
    tv.print((int) hero.HP);
    tv.print(" m:");
    tv.print((int) hero.MP);
    tv.print(" ");
  }
}

void print_msg(char *s) {
  if(hero.HP > 0) {
    tv.select_font(fontRL);
    tv.set_cursor(11*6,11*8);
    tv.print("         ");
    tv.set_cursor(11*6,11*8);
    tv.print(s);
  }
}

int select_class(){
  int i = 0;
  tv.println("Select your Class: ");
  tv.println();
  //tv.clear_screen();

  tv.println("[X] - Ranger");
  tv.println("[B] - Barbarian");
  tv.println("[Y] - Wizard");
  btns = player1.getButtons();
  while(!((btns & BTN_X) | (btns & BTN_B) | (btns & BTN_Y))){
    btns = player1.getButtons();  
  }
  tv.clear_screen();
  tv.println("Seja Bem-Vindo,");
  if(btns & BTN_X){
    Hero n_hero(RANGER);
    hero = n_hero;
    tv.println("RANGER");
  }
  else if(btns & BTN_B){
    Hero n_hero(BARBARIAN);
    hero = n_hero;
    tv.println("BARBARIAN");   
  }
  else if(btns & BTN_Y){
    Hero n_hero(WIZARD);
    hero = n_hero;
    tv.println("WIZARD");
  }
  tv.println("");
  tv.printPGM(history);

  btns = player1.getButtons();
  while(!(btns & BTN_R)) btns = player1.getButtons();
  //tv.delay(8000);
  tv.clear_screen();

  return 0;
}

void atackPos(byte i, byte j, byte dano, char tile) {
  Hero* monster = get_monster(i, j);
  print_screen(i-camV.i, j - camH.i, tile);
  if (monster) {
    if(!monster->damage(dano)) {
      hero.KILLS++;
      byte level_up = hero.checkLevel();
      if(level_up) print_msg("LEVEL UP!");
      else   print_msg("          ");
    }
  }

  if(hero.POS.i == i && hero.POS.j ==  j)
    hero.damage(dano);
}

void cleave_atk(Hero h){
  tv.select_font(fontRL);
  if (h.POS.d == LEFT) {
    atackPos(h.POS.i+1, h.POS.j - 1, h.ATK, char(136));
    atackPos(h.POS.i, h.POS.j - 1, h.ATK,   char(136));
    atackPos(h.POS.i-1, h.POS.j - 1, h.ATK, char(136));
  } 
  else if (h.POS.d == RIGHT) {
    atackPos(h.POS.i+1, h.POS.j + 1, h.ATK, char(138));
    atackPos(h.POS.i, h.POS.j + 1, h.ATK,   char(138));
    atackPos(h.POS.i-1, h.POS.j + 1, h.ATK, char(138));
  } 
  else if (h.POS.d == UP) {
    atackPos(h.POS.i - 1, h.POS.j+1, h.ATK, char(135));
    atackPos(h.POS.i - 1, h.POS.j, h.ATK,   char(135));
    atackPos(h.POS.i - 1, h.POS.j-1, h.ATK, char(135));
  } 
  else if (h.POS.d == DOWN) {
    atackPos(h.POS.i + 1, h.POS.j+1, h.ATK, char(137));
    atackPos(h.POS.i + 1, h.POS.j, h.ATK,   char(137));
    atackPos(h.POS.i + 1, h.POS.j-1, h.ATK, char(137));
  }
}

void atack(Hero h) {
  h.AGRS = 1;
  tv.select_font(fontRL);
  if (h.POS.d == LEFT) {
    atackPos(h.POS.i, h.POS.j - 1, h.ATK, char(136));
  } 
  else if (h.POS.d == RIGHT) {
    atackPos(h.POS.i, h.POS.j + 1, h.ATK, char(138));
  } 
  else if (h.POS.d == UP) {
    atackPos(h.POS.i - 1, h.POS.j, h.ATK, char(135));
  } 
  else if (h.POS.d == DOWN) {
    atackPos(h.POS.i + 1, h.POS.j, h.ATK, char(137));
  }
}

void atack_projetil(Hero h) {
  for(int i = 0; i < 200; i++) {
    if(projeteis[i].A == 0) {
      if(h.HERO_CLASS == RANGER) {
        projeteis[i].SPD = 14;
      } 
      else {
        projeteis[i].SPD = 13;
      }

      projeteis[i].ATK = hero.ATK * 2;
      projeteis[i].TKT = TICKET - projeteis[i].SPD;

      tv.select_font(fontRL);
      if (h.POS.d == LEFT) {
        projeteis[i].POS   = h.POS;
        projeteis[i].POS.j -= 1;
        atackPos(h.POS.i, h.POS.j - 1, h.ATK, char(142));
      } 
      else if (h.POS.d == RIGHT) {
        projeteis[i].POS   = h.POS;
        projeteis[i].POS.j += 1;
        atackPos(h.POS.i, h.POS.j + 1, h.ATK, char(140));
      } 
      else if (h.POS.d == UP) {
        projeteis[i].POS   = h.POS;
        projeteis[i].POS.i -= 1;
        atackPos(h.POS.i - 1, h.POS.j, h.ATK, char(139));
      } 
      else if (h.POS.d == DOWN) {
        projeteis[i].POS   = h.POS;
        projeteis[i].POS.i += 1;       
        atackPos(h.POS.i + 1, h.POS.j, h.ATK, char(141));
      }
      projeteis[i].A = 1;
      return;
    }  
  }
}

void stun(Hero& m) {
  m.TKT = 50;
  m.damage(1);
  print_msg("STUNNED!");
}

int meteor_count = 0;
void check_meteors() {
  if(meteor_count > 3) {
    meteor_count -= 3;

    for(int i=0; i < 3; i++) {
      int k = random(-5, 5);
      int l = random(-5, 5);

      if(is_in_cam(hero.POS.i + k, hero.POS.j + l))
        if(k != 0 && l != 0)
          atackPos(hero.POS.i + k, hero.POS.j + l, hero.ATK*4, '*');
    }
  }
}

void fire(byte x, byte y, int d) {
  if(!is_in_cam(x, y)) return;
  if(d == 0) return;
  if(isColliding(x, y)) return;

  if(!(x == hero.POS.i && y == hero.POS.j))
    atackPos(x, y, hero.ATK, char(144+random(0, 4)));

  if(random(1, 8)%7 == 0) 
    fire(x, y - 1, d-1);
  if(random(1, 8)%7 == 0)  
    fire(x, y + 1, d-1);
  if(random(1, 8)%7 == 0)  
    fire(x + 1, y, d-1);
  if(random(1, 8)%7 == 0) 
    fire(x - 1, y, d-1);
  if(random(1, 8)%7 == 0) 
    fire(x - 1, y - 1, d-1);
  if(random(1, 8)%7 == 0) 
    fire(x - 1, y + 1, d-1);
  if(random(1, 8)%7 == 0) 
    fire(x + 1, y - 1, d-1);
  if(random(1, 8)%7 == 0) 
    fire(x + 1, y + 1, d-1);
}

int speed_count = 0;
int old_speed = 0;
int speeded = 0;
void check_speed() {
  if(speed_count == 0) {
    if(speeded == 1)
      hero.SPD = old_speed;
    speeded = 0;
  } else {
    if(speeded == 0) {
      if(hero.SPD < 14) {
        old_speed = hero.SPD;
        hero.SPD += 1;
      }
      speeded = 1;
    }
    speed_count--; 
  }
}


void control() {
  btns = player1.getButtons();

  if(btns & BTN_A){
    if(hero.MP > 3) {
      hero.MP -= 3;
      if(hero.HERO_CLASS == BARBARIAN) {
        if (hero.POS.d == LEFT) {
          stun(*get_monster(hero.POS.i, hero.POS.j - 1));
        } 
        else if (hero.POS.d == RIGHT) {
          stun(*get_monster(hero.POS.i, hero.POS.j + 1));
        } 
        else if (hero.POS.d == UP) {
          stun(*get_monster(hero.POS.i - 1, hero.POS.j));
        } 
        else if (hero.POS.d == DOWN) {
          stun(*get_monster(hero.POS.i + 1, hero.POS.j));
        }
        atack(hero);
      } 
      else if(hero.HERO_CLASS == RANGER) {
        hero.POS.d = LEFT;
        atack_projetil(hero);
        hero.POS.d = RIGHT;
        atack_projetil(hero);
        hero.POS.d = UP;
        atack_projetil(hero);
        hero.POS.d = DOWN;
        atack_projetil(hero);
      } 
      else if(hero.HERO_CLASS == WIZARD) {
        meteor_count += 18 * (hero.LVL+1)/2;
        print_msg("METEORS");
      }

    }
  }
  if (btns & BTN_B){
    if(hero.MP > 4*(hero.LVL+1)/2) {
      hero.MP -= 4*(hero.LVL+1)/2;
      if(hero.HERO_CLASS == BARBARIAN) {
        Hero *m1 = get_monster(hero.POS.i, hero.POS.j - 1);
        Hero *m2 = get_monster(hero.POS.i, hero.POS.j + 1);
        Hero *m3 = get_monster(hero.POS.i-1, hero.POS.j);
        Hero *m4 = get_monster(hero.POS.i+1, hero.POS.j);

        stun(*m1);
        stun(*m2);
        stun(*m3);
        stun(*m4);

        for(int i=0; i < hero.LVL+1; i++) {
          hero.POS.d = RIGHT;
          atack(hero);
          hero.POS.d = LEFT;
          atack(hero);
          hero.POS.d = UP;
          atack(hero);
          hero.POS.d = DOWN;
          atack(hero);
        }

        m1->setPos(hero.POS.i, hero.POS.j - 2);
        m2->setPos(hero.POS.i, hero.POS.j + 2);
        m3->setPos(hero.POS.i-2, hero.POS.j);
        m4->setPos(hero.POS.i+2, hero.POS.j);
      } 
      else if(hero.HERO_CLASS == WIZARD) {
        for(int i = 0; i < num_monster; i++) {
          monsters[i].TKT += 7 + ((hero.LVL+1)/2);
        }
        for(int i = 0; i < 200; i++) {
          if(projeteis[i].A == 1) {
            projeteis[i].A = 0;
          }
        }
        print_msg("CHRONOS");
      }
      else if(hero.HERO_CLASS == RANGER) {
        speed_count += 20 + hero.LVL;
        print_msg("BOOST");
      }
    }
  }
  if(btns & BTN_DOWN){
    if(hero.POS.d == DOWN)
      hero.setPos(hero.POS.i + 1, hero.POS.j);
    hero.POS.d = DOWN;
  }
  if(btns & BTN_UP){
    if(hero.POS.d == UP)
      hero.setPos(hero.POS.i - 1, hero.POS.j);
    hero.POS.d = UP;
  }
  if(btns & BTN_LEFT){
    if(hero.POS.d == LEFT)
      hero.setPos(hero.POS.i, hero.POS.j - 1);
    hero.POS.d = LEFT;
  }
  if(btns & BTN_RIGHT){
    if(hero.POS.d == RIGHT)
      hero.setPos(hero.POS.i, hero.POS.j + 1);
    hero.POS.d = RIGHT;
  }
  if(btns & BTN_Y){
    if(hero.HERO_CLASS == WIZARD){
      atack_projetil(hero);
      hero.TKT += 5;
    } 
    else {
      atack(hero);
    }
  }
  if(btns & BTN_X){
    if(hero.MP > 0) {
      if(hero.HERO_CLASS == BARBARIAN) {
        cleave_atk(hero);
      } 
      else if(hero.HERO_CLASS == RANGER){
        atack_projetil(hero);
        
      } 
      else if(hero.HERO_CLASS == WIZARD) {
        fire(hero.POS.i, hero.POS.j, hero.LVL+1);
        print_msg("LIGHTING");
      }
      hero.MP--;
    }
  }
  if(btns & BTN_L){
    print_sheet();
  }
  if(btns & BTN_R){
    if(hero.MP > 0 && hero.HP > 0) {
      if(hero.HERO_CLASS == BARBARIAN) {
        hero.HP += 2;
        hero.MP--;
      } 
      else if(hero.HERO_CLASS == WIZARD) {
        if(hero.HP > 2) {
          hero.HP--;
          hero.MP += 2;
        }
      } 
      else if(hero.HERO_CLASS == RANGER) {
        if(hero.MP > 1) {
          hero.MP -= 2;
          atack_projetil(hero);

          if(hero.POS.d == LEFT)
            hero.POS.d = RIGHT;
          else if(hero.POS.d == RIGHT)
            hero.POS.d = LEFT;
          else if(hero.POS.d == UP)
            hero.POS.d = DOWN;
          else if(hero.POS.d == DOWN)
            hero.POS.d = UP;

          atack_projetil(hero);  
        }
      }
    }
  }
  if(btns & BTN_START){

  }
  if(btns & BTN_SELECT){

  }
}

int is_in_cam(int y, int x) {
  return x > camH.i && x < camH.j && y > camV.i && y < camV.j;
}

void init_monsters() {
  for(int i = 0; i < num_monster; i++) {
    monsters[i].POS.i = random(4, sizeY);
    monsters[i].POS.j = random(4, sizeX);

    monsters[i].KILLS = 0;
    monsters[i].SPD = SPD_MONSTER + floor_state;
    monsters[i].HP  = M_INIT_HP*(floor_state+1) * (floor_state > 2 ? 3 : 1);
    monsters[i].ATK = ((floor_state+1) + 1) + (floor_state > 2 ? (2*floor_state) : 0);

    monsters[i].TKT    = TICKET - monsters[i].SPD + (floor_state > 2 ? 1 : 0);
    monsters[i].LVL    = floor_state;
    monsters[i].MAX_HP = monsters[i].HP;
    monsters[i].AGRS   = 0;

    if(get_position(monsters[i].POS.i, monsters[i].POS.j) == char(144)) {
      i--;
      continue;
    }
  }
  
  for(int i=0; i < 200; i++) {
    projeteis[i].A = 0;
  }
}

byte ranger_distance = 0;
float distance(Position a, Position b) {
  if(!ranger_distance)
    return sqrt(pow(a.i - b.i,2) + pow(a.j - b.j, 2));
  else
    return abs(a.i - b.i) + abs(a.j - b.j);
}

Position runForest(Hero h, Hero hero, int dir, int hide) {
  Position p;
  p.i = h.POS.i;
  p.j = h.POS.j;
  float min = 999999999;
  Position aux;

  if(get_position(h.POS.i+1, h.POS.j) != char(144) && !has_monster(h.POS.i+1, h.POS.j)) {
    aux.i = h.POS.i+1;
    aux.j = h.POS.j;
    float peso = (dir*distance(aux, hero.POS)) - hide* (!canSee(aux.i, aux.j, hero.POS.i, hero.POS.j));

    if(peso < min) {
      p.i = aux.i;
      p.j = aux.j;
      min = peso;
    }
  }

  if(get_position(h.POS.i-1, h.POS.j) != char(144) && !has_monster(h.POS.i+1, h.POS.j)) {
    aux.i = h.POS.i-1;
    aux.j = h.POS.j;
    float peso = (dir*distance(aux, hero.POS)) - hide* (!canSee(aux.i, aux.j, hero.POS.i, hero.POS.j));

    if(peso < min) {
      p.i = aux.i;
      p.j = aux.j;
      min = peso;
    }
  }

  if(get_position(h.POS.i, h.POS.j + 1) != char(144) && !has_monster(h.POS.i+1, h.POS.j)) {
    aux.i = h.POS.i;
    aux.j = h.POS.j + 1;
    float peso = (dir*distance(aux, hero.POS)) - hide* (!canSee(aux.i, aux.j, hero.POS.i, hero.POS.j));

    if(peso < min) {
      p.i = aux.i;
      p.j = aux.j;
      min = peso;
    }
  }

  if(get_position(h.POS.i, h.POS.j - 1) != char(144) && !has_monster(h.POS.i+1, h.POS.j)) {
    aux.i = h.POS.i;
    aux.j = h.POS.j - 1;
    float peso = (dir*distance(aux, hero.POS)) - hide* (!canSee(aux.i, aux.j, hero.POS.i, hero.POS.j));

    if(peso < min) {
      p.i = aux.i;
      p.j = aux.j;
      min = peso;
    }
  }

  return p;
}

byte is_ranger = 0;

int respaw_count = 10;
void bilheteria_monstros() {
  if(floor_state == 3 && num_monster < 100 ) {
    if(respaw_count != 0) {
      respaw_count--;
    } 
    else {
      int i = num_monster;
      num_monster++;

      respaw_count = 10;
      monsters[i].POS.i = bau.i;
      monsters[i].POS.j = bau.j;

      monsters[i].KILLS = 0;
      monsters[i].SPD = SPD_MONSTER + floor_state;
      monsters[i].HP  = M_INIT_HP*floor_state;
      monsters[i].ATK = floor_state + 1;

      monsters[i].TKT    = TICKET - monsters[i].SPD;
      monsters[i].LVL    = floor_state;
      monsters[i].MAX_HP = monsters[i].HP;
      monsters[i].AGRS   = 0;  
    }
  }

  for (int i= 0;i<num_monster;i++){
    if(monsters[i].TKT <= 0){
      if(i%3 == 0) {
        is_ranger = 1;
      } 
      else {
        is_ranger = 0;
      }

      if(floor_state == 1) {
        if(is_ranger == 1) {
          IA1_ranger_monsters(monsters[i]);
        } 
        else {
          IA1Monsters(monsters[i]);
        }
      } 
      else if(floor_state == 2) {
        IA2_monsters(monsters[i]);
      } 
      else if(floor_state == 3){
        if(i%2 == 0) {
          IA3_monsters(monsters[i]);
        } 
        else {
          if(is_ranger == 1) {
            IA1_ranger_monsters(monsters[i]);
          } 
          else {
            IA1Monsters(monsters[i]);
          }
        }
      }

      monsters[i].TKT = TICKET - monsters[i].SPD;
    }
    else{
      monsters[i].TKT--;
    }  
  }

  if(monsters[num_monster-1].HP == 0) num_monster--;
}

void moveRandomicly(Hero& m) {
  m.AGRS = 0;
  int act = random(0, 5);
  if (act == UP) { 
    m.POS.d = UP;
    m.setPos(m.POS.i - 1, m.POS.j);
  }
  if (act == DOWN) { 
    m.POS.d = DOWN;
    m.setPos(m.POS.i + 1, m.POS.j);
  }
  if (act == LEFT) { 
    m.POS.d = LEFT;
    m.setPos(m.POS.i, m.POS.j - 1);
  }
  if (act == RIGHT) { 
    m.POS.d = RIGHT;
    m.setPos(m.POS.i, m.POS.j + 1);
  }
}

void IA1Monsters(Hero& m) {
  m.AGRS = 1;
  if(distance(m.POS, hero.POS) == 1) {
    if((m.POS.j, m.POS.i) && !canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j)) {
      if(m.POS.i == hero.POS.i) {
        if (m.POS.j < hero.POS.j)
          m.POS.d = RIGHT;
        else
          m.POS.d = LEFT;
      } 
      else if(m.POS.j == hero.POS.j) {
        if (m.POS.i < hero.POS.i)
          m.POS.d = DOWN;
        else
          m.POS.d = UP;
      }
      atack(m);
    }
  } 
  else if(!canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j)) {
    Position np = runForest(m, hero, 1, 4);
    m.setPos(np.i, np.j);
  } 
  else {
    moveRandomicly(m);
  }

}

void IA1_ranger_monsters(Hero& m) {
  m.AGRS = 1;
  ranger_distance = 1;
  if(distance(m.POS, hero.POS) <= 3 && random(0, 10) > 6) {
    Position np = runForest(m, hero, -1, 4);
    m.setPos(np.i, np.j);
  } 
  else if(is_in_cam(m.POS.i, m.POS.j) && !canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j) 
    && (m.POS.i == hero.POS.i || m.POS.j == hero.POS.j)) {

    if(m.POS.i == hero.POS.i) {
      if (m.POS.j < hero.POS.j)
        m.POS.d = RIGHT;
      else
        m.POS.d = LEFT;
    } 
    else if(m.POS.j == hero.POS.j) {
      if (m.POS.i < hero.POS.i)
        m.POS.d = DOWN;
      else
        m.POS.d = UP;
    }

    byte without_monster = 1;
    for(int i = 0; i < num_monster; i++) {
      if(!canSee(m.POS.i, m.POS.j, monsters[i].POS.i, monsters[i].POS.j) 
        && is_in_cam(monsters[i].POS.i, monsters[i].POS.j)
        && monsters[i].HP > 0) {
        if(m.POS.i == monsters[i].POS.i) {
          if (m.POS.j < monsters[i].POS.j)
            if(m.POS.d == RIGHT) without_monster = 0;
            else
              if(m.POS.d == LEFT) without_monster = 0;
        } 
        else if(m.POS.j == monsters[i].POS.j) {
          if (m.POS.i < monsters[i].POS.i)
            if(m.POS.d == DOWN) without_monster = 0;
            else
              if(m.POS.d == UP) without_monster = 0;
        }

        if (without_monster == 0)
          break;

      }  
    }

    if(without_monster) atack_projetil(m); 

  } 
  else if(!canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j)) {
    Position np = runForest(m, hero, 1, 4);
    m.setPos(np.i, np.j);
  } 
  else {
    moveRandomicly(m);
  }

  ranger_distance = 0;
}

void IA2_monsters(Hero& m) {
  m.AGRS = 1;
  if(float(m.HP)/m.MAX_HP > 0.35) {
    if(is_ranger) {
      IA1_ranger_monsters(m);
    } 
    else {
      IA1Monsters(m);
    }
  } 
  else {
    if(is_in_cam(m.POS.i, m.POS.j) && !canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j)) {
      Position np = runForest(m, hero, -1, -4);
      m.setPos(np.i, np.j);
    } 
    else {
      moveRandomicly(m);
    }
  }
}    

void IA3_monsters(Hero& m) {
  m.AGRS = 1;
  if(float(m.HP)/m.MAX_HP > 0.35) {
    if(is_in_cam(m.POS.i, m.POS.j) && !canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j)) {
      if(is_ranger) {
        IA1_ranger_monsters(m);
      } 
      else {
        IA1Monsters(m);
      }
    } 
    else {
      int follow;
      for(int i = 0; i < num_monster; i++) {
        follow = 0;
        if(monsters[i].AGRS) {
          if(!canSee(m.POS.i, m.POS.j, monsters[i].POS.i, monsters[i].POS.j) 
            && is_in_cam(monsters[i].POS.i, monsters[i].POS.j) && distance(m.POS, monsters[i].POS) < 7) {

            Position np = runForest(m, hero, 1, 1);
            m.setPos(np.i, np.j);
            follow = 1;
            break;
          }
        }
      }
      if(!follow) {
        moveRandomicly(m);
      }
    } 
  } 
  else {
    if(is_in_cam(m.POS.i, m.POS.j) && !canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j)) {
      Position np = runForest(m, hero, -1, -4);
      m.setPos(np.i, np.j);
    } 
    else {
      int follow;
      for(int i = 0; i < num_monster; i++) {
        follow = 0;
        if(monsters[i].HP > m.HP) {
          Position np = runForest(m, monsters[i], 1, 0);
          m.setPos(np.i, np.j);
          follow = 1;
          break;
        }
      }
      if(!follow) {
        moveRandomicly(m);
      }
    }
  }
}

void print_game_over(){
  tv.clear_screen();
  tv.select_font(font8x8); 
  tv.draw_circle(tv.hres()/2,tv.vres()/2,tv.vres()/3,WHITE);
  tv.set_cursor(25, 42);
  tv.println("GAME OVER!!");
  tv.delay(3000);
  closing_credits();
  while(1);
  //setup();
}

void move_projeteis() {
  for(int i = 0; i < 200; i++) {
    if(projeteis[i].A == 1) {
      projeteis[i].TKT--;
      if(projeteis[i].TKT <= 0) {
        projeteis[i].TKT = TICKET - projeteis[i].SPD;

        if(is_in_cam(projeteis[i].POS.i, projeteis[i].POS.j)){
          if (projeteis[i].POS.d == LEFT) {
            projeteis[i].POS.j -= 1;
            atackPos(projeteis[i].POS.i, projeteis[i].POS.j, projeteis[i].ATK, char(142));
          } 
          else if (projeteis[i].POS.d == RIGHT) {
            projeteis[i].POS.j += 1;
            atackPos(projeteis[i].POS.i, projeteis[i].POS.j, projeteis[i].ATK, char(140));
          } 
          else if (projeteis[i].POS.d == UP) {
            projeteis[i].POS.i -= 1;
            atackPos(projeteis[i].POS.i, projeteis[i].POS.j, projeteis[i].ATK, char(139));
          } 
          else if (projeteis[i].POS.d == DOWN) {
            projeteis[i].POS.i += 1;       
            atackPos(projeteis[i].POS.i, projeteis[i].POS.j, projeteis[i].ATK, char(141));
          }

          if(isColliding(projeteis[i].POS.i, projeteis[i].POS.j) || has_monster(projeteis[i].POS.i, projeteis[i].POS.j) == 1 
            || (hero.POS.i == projeteis[i].POS.i && hero.POS.j == projeteis[i].POS.j)) {
            projeteis[i].A = 0;
            break;
          }
        } 
        else projeteis[i].A = 0; 
      }  
    }
  }
}

void closing_credits() {
  tv.clear_screen();
  tv.select_font(font8x8);
  tv.delay(1500);
  tv.print("Como");
  tv.delay(1500);
  tv.print(" se");
  tv.delay(1500);
  tv.print(" fuesse\n");
  tv.delay(2000);
  tv.print(" L");
  tv.delay(1000);
  tv.print(" A");
  tv.delay(1000);
  tv.print(" D");
  tv.delay(1000);
  tv.print(" I");
  tv.delay(1000);
  tv.print(" N");
  tv.delay(1000);
  tv.print(" O");
  tv.delay(1000);
  tv.print(" !\n\n\n\n");
  tv.select_font(font6x8);
  tv.delay(3000);
  tv.println("   Criador por: \n");
  tv.delay(3000);
  tv.println("Vanderson Rosario; e\n");
  tv.delay(3000);
  tv.println("Vinicius Cornieri ");
  tv.delay(8000);
  tv.clear_screen();
  while(1) tv.delay(1000);
}

PROGMEM const char objL0[] = "Kill 15 monsters!";
PROGMEM const char objL1[] = "Find the portal (@)!";
PROGMEM const char objL2[] = "Find the chest!";
PROGMEM const char objL3[] = "    YOU WIN!!!";


void floor1() {
  if(floor_state == 0) {
    floor_state++;
    portal.i = 200;
    portal.j = 200;
    bau.i = 200;
    bau.j = 200;
    tv.select_font(font8x8);
    tv.clear_screen();
    tv.println("FLOOR 1\n");
    tv.select_font(font6x8);
    tv.printPGM(objL0);
    wait_r();
    loading();
    randomSeed(analogRead(0));
    clean_map();
    createRoom(4, sizeX/2, 15);
    num_monster = 30;
    init_monsters();
    init_cam();
  }
}

void floor2() {
  if(floor_state == 1 && hero.KILLS >= 15) {
    floor_state++;
    tv.select_font(font8x8);
    tv.clear_screen();
    tv.println("FLOOR 2\n");
    tv.select_font(font6x8);
    tv.printPGM(objL1);
    wait_r();
    loading();
    randomSeed(analogRead(0));
    hero.POS.i = 1;
    hero.POS.j = 1;
    clean_map();
    createRoom(4, sizeX/2, 15);
    for(;;){
      portal.i = random(4, sizeY);
      portal.j = random(4, sizeX);
      if(get_position(portal.i, portal.j) != char(144))
        break;
    }
    num_monster = 35;
    init_monsters();
    init_cam();
  }
}

void floor3() {
  if(floor_state == 2 && hero.POS.i == portal.i && hero.POS.j == portal.j) {
    floor_state++;
    portal.i = 200;
    portal.j = 200;
    tv.select_font(font8x8);
    tv.clear_screen();
    tv.println("FLOOR 3\n");
    tv.select_font(font6x8);
    tv.printPGM(objL2);
    wait_r();
    loading();
    randomSeed(analogRead(0));
    hero.POS.i = 1;
    hero.POS.j = 1;
    clean_map();
    createRoom(4, sizeX/2, 15);
    for(;;){
      bau.i = random(sizeY/2, sizeY);
      bau.j = random(sizeX/2, sizeX);
      if(get_position(bau.i, bau.j) != char(144))
        break;
    }
    num_monster = 45;
    init_monsters();
    init_cam();
  }
}

void floor4() {
  if(floor_state == 3 && hero.POS.i == bau.i && hero.POS.j == bau.j) {
    floor_state++;
  }
}

void floor5() {
  if(floor_state == 4) {
    tv.delay(10000);
    closing_credits();
  }
}


void setup(){
  num_projeteis = 0;

  randomSeed(analogRead(0));
  tv.begin(NTSC);
  tv.select_font(font6x8);

  if(hero.HP != 0) 
   print_intro();
  select_class();

  tv.delay(1000);
}

int mp_count = 15;
void recover_mana() {
  if(mp_count == 0) {
    if(hero.MP < hero.MAX_MP) {
      if(hero.HERO_CLASS == WIZARD) hero.MP++;
      hero.MP++;
    }
    mp_count = 15;
  } 
  else {
    mp_count--;
  }
}

void loop(){
  floor1();
  floor2();
  floor3();
  floor4();
  floor5();

  check_speed();
  check_meteors();
  recover_mana();
  print_botton();
  move_projeteis();
  hero.TKT--;
  
  if(hero.TKT <= 0){
    hero.TKT = TICKET - hero.SPD;
    control();
  }
  
  bilheteria_monstros();
  moveCam();
  print_field();
  if(hero.HP<=0) print_game_over();
  //tv.delay(250);
}



























