#include "GameMgr.hpp"

#include <algorithm>
#include <string>
#include <toml++/toml.hpp>

#include "AudioMgr.hpp"
#include "FontMgr.hpp"
#include "GlobalVars.hpp"
#include "ImageMgr.hpp"
#include "Scene.hpp"
#include "SceneMgr.hpp"

GameMgr::GameMgr() {
  G::image = new ImageMgr;
  G::font = new FontMgr;
  G::audio = new AudioMgr;
  G::scene = new SceneMgr;
  SDL_Log("[I] <GameMgr> Start loading assets");
  loadAssets("assets.toml");
  G::scene->changeScene("init");
}
void GameMgr::input(SDL_Event* event) {
  // global input event...
  G::current_scene->input(event);
}
void GameMgr::draw() {
  SDL_SetRenderDrawColor(G::renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(G::renderer);
  G::current_scene->draw(G::renderer);
  SDL_RenderPresent(G::renderer);
}
void GameMgr::update(float delta) { G::current_scene->update(delta); }
GameMgr::~GameMgr() {
  delete G::scene;
  delete G::audio;
  delete G::font;
  delete G::image;
  if (G::renderer) {
    SDL_DestroyRenderer(G::renderer);
    G::renderer = nullptr;
  }
  if (G::window) {
    SDL_DestroyWindow(G::window);
    G::window = nullptr;
  }
}
void GameMgr::loadAssets(const std::string& config_path) {
  size_t size;
  void* config_data = SDL_LoadFile(config_path.c_str(), &size);
  if (!config_data) {
    SDL_Log("[E] <loadAssets> Can't open toml file '%s': %s",
            config_path.c_str(), SDL_GetError());
    return;
  }
  toml::parse_result config;
  try {
    config = toml::parse((char*)config_data);
    SDL_free(config_data);
  } catch (const toml::parse_error& e) {
    SDL_Log("[E] <loadAssets> Can't parse toml file: %s", config_path.c_str());
    return;
  }
  // 音频
  if (auto* files = config["audio"]["files"].as_array()) {
    SDL_Log("[I] <loadAssets> Loading audio in %s...", config_path.c_str());
    files->for_each([&](auto&& el) {
      std::string file = el.value_or("");
      if (file.empty()) return;
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
      if (G::audio->load(file, type, path))
        SDL_Log("[I] <loadAssets> Loaded audio '%s'.", file.c_str());
    });
  }
  // 图片
  if (auto* files = config["image"]["files"].as_array()) {
    SDL_Log("[I] <loadAssets> Loading image in %s...", config_path.c_str());
    files->for_each([&](auto&& el) {
      std::string file = el.value_or("");
      if (file.empty()) return;
      if (G::image->load(G::renderer, file, "image/" + file + ".png"))
        SDL_Log("[I] <loadAssets> Loaded image '%s'.", file.c_str());
    });
  }
  // 字体
  if (auto* files = config["font"]["files"].as_array()) {
    SDL_Log("[I] <loadAssets> Loading font in %s...", config_path.c_str());
    files->for_each([&](auto&& el) {
      std::string file = el.value_or("");
      if (file.empty()) return;
      if (G::font->load(G::renderer, file, "font/" + file + ".fnt"))
        SDL_Log("[I] <loadAssets> Loaded font '%s'.", file.c_str());
    });
  }
}