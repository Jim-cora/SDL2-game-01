#ifndef OBJECTS_H
#define OBJECTS_H

#include "SDL.h"

struct Player {

    SDL_Texture* texture;   //the player image
    SDL_FPoint position = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 150;  //speed of the player
    int health = 100; //health of the player
    bool isDead = false; //玩家是否死亡
    int score = 0; //score of the player
    int level = 1; //difficult level of the player
    
};


struct Projectile_Player_Template {
    SDL_Texture* texture;   //bullet image of the player
    SDL_FPoint position = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 250;  //speed of the player bullet
    Uint32 coolDown = 500; //ms
    Uint32 lastCoolTime = 0; //ms
};


struct Enemy_Template {
    SDL_Texture* texture;   //the enemy image
    SDL_FPoint position = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 100;  //speed of the enemy
    Uint32 coolDown = 2500; //ms
    Uint32 lastCoolTime = 0; //ms
};

struct Projectile_Enemy_Template {
    SDL_Texture* texture;   //bullet image of the enemy
    SDL_FPoint position = {0,0};    //x and y
    SDL_FPoint direction = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 200;  //speed of the enemy

    
};

#endif // OBJECTS_H