// 资源管理器
#pragma once
#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>

class ImageMgr {
 public:
  // 单例
  static ImageMgr* get() {
    static ImageMgr instance;
    return &instance;
  }
  ImageMgr(const ImageMgr&) = delete;
  ImageMgr& operator=(const ImageMgr&) = delete;

  // 加载资源
  bool load(SDL_Renderer* renderer, const std::string& name,
            const std::string& path, int frame_count = 1);

  void draw(SDL_Renderer* renderer, const std::string& name, float x, float y,
            int frame_idx = 0, float scale = 1.0f);
  void exit();

 private:
  ImageMgr() = default;
  ~ImageMgr() = default;
  std::unordered_map<std::string, std::pair<SDL_Texture*, int>> image_assets;
};
