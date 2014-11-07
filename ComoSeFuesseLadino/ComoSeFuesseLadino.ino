#include <SNESpaduino.h>
#include <TVout.h>
#include <fontALL.h>
#include "map.h"
#include "hero.h"
#include "avr/pgmspace.h"
#include "fontRL.h"

TVout tv;
SNESpaduino player1(20, 21, 19);

uint16_t btns;
Hero hero(1);

#define num_monster 30
Hero monsters[100];

Position camH;
Position camV;
PROGMEM const char history[] = "Aqui vosmece encontrara o mundo de RUP, que foi d7minado por um Diagrametico TROLL.\n Jose, voce foi REQUISITADO a libertar esse mundo e por fim a todo esse desperdicio de papel...PRESS [R]";
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

void print_screen(int x, int y, char c){
  tv.set_cursor(y*6,x*8);
  tv.print(c);
}

int has_monster(int x, int y){
  for(int i = 0; i< num_monster; i++){
    if(monsters[i].POS.i == x && monsters[i].POS.j == y){
      return 1;
    }
  }
  return 0;
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
   tv.print("NAME : ");
   tv.println("JOSE");
   tv.set_cursor(2,25);
   tv.print("LVL  : ");
   tv.println(hero.LVL);
   tv.set_cursor(2,42);
   tv.print("HP   : ");
   tv.println(hero.HP);
   tv.set_cursor(2,50);
   tv.print("SPD  : ");
   tv.println(hero.SPD);
   tv.set_cursor(2,58);
   tv.print("ATK  : ");
   tv.println(hero.ATK);
   tv.set_cursor(73,81);
   
   tv.print("PRESS [R]");
   btns = player1.getButtons();
   while(!(btns & BTN_R)) btns = player1.getButtons();
   //print_field();
}

void print_field(){
  //tv.select_font(fontRL);
  tv.select_font(font6x8);
  //tv.clear_screen();
  for(byte i = camV.i; i < camV.j; i++) {
    for(byte j = camH.i; j < camH.j; j++) {
      if(hero.POS.i == i && hero.POS.j == j)
          //print_screen(i-camV.i, j-camH.i, (char(128 + hero.POS.d)));
          print_screen(i-camV.i, j-camH.i, '@');
      else
        if(canSee(hero.POS.i, hero.POS.j, i, j) != 1){
          if(!has_monster(i,j)){
            print_screen(i-camV.i, j-camH.i, (get_position(i, j)));
          }else
          //print_screen(i-camV.i, j-camH.i, char(134));
          print_screen(i-camV.i, j-camH.i, '%');
        }
        else
          print_screen(i-camV.i, j-camH.i, ' ');
    }
    //tv.print("\n");
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
 }else if(btns & BTN_B){
   Hero n_hero(BARBARIAN);
   hero = n_hero;
   tv.println("BARBARIAN");   
 }else if(btns & BTN_Y){
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
  tv.set_cursor(55,60);
  tv.println("pre-alpha");
  tv.delay(5000);
  tv.clear_screen();
  
}

void loading() {
  tv.set_cursor(40,60);
  tv.select_font(font8x8);
  tv.println("LOADING...");
  tv.select_font(font6x8);
}

void control() {
    btns = player1.getButtons();
    if(btns & BTN_A){
      
    }if (btns & BTN_B){

    }if(btns & BTN_DOWN){
      hero.setPos(hero.POS.i + 1, hero.POS.j);
      hero.POS.d = 2;
    }if(btns & BTN_UP){
      hero.setPos(hero.POS.i - 1, hero.POS.j);
      hero.POS.d = 0;
    }if(btns & BTN_LEFT){
      hero.setPos(hero.POS.i, hero.POS.j - 1);
      hero.POS.d = 1;
    }if(btns & BTN_RIGHT){
      hero.setPos(hero.POS.i, hero.POS.j + 1);
      hero.POS.d = 3;
    }if(btns & BTN_Y){

    }if(btns & BTN_X){

    }if(btns & BTN_L){
      print_sheet();
    }if(btns & BTN_R){
      //print_sheet();
    }if(btns & BTN_START){

    }if(btns & BTN_SELECT){

    }
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


void IA1Monsters() {
  for(int i = 0; i < num_monster; i++) {
    monsters[i].setPos(monsters[i].POS.i + random(0, 3)-1, monsters[i].POS.j + random(0, 3)-1);
  }
}


void setup(){
  randomSeed(analogRead(0));
  Serial.begin(9600);
  tv.begin(NTSC);
  tv.select_font(font6x8);
  print_intro();
  select_class();
  loading();
  createRoom(4, sizeX/2, 15);
  init_monsters();
  init_cam();
  
  tv.delay(1000);
}

void loop(){
  IA1Monsters();
  control();
  moveCam();
  print_field();
  tv.delay(200);
}
