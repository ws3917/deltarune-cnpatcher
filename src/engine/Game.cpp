#include "Game.hpp"

#include <nlohmann/json.hpp>
#include <string>

#include "AudioMgr.hpp"
#include "FontMgr.hpp"
#include "ImageMgr.hpp"

void Game::init() {
  // 创建窗口
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

  // 加载资源
  SDL_Log("[I] <Init> Start loading assets");
  loadAssets("assets.json");
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
  current_scene->input(event);
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
void Game::loadAssets(const std::string& json_path) {
  size_t size;
  void* json_data = SDL_LoadFile(json_path.c_str(), &size);
  if (!json_data) {
    SDL_Log("[E] <loadAssets> Can't open json file '%s': %s", json_path.c_str(),
            SDL_GetError());
    return;
  }
  nlohmann::json j;
  try {
    j = nlohmann::json::parse((char*)json_data, (char*)json_data + size);
    SDL_free(json_data);
  } catch (const nlohmann::json::parse_error& e) {
    SDL_Log("[E] <loadAssets> Can't parse json file: %s", json_path.c_str());
    return;
  }
  // 音频
  if (j.contains("audio")) {
    SDL_Log("[I] <loadAssets> Loading audio in %s...", json_path.c_str());
    const std::vector<std::string>& files =
        j["audio"].get<std::vector<std::string>>();
    for (auto& file : files) {
      std::string path = "audio/" + file + ".ogg";
      std::replace(path.begin(), path.end(), '_', '/');
      AudioMgr::AudioType type;
      switch (file[0]) {
        case 's':  // sound, snd
          type = AudioMgr::AudioType::Sound;
          break;
        case 'm':  // music, mus
          type = AudioMgr::AudioType::Music;
          break;
        default:  // voice, voi
          type = AudioMgr::AudioType::Voice;
          break;
      }
      if (AudioMgr::get()->load(file, type, path))
        SDL_Log("[I] <loadAssets> Loaded audio '%s'.", file.c_str());
    }
  }
  // 图片
  if (j.contains("image")) {
    SDL_Log("[I] <loadAssets> Loading image in %s...", json_path.c_str());
    const std::vector<std::string>& files =
        j["image"].get<std::vector<std::string>>();
    for (auto& file : files) {
      if (ImageMgr::get()->load(renderer, file, "image/" + file + ".png"))
        SDL_Log("[I] <loadAssets> Loaded image '%s'.", file.c_str());
    }
  }
  // 字体
  if (j.contains("font")) {
    SDL_Log("[I] <loadAssets> Loading font in %s...", json_path.c_str());
    const std::vector<std::string>& files =
        j["font"].get<std::vector<std::string>>();
    for (auto& file : files) {
      if (FontMgr::get()->load(renderer, file, "font/" + file + ".fnt"))
        SDL_Log("[I] <loadAssets> Loaded font '%s'.", file.c_str());
    }
  }
}