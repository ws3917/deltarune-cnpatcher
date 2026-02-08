#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "engine/GameMgr.hpp"
#include "engine/GlobalVars.hpp"

SDL_AppResult SDL_AppInit(void**, int, char**) {
  // 初始化
  SDL_Log("-- Game Start --\n");
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("[C] <Init> SDL init failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!MIX_Init()) {
    SDL_Log("[C] <Init> SDL_mixer init failed: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // 创建窗口
  G::window =
      SDL_CreateWindow(G::NAME, G::WIDTH, G::HEIGHT,
                       SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
  G::renderer = SDL_CreateRenderer(G::window, nullptr);
  if (!G::window || !G::renderer) {
    SDL_Log("[C] <Init> Can't create window / renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // 垂直同步
  SDL_SetRenderVSync(G::renderer, 1);
  SDL_SetRenderLogicalPresentation(G::renderer, G::WIDTH, G::HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  G::game = new GameMgr;
  return G::status;
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;
  SDL_ConvertEventToRenderCoordinates(G::renderer, event);
  G::game->input(event);
  return G::status;
}

SDL_AppResult SDL_AppIterate(void*) {
  uint64_t current_tick = SDL_GetTicksNS();
  if (G::last_tick == 0) G::last_tick = current_tick;
  float delta = (current_tick - G::last_tick) / 1e6f;
  G::last_tick = current_tick;
  G::game->update(delta);
  G::game->draw();
  return G::status;
}

void SDL_AppQuit(void*, SDL_AppResult) {
  delete G::game;
  MIX_Quit();
  SDL_Quit();
  if (G::status == SDL_APP_FAILURE)
    SDL_Log("-- Game End With Error --");
  else
    SDL_Log("-- Game End Successfully --");
}