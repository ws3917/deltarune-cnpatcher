#pragma once
#include "Scene.h"
class SInit : public Scene
{
  public:
    void update(uint64_t delta) override;
    void draw(SDL_Renderer *renderer) override;
    void input(SDL_Event *event) override;
};