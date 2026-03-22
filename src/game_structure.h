#ifndef GAME_STRUCTURE_H
#define GAME_STRUCTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "scene_main.h"
#include "game_rand.h"


class game  {
public:

    ~game();
    // 获取游戏类的单例实例的静态方法
    // 使用静态局部变量实现单例模式，确保全局只有一个实例
    static game& getInstance(){
        // 声明一个静态的game类实例，它将在第一次调用时被创建
        // 之后每次调用都会返回这个已存在的实例
        static game instance;
        return instance;
    }

    void gameInit();
    void gameRun();
    void gameExit();

    void gameUpdate();
    void gameRender();
    void gameHandleEvents(SDL_Event* event);
    void gameChangeScene(SceneBase* scene);

    // 获取窗口的方法
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    float getDeltaTime() const { return deltaTime; }

    SDL_Window* getWindow() const { return window; }
    SDL_Renderer* getRenderer() const { return renderer; }

   

private:
    game(); // 私有构造函数，防止外部实例化

    //去掉复制构造函数和赋值运算符
    game(const game&) = delete;
    game& operator=(const game&) = delete;


    int windowWidth = 450;
    int windowHeight = 700;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture *BGtexture = nullptr;
    bool isRunning = true;
    SceneBase* currentScene = nullptr;

    uint32_t FPS = 60;
    float deltaTime = 0; // 每帧的时间间隔 60 FPS 约 16.67ms


};



#endif // GAME_STRUCTURE_H