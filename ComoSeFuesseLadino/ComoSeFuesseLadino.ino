#include <SNESpaduino.h>
#include <TVout.h>
#include <fontALL.h>
#include "avr/pgmspace.h"
#include "fontRL.h"
#include "map.h"
#include "IA.h"

SNESpaduino player1(20, 21, 19);

uint16_t btns;

PROGMEM const char history[] = "Aqui vosmece encontrara o mundo de RUP, que foi d7minado por um Diagrametico TROLL.\n Jose, voce foi REQUISITADO a libertar esse mundo e por fim a todo esse desperdicio de papel...PRESS [R]";

TVout tv;

void print_screen(int x, int y, char c){
  tv.set_cursor(y*6,x*8);
  tv.print(c);
}

int print_intro() {
  tv.set_cursor(20, 30);
  //tv.select_font(fontRL);
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
  tv.println("alpha v0.1");
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
  tv.println(hero.LVL);
  tv.set_cursor(2,38);
  tv.print("HP    : ");
  tv.println(hero.HP);
  tv.set_cursor(2,46);
  tv.print("SPD   : ");
  tv.println(hero.SPD);
  tv.set_cursor(2,54);
  tv.print("ATK   : ");
  tv.println(hero.ATK);
  tv.set_cursor(2,62);
  tv.print("KILLS : ");
  tv.println(hero.KILLS);
  tv.set_cursor(73,81);

  tv.print("PRESS [R]");
  btns = player1.getButtons();
  while(!(btns & BTN_R)) btns = player1.getButtons();
  //print_field();
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
            print_screen(i-camV.i, j-camH.i, (get_position(i, j)));
          }
          else
            print_screen(i-camV.i, j-camH.i, char(134));
        }
        else
          print_screen(i-camV.i, j-camH.i, ' ');
    }
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
  while(!((btns & BTN_R) || (btns & BTN_L))) btns = player1.getButtons();
  //tv.delay(8000);
  tv.clear_screen();

  return 0;
}

void atackPos(int i, int j, int dano, char tile) {
  Hero* monster = get_monster(i, j);
  print_screen(i-camV.i, j - camH.i, tile);
  if (monster) {
    if(!monster->damage(dano)) hero.KILLS++;
  }

  if(hero.POS.i == i && hero.POS.j ==  j)
    hero.damage(dano);
}

void cleave_atk(Hero h){
  tv.select_font(fontRL);
  if (h.POS.d == LEFT) {
    atackPos(h.POS.i+1, h.POS.j - 1, h.ATK, char(136));
    atackPos(h.POS.i, h.POS.j - 1, h.ATK, char(136));
    atackPos(h.POS.i-1, h.POS.j - 1, h.ATK, char(136));
  } 
  else if (h.POS.d == RIGHT) {
    atackPos(h.POS.i+1, h.POS.j + 1, h.ATK, char(138));
    atackPos(h.POS.i, h.POS.j + 1, h.ATK, char(138));
    atackPos(h.POS.i-1, h.POS.j + 1, h.ATK, char(138));
  } 
  else if (h.POS.d == UP) {
    atackPos(h.POS.i - 1, h.POS.j+1, h.ATK, char(135));
    atackPos(h.POS.i - 1, h.POS.j, h.ATK, char(135));
    atackPos(h.POS.i - 1, h.POS.j-1, h.ATK, char(135));
  } 
  else if (h.POS.d == DOWN) {
    atackPos(h.POS.i + 1, h.POS.j+1, h.ATK, char(137));
    atackPos(h.POS.i + 1, h.POS.j, h.ATK, char(137));
    atackPos(h.POS.i + 1, h.POS.j-1, h.ATK, char(137));
  }
}

void atack(Hero h) {
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

void control() {
  btns = player1.getButtons();
  if(btns & BTN_A){

  }
  if (btns & BTN_B){

  }
  if(btns & BTN_DOWN){
    hero.setPos(hero.POS.i + 1, hero.POS.j);
    hero.POS.d = DOWN;
  }
  if(btns & BTN_UP){
    hero.setPos(hero.POS.i - 1, hero.POS.j);
    hero.POS.d = UP;
  }
  if(btns & BTN_LEFT){
    hero.setPos(hero.POS.i, hero.POS.j - 1);
    hero.POS.d = LEFT;
  }
  if(btns & BTN_RIGHT){
    hero.setPos(hero.POS.i, hero.POS.j + 1);
    hero.POS.d = RIGHT;
  }
  if(btns & BTN_Y){
    atack(hero);
  }
  if(btns & BTN_X){
    cleave_atk(hero);
  }
  if(btns & BTN_L){
    print_sheet();
  }
  if(btns & BTN_R){
    //print_sheet();
  }
  if(btns & BTN_START){

  }
  if(btns & BTN_SELECT){

  }
}

int is_in_cam(int x, int y) {
  return x > camH.i && x < camH.j && y > camV.i && y < camV.j;
}

void init_monsters() {
  for(int i = 0; i < num_monster; i++) {
    monsters[i].POS.i = random(4, sizeY);
    monsters[i].POS.j = random(4, sizeX);
    if(get_position(monsters[i].POS.i, monsters[i].POS.j) == '#') {
      i--;
      continue;
    }
  }
}

float distance(Position a, Position b) {
  return sqrt(pow(a.i - b.i,2) + pow(a.j - b.j, 2));
}


Position runForest(Hero h, int dir, int hide) {
  Position p;
  p.i = h.POS.i;
  p.j = h.POS.j;
  float min = 999999999;
  Position aux;

  if(get_position(h.POS.i+1, h.POS.j) != '#') {
    aux.i = h.POS.i+1;
    aux.j = h.POS.j;
    float peso = (dir*distance(aux, hero.POS)) - hide* (!canSee(aux.i, aux.j, hero.POS.i, hero.POS.j));

    if(peso < min) {
      p.i = aux.i;
      p.j = aux.j;
      min = peso;
    }
  }

  if(get_position(h.POS.i-1, h.POS.j) != '#') {
    aux.i = h.POS.i-1;
    aux.j = h.POS.j;
    float peso = (dir*distance(aux, hero.POS)) - hide* (!canSee(aux.i, aux.j, hero.POS.i, hero.POS.j));

    if(peso < min) {
      p.i = aux.i;
      p.j = aux.j;
      min = peso;
    }
  }

  if(get_position(h.POS.i, h.POS.j + 1) != '#') {
    aux.i = h.POS.i;
    aux.j = h.POS.j + 1;
    float peso = (dir*distance(aux, hero.POS)) - hide* (!canSee(aux.i, aux.j, hero.POS.i, hero.POS.j));

    if(peso < min) {
      p.i = aux.i;
      p.j = aux.j;
      min = peso;
    }
  }

  if(get_position(h.POS.i, h.POS.j - 1) != '#') {
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

void bilheteria_monstros(){
  for (int i= 0;i<num_monster;i++){
    if(true){
      IA1Monsters(monsters[i]);
      monsters[i].TKT = TICKET / monsters[i].SPD;
    }else{
      monsters[i].TKT--;
    }  
  }
}

void IA1Monsters(Hero m) {
  
    if(distance(m.POS, hero.POS) == 1) {
      if(!is_in_cam(m.POS.j, m.POS.i) && !canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j)) {
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
    else if(!canSee(hero.POS.i, hero.POS.j, m.POS.i, m.POS.j)){
      m.POS = runForest(m, 1, 4);
    } 
    else {
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
  
}

void print_game_over(){
  tv.clear_screen();
  tv.select_font(font8x8); 
  tv.draw_circle(tv.hres()/2,tv.vres()/2,tv.vres()/3,WHITE);
  tv.set_cursor(25, 42);
  tv.println("GAME OVER!!");
  tv.delay(3000);
  setup();
}

void setup(){
  randomSeed(analogRead(0));
  tv.begin(NTSC);
  tv.select_font(font6x8);
  if(hero.HP != 0) 
    print_intro();
  select_class();
  loading();
  createRoom(4, sizeX/2, 15);
  init_monsters();
  init_cam();

  tv.delay(1000);
}

void loop(){
  bilheteria_monstros();
  if(hero.TKT==0){
    hero.TKT = TICKET - hero.SPD;
    control();
  }
  moveCam();
  print_field();
  if(hero.HP==0) print_game_over();
  //tv.delay(250);
}




