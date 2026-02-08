#include "S_Init.hpp"

#include "../engine/AudioMgr.hpp"
#include "../engine/FontMgr.hpp"
#include "../engine/ImageMgr.hpp"

void S_Init::update(float delta) {
  img_pos_x += delta;
  if (img_pos_x >= G::WIDTH) img_pos_x = 0;
}

void S_Init::draw(SDL_Renderer* renderer) {
  G::font->draw(renderer, "main", "你好\n我是ws3917", 200, 200, 2.0f);
  G::image->draw(renderer, "icon", G::WIDTH / 2.0f, 20, 0, 0.5f);
  G::image->draw(renderer, "test1", img_pos_x, 200);
  G::image->draw(renderer, "test2", img2_pos.x, img2_pos.y, 0, 2.0);
}

void S_Init::input(SDL_Event* event) {
  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.scancode) {
      case SDL_SCANCODE_Z:
        G::audio->play("snd_test1");
        break;
      case SDL_SCANCODE_X:
        G::audio->play("snd_test2");
        break;
      case SDL_SCANCODE_LEFT:
        G::audio->stop("mus_test");
        break;
      case SDL_SCANCODE_RIGHT:
        G::audio->play("mus_test");
      default:
        break;
    }
  }
  if (event->type == SDL_EVENT_MOUSE_MOTION) {
    img2_pos.x = event->motion.x;
    img2_pos.y = event->motion.y;
  }
}

void S_Init::enter() { G::audio->play("mus_test"); }