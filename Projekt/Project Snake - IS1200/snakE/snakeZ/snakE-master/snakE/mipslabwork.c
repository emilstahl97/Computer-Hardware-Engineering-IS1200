/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>   /*includes standardlibrary to use function rand()*/
#include "standard.h" /*includes the use of functions rand()*/

/*Defines the size of the display, which the game is displayed on.
*The number of pixels on the displayed screen is 32x32,
*but horisontally we represent 8 pixels with a block that is 8pixels since one element in the char array represents 8 pixels horisontally on the OLED.
*- Written by Emil Ståhl*/
#define HEIGHT 4
#define WIDTH 32
#define SIZE_OF_PAGE HEIGHT*WIDTH
#define PIXEL_UNIT 8
#define NUMBER_OF_PAGES 2

/*Global variables that the game utilizes
*- Written by Emil Ståhl*/
volatile int *porte = (volatile int *) 0xbf886110;
int difficulty = 200;
int stageOfGame = 0;
int snakeX = 15;
int snakeY = 15;
int foodX = 20;
int foodY = 20;
int posFood;
int velocity = 0;
int posArray;
int score = 0;

	


/* Lab-specific initialization goes here */

/*Set up the buttons and switches to be inputs, and output on the lights.
*Implements a timer.
*- Written by Emil Ståhl*/
void labinit( void ){
  volatile int *trise = (volatile int *) 0xbf886100;
  *trise = *trise & 0x00;
  TRISE = TRISE & 0x0fe0;
  TRISF = TRISF & 0x2;
  PR2 = (80000000/256)/10;
  T2CON = 0x70;
  TMR2 = 0;
  T2CONSET = 0x8000;
  return;
}

/*Functions that returns a generated number based on timer. number is being used in srand() - Written by Emil Ståhl*/
int feedSeed(){
  return TMR2;
}

/*Function that gets the position of the snake in the char array "display" (located in file mipslabdata.h),
*represents the position and is used to move the snake around.
*- Written by Emil Ståhl*/
int get_posSnake(){
  int i = snakeY / PIXEL_UNIT;
  return posArray = snakeX + i*32;
}

/*Function that gets the position of the food in the char array "display" (located in file mipslabdata.h),
*represents the position and is used to both display the food and to generate a new position for it.
*- Written by Emil Ståhl*/
int get_posFood(){
  int i = foodY / PIXEL_UNIT;
  return posFood = foodX + i*32;
}

/*Function that turns off the specific light that represents either the food or snake
*in the char array "display" and on the OLEDdisplay aswell.
*- Written by Emil Ståhl*/
void set_pixel(int x, int y){
  int i = y / PIXEL_UNIT;
  display[x + i*32] =  1 << (y - i * PIXEL_UNIT);
}

/*Resets the lights that the snake or food is not represented on in the char array "display".
*- Written by Emil Ståhl*/
void update(){
  int j;
  for(j = 0; j < SIZE_OF_PAGE; j++){
    if(j != posArray && j != posFood)
      display[j] = 0;
  }
}

/*Turns the snake to the left, also changes it's velocity so it keeps moving constantly to the left untill other functions are called.
*- Written by Emil Ståhl*/
void move_Left(){
  velocity = -1;
  
  posArray += velocity;
}

/*Works the same as the function move_left, but moves it to the Right instead.
*- Written by Emil Ståhl*/
void move_Right(){
  velocity = 1;
  
  posArray += velocity;
}

/*Moves the snake up on the screen, does this by changing the position of the snake in the array by -32, since the width of the screen is 32bits.
*- Written by Emil Ståhl*/
void move_Up(){
  velocity = -32;
 
  posArray += velocity;
}

/*Works the same as function move_Up, but move it down instead by adding 32 to the variable posArray
 *- Written by Emil Ståhl*/
void move_Down(){
  velocity = 32;
 
  posArray += velocity;
}

/*Reads the values of the switches 1,2, & 3 and represents them with a binary number.
*Based on the binary number, a difficulty is chosen, may be switched while playing
 *- Written by Emil Ståhl*/
void set_difficulty(){
  int sw;
  if(sw = getsw()){
    if(getsw() == 0x3)
      difficulty = 50;

    if(getsw() == 0x2)
      difficulty = 100;

    if(getsw() == 0x1)
      difficulty = 150;
  }
}

/*Function that updates the movement of the snake based on which button is pressed.
*Does this by calling the move_(DIRECTION) functions.
 *- Written by Emil Ståhl*/
void buttons(){
  int but;
  if(but = getbtns()){
    if(but & 4){
      move_Left();
    }
    if(but & 2){
      move_Down();
    }
    if(but & 1){
      move_Up();
    }
  }

  if(but = btn1()){
    move_Right();
  }
}

/*Function that is called the function check_collide occurs.
*Creates a new random position for the food by using function rand() & increases the score represented on the lights by 1
 *- Written by Emil Ståhl*/
void food(){
  set_pixel(foodX,foodY);
  *porte += 0x01;
  score++;
  if(difficulty>50)
	  difficulty-=5;
}

/*Function that is constantly called in labwork to check if you eat the food or not.
*Makes use of function food()
 *- Written by Emil Ståhl*/
void check_collide(){
  if(snakeX == foodX && snakeY == foodY){
    foodX = rand() % 31;
    foodY = rand() % 31;
    food();
	
  }
}

/*Moves the snake constantly based on what button was previously pressed,
*implements this by checking the current velocity/direction that is decided in functions move_(DIRECTION)
 *- Written by Emil Ståhl*/
void move_Constant(){
  if(velocity == 1)
    snakeX++;

  if(velocity == -1)
    snakeX--;

  if(velocity == 32)
    snakeY++;

  if(velocity == -32)
    snakeY--;
}

/*Function that clears the screen, is called both when you reset the game and each time labwork calls.
*Hence the use of parameters that declare what type of screen you want to clear with and which part of the screen.
*By using these parameters we can recycle the function and not have to make a specific one for when we either wanna reset the game or update the screen.
 *- Written by Emil Ståhl*/
void display_clear(int amount, uint8_t screen[]){
  int i;
  for(i = 0; i < amount*WIDTH; i += 32){
    display_image(i, screen);
  }
}

/*Restores all the starting values for the game,
*including: position of snake, position of food, velocity, score and clears the screen
 *- Written by Emil Ståhl*/
void reset(){
  snakeX = 15;
  snakeY = 15;
  foodX = rand() % 31;
  foodY = rand() % 31;
  velocity = 0;
  *porte = 0x0;
   set_difficulty();
  display_clear(NUMBER_OF_PAGES*2,death);
}

/*Sort of unnecessary function, is always called but only used if you achieved the highest score possible,
*which is score of 256, or 2^8 , or 11111111 on the lights
 *- Written by Emil Ståhl*/
void game_win(){
  if(score > 255){
    int but;
    while(1){
      display_update();
      display_string(0, "YOU WON!");
      display_string(1, "SCORE ON LIGHTS");
      display_string(2, "TO RESTART");
      display_string(3, "PRESS BUTTON 1");
      if(but = btn1()){
        reset();
        break;
      }
    }
  }
}

/*Used to generated a seed that is necessary for the rand() function to work properly.
*It achieves this by taking the value of the timer when the player starts the game by pressing button 1 on the IO-Shield.
*It calls the function feedSeed() and thereby gives it a pseudo-random value to use as reference.
 *- Written by Emil Ståhl*/
void startup(){
  display_update();
  display_string(0, "WELCOME TO");
  display_string(1, "snakE");
  display_string(2, "To start");
  display_string(3, "press BUTTON 1");
  int but;
  if(but = btn1()){
    srand(feedSeed());
    reset();
    stageOfGame = 1;
  }
}

/*Checks if the player moves out of the screen represented by the char array "display" in file mipslabdata.c
*If the if statement is true, then you first clear the screen entirely and then display text to guide the player to restart the game.
*If the player chooses to restart the game, the function reset() will be called.
 *- Written by Emil Ståhl*/
void game_over(){
  if(snakeX < 0 || snakeX > 31 || snakeY < 0 || snakeY > 31){
    int i;
    int but;
    for(i = 0; i < WIDTH*NUMBER_OF_PAGES; i += WIDTH){
      display_image(i, clear);
    }
    while(1){
      display_update();
      display_string(0, "GAME OVER!");
      display_string(1, "SCORE ON LIGHTS");
      display_string(2, "TO RESTART");
      display_string(3, "PRESS BUTTON 1");

      if(but = btn1()){
        reset();
        break;
      }
    }
  }
}

/* This function is called repetitively from the main program */

/*Main function that the games basic functions are called in,
*Uses switch case to declare in what state the game is in, either start menu or playing.
 *- Written by Emil Ståhl*/
void snakE( void ){
  switch(stageOfGame){
    case 0:
      startup();
	  set_difficulty();
    break;

    case 1:
      while(1){
        delay(difficulty);
        game_over();
        game_win();
        buttons();
        display_clear(NUMBER_OF_PAGES/2,clear);
        move_Constant();
        get_posSnake();
        get_posFood();
        check_collide();
        set_pixel(foodX, foodY);
        set_pixel(snakeX, snakeY);
        update();
        display_image(0,display);
      }
      break;
  }
}
