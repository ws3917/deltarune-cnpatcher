#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cstdint>

#include "engine/Game.hpp"
#include "scene/S_Init.hpp"
int main(int, char**) {
  // 强制UTF-8，防止用GBK出现乱码
  //    std::locale::global(std::locale(".UTF-8"));
  Game* game = Game::get();
  SDL_Log("-- Game Start --\n");
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("[C] <Init> SDL init failed: %s", SDL_GetError());
    return -1;
  }
  if (!MIX_Init()) {
    SDL_Log("[C] <Init> SDL_mixer init failed: %s", SDL_GetError());
    return -1;
  }
  game->init();
  // 进入初始场景
  game->enterScene(new S_Init);
  SDL_Event event;
  uint64_t last_tick = 0;
  while (game->getState() == SDL_APP_CONTINUE) {
    while (SDL_PollEvent(&event)) game->input(&event);

    uint64_t current_tick = SDL_GetTicksNS();
    if (last_tick == 0) last_tick = current_tick;
    float delta = (current_tick - last_tick) / 1e6f;
    last_tick = current_tick;
    game->update(delta);
    game->draw();
  }
  game->exit();
  MIX_Quit();
  SDL_Quit();
  if (game->getState() == SDL_APP_FAILURE) {
    SDL_Log("-- Game End With Error --");
    return -1;
  }
  SDL_Log("-- Game End Successfully --");
  return 0;
}