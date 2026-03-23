#include "game_structure.h"

game::game()
{
     //  构造函数就调用游戏整体初始化,不在这里操作 主动初始化吧
}

game::~game()
{
    gameExit();
}

// 游戏类的初始化函数
void game::gameInit()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s", SDL_GetError());
        isRunning = false;
    }
    // 初始化SDL图像子系统 include PNG and JPG SVG etc...
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s", IMG_GetError());
        isRunning = false;
    }

    //初始化mixer
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_Init Error: %s", Mix_GetError());
        isRunning = false;
    }
    //  初始化SDL音频子系统
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_OpenAudio Error: %s", Mix_GetError());
        isRunning = false;
    }

    //读取音频
    Mix_Music* music = Mix_LoadMUS("game-packs/音乐/Mars.wav"); //game-packs\音乐\Mars.wav
    
    // 初始化SDL_TTF字体子系统

    //创建window and renderer
    window = SDL_CreateWindow("英式W-S-A-D方向 按下空格键 射击", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s", SDL_GetError());
        isRunning = false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s", SDL_GetError());
        isRunning = false;
    }
    currentScene = new scene_main();  // 初始化场景，避免定义场景指针为空
    currentScene->init();

        //加载一个纹理 background
    BGtexture = IMG_LoadTexture(renderer, "game-packs\\BG1.png");
    if (BGtexture == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateTexture Error: %s", SDL_GetError());
        isRunning = false;
    }
    //BGtexture 颜色对比度调整
    SDL_SetTextureColorMod(BGtexture, 200, 200, 200);
    
    //设置channel 
    Mix_AllocateChannels(64);
    //播放bg音乐
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16); 
    Mix_Volume(-1, MIX_MAX_VOLUME / 8); // 设置音量
}



void game::gameExit()
{   
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    
    SDL_DestroyTexture(BGtexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}


void game::gameUpdate()
{
    currentScene->update();
}


void game::gameRender()
{

    // 渲染初始背景
    SDL_RenderClear(renderer);
    if (BGtexture != nullptr){
            SDL_RenderCopy(renderer, BGtexture, NULL, NULL);
    }
    
    //渲染当前场景信息
    if (currentScene != nullptr){
        currentScene->render();
    }
    
    SDL_RenderPresent(renderer); // 最终渲染结果
}


void game::gameHandleEvents(SDL_Event *event){

    while (SDL_PollEvent(event))  // 检查game初始事件
        {
            // 检查是否收到退出事件
            if (event->type == SDL_QUIT)
            {
                isRunning = false;  // 如果收到退出事件，设置运行状态为false
        }
        currentScene->handleEvent(event);  // 处理当前场景的事件
    }
}

void game::gameChangeScene(SceneBase *scene)
{
    if (currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

/**
 * 游戏主循环函数
 * 负责处理游戏事件、更新游戏状态和渲染画面
 */
void game::gameRun()    
{
    while (isRunning)
    {
        SDL_Event event;  // 事件初始化，一般不要直接定义成指针 初始化后为空指针 ！！

        auto frame_start = SDL_GetTicks();  // 获取当前时间戳
        // 处理事件
        gameHandleEvents(&event);
        // 更新游戏状态
        gameUpdate();
        // 渲染画面
        gameRender();

        //------------------------------计算帧时间---------------------------------//
        
        auto frame_end = SDL_GetTicks();  // 获取当前时间戳
        auto frame_duration = frame_end - frame_start;  // 计算帧时长
        if (frame_duration < (1000/FPS))  // 如果帧时长小于16ms，则等待剩余时间
        {
            SDL_Delay((1000/FPS) - frame_duration);
            deltaTime = 1.0f/FPS; // 更新帧时间
        }
        else{
            deltaTime = frame_duration/1000.0f; // 更新帧时间
        }

        // print Info
        // std::cout << "deltaTime: " << deltaTime <<"\tReal FPS: " << 1/deltaTime
        //              <<"\tTime:" << SDL_GetTicks()/1000 << std::endl;

    }
    
}