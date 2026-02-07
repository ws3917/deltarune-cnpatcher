#pragma once
#include "../engine/Scene.hpp"

class S_Init : public Scene {
 public:
  void enter() override;
  void update(float delta) override;
  void draw(SDL_Renderer* renderer) override;
  void input(SDL_Event* event) override;

 private:
  float img_pos_x = 0;
};