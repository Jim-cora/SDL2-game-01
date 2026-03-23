#ifndef OBJECTS_H
#define OBJECTS_H

#include "SDL.h"


/**
 * @enum Bonus_TextureType
 * @brief 定义了不同奖励类型的枚举类
 */
enum class Bonus_TextureType {
    extra_life,      // 额外生命奖励
    extra_shield,    // 额外护盾奖励
    freeze_time,     // 时间冻结奖励
    extra_support,   //rocket help
    BONUS_NONE, //no bonus
};

 // 音效枚举
enum class SoundType {
    Player_Shoot,
    Enemy_Shoot,
    Player_Hit,
    Player_Die,
    Enemy_base_Die,
    Get_Bonus,
};

struct Player {

    SDL_Texture* texture;   //the player image
    SDL_FPoint position = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 150;  //speed of the player
    int health = 3; //health of the player
    int max_health = 5; //shield of the player
    bool isDead = false; //玩家是否死亡
    int score = 0; //score of the player
    int level = 1; //difficult level of the player
    
};


struct Projectile_Player_Template {
    SDL_Texture* texture;   //bullet image of the player
    SDL_FPoint position = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 200;  //speed of the player bullet
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
    float dropRate = 0.7f; //probability of the enemy dropping a bonus item
};

struct Projectile_Enemy_Template {
    SDL_Texture* texture;   //bullet image of the enemy
    SDL_FPoint position = {0,0};    //x and y
    SDL_FPoint direction = {0,0};    //x and y
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 150;  //speed of the enemy
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
    Uint32 fps = 20; //ms
};

struct Item{
    SDL_Texture* texture;   //the item image
    SDL_FPoint position = {0,0};    //x and y
    SDL_FPoint direction = {0,0};  //
    int width = 0;  //width of the image
    int height = 0; //height of the image
    int speed = 100;  //speed of the item
    Bonus_TextureType type = Bonus_TextureType::BONUS_NONE; //type of the item
    int total_cls = 4; //total class of the bonus item
    int bounceCount = 2; //bounce count of the item
    //物品出现概率



};

#endif // OBJECTS_H