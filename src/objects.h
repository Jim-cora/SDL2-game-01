#ifndef OBJECTS_H
#define OBJECTS_H

#include "SDL.h"

struct Player {

    SDL_Texture* texture;   //the player image
    SDL_FPoint position = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 150;  //speed of the player
    int health = 3; //health of the player
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
    int damage = 1; //damage of the player bullet
    Uint32 coolDown = 500; //ms
    Uint32 lastCoolTime = 0; //ms
};


struct Enemy_Template {
    SDL_Texture* texture;   //the enemy image
    SDL_FPoint position = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 100;  //speed of the enemy
    int health = 2; //health of the enemy
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
    int damage = 1; //damage of the enemy bullet

    
};

struct explosion_template {
    SDL_Texture* texture;   //the explosion image
    SDL_FPoint position = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int frame_width = 0;  //width of the frame
    int frame_height = 0; //height of the frame
    int current_frame = 0; //current frame
    int total_frames = 0; //total frames
    Uint32 startTime = 0; //ms
    Uint32 fps = 10; //ms
};


#endif // OBJECTS_H