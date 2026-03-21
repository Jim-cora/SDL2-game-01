#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


using namespace std;

int main(int, char**)
{
    
    std::cout << "Hello World!\n";
    
    // int a;

    //Initialize SDL all
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        std::cout << "SDL_image Initial Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return 1;
    }
/*============================================ 任务/workspace ==============================================*/
    //创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    //创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //加载图片
    SDL_Texture* texture = IMG_LoadTexture(renderer, "game-packs/texture/ShipsPNG/ship8.png");
    
    //加载字体
    TTF_Font* font = TTF_OpenFont("game-packs/ken_fonts/01-PixelFont.ttf", 30);
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, "z 导电膏和中国", { 255, 255, 255, 255 });
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, surface);

    //读取音频
    Mix_Music* music = Mix_LoadMUS("game-packs/音乐/Venus.wav");

        //播放音乐
        Mix_PlayMusic(music, -1);
/*================================================= End ===================================================*/

    //渲染循环
    while (true)
    {
        //清除渲染器
        // SDL_RenderClear(renderer);

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
        }
        SDL_RenderClear(renderer); //清屏
        //creat rect
        SDL_Rect rect = { 100, 100, 300, 300 };
        //渲染rect
        SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        //渲染字体
        SDL_Rect text_rect = { 300, 350, surface->w, surface->h};
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        //恢复渲染器默认颜色
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        //渲染图片
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer); //更新展示渲染器


    }

/*================================================= End ===================================================*/
    //清理音乐资源
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();

    //销毁图片纹理、推出SDL-image
    SDL_DestroyTexture(texture);
    IMG_Quit();

    //销毁字体、推出SDL-ttf
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(font);
    TTF_Quit();

    //销毁渲染器、窗口
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


    