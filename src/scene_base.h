#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <SDL.h>

class SceneBase {
public:
    SceneBase() = default;
    virtual ~SceneBase()    = default;

    virtual void init() = 0;
    virtual void clean() = 0;
    virtual void render() = 0;
    virtual void update() = 0;
    virtual void handleEvent(SDL_Event* event) = 0;
};

#endif // SCENE_H