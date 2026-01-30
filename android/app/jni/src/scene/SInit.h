#pragma once
#include "../engine/Font.h"
#include "Scene.h"

class SInit : public Scene
{
  public:
    void enter() override;
    void update(uint64_t delta) override;
    void draw(SDL_Renderer *renderer) override;
    void input(SDL_Event *event) override;

  private:
    Font main_font;
};