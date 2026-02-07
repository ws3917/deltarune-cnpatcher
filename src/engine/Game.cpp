#include "Game.hpp"

#include "AudioMgr.hpp"
#include "FontMgr.hpp"
#include "ImageMgr.hpp"


void Game::init() {
  // 创建窗口
  constexpr int WIDTH = 960, HEIGHT = 720;
  window =
      SDL_CreateWindow("title", WIDTH, HEIGHT,
                       SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!window || !renderer) {
    SDL_Log("[C] <Init> Can't create window / renderer: %s", SDL_GetError());
    abort(true);
  }

  // 垂直同步
  SDL_SetRenderVSync(renderer, 1);

  // 高分辨率
  SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_LINEAR);
  float scale = SDL_GetWindowDisplayScale(window);
  SDL_SetRenderLogicalPresentation(renderer, WIDTH, HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  SDL_SetWindowSize(window, static_cast<int>(WIDTH * scale),
                    static_cast<int>(HEIGHT * scale));

  // 音频
  if (!AudioMgr::get()->init()) {
    abort(true);
  }
}
void Game::enterScene(Scene* scene) {
  current_scene = scene;
  if (!current_scene) {
    SDL_Log("[C] <Init> Can't enter initial scene.");
    abort(true);
  }
  current_scene->enter();
}
void Game::input(SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) abort(false);

  SDL_ConvertEventToRenderCoordinates(renderer, event);
}
void Game::draw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  current_scene->draw(renderer);
  SDL_RenderPresent(renderer);
}
void Game::update(float delta) { current_scene->update(delta); }
void Game::abort(bool error) {
  state = error ? SDL_APP_FAILURE : SDL_APP_SUCCESS;
};
void Game::exit() {
  AudioMgr::get()->exit();
  ImageMgr::get()->exit();
  FontMgr::get()->exit();
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
}
SDL_AppResult Game::getState() { return state; }