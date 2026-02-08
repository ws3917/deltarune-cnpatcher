// 资源管理器
#pragma once
#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>

class ImageMgr {
 public:
  bool load(SDL_Renderer* renderer, const std::string& name,
            const std::string& path, int frame_count = 1);

  void draw(SDL_Renderer* renderer, const std::string& name, float x, float y,
            int frame_idx = 0, float scale = 1.0f);
  ImageMgr() = default;
  ~ImageMgr();

 private:
  std::unordered_map<std::string, std::pair<SDL_Texture*, int>> image_assets;
};
