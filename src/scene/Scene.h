#pragma once
#include <SDL3/SDL.h>
// Interface: Scene
class Scene
{
  public:
    Scene() = default;
    virtual ~Scene() = default;
    virtual void enter() {}
    virtual void exit() {}
    virtual void update(uint64_t delta) = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;
    virtual void input(SDL_Event *event) = 0;

    int id;
};