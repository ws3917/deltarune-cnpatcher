#include "S_Init.hpp"

#include "../engine/AudioMgr.hpp"
#include "../engine/FontMgr.hpp"
#include "../engine/Game.hpp"
#include "../engine/ImageMgr.hpp"

void S_Init::update(float delta) {
  img_pos_x += delta;
  if (img_pos_x >= Game::WIDTH) img_pos_x = 0;
}

void S_Init::draw(SDL_Renderer* renderer) {
  FontMgr::get()->draw(renderer, "main", "你好\n我是ws3917", 200, 200, 2.0f);
  ImageMgr::get()->draw(renderer, "icon", Game::WIDTH / 2.0f, 20, 0, 0.5f);
  ImageMgr::get()->draw(renderer, "test1", img_pos_x, 200);
  ImageMgr::get()->draw(renderer, "test2", img2_pos.x, img2_pos.y, 0, 2.0);
}

void S_Init::input(SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.scancode) {
      case SDL_SCANCODE_Z:
        AudioMgr::get()->play("snd_test1");
        SDL_Log("snd test1");
        break;
      case SDL_SCANCODE_X:
        AudioMgr::get()->play("snd_test2");
        SDL_Log("snd test2");
        break;
      case SDL_SCANCODE_LEFT:
        AudioMgr::get()->stop("mus_test");
        break;
      case SDL_SCANCODE_RIGHT:
        AudioMgr::get()->play("mus_test");
      default:
        break;
    }
  }
  if (event->type == SDL_EVENT_MOUSE_MOTION) {
    img2_pos.x = event->motion.x;
    img2_pos.y = event->motion.y;
  }
}

void S_Init::enter() { AudioMgr::get()->play("mus_test"); }