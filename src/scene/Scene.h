#pragma once
#include <SDL3/SDL.h>
class Scene
{
  public:
    void update(uint64_t delta);
    void draw(SDL_Renderer* renderer);
    void input(SDL_Event* event);
};