#include "ImageMgr.hpp"

bool ImageMgr::load(SDL_Renderer* renderer, const std::string& name,
                    const std::string& path, int frame_count) {
  SDL_Surface* surface = SDL_LoadPNG(path.c_str());
  if (!surface) {
    SDL_Log("[E] <ImageMgr> Can't load image '%s': %s", path.c_str(),
            SDL_GetError());
    return false;
  }
  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
  if (!tex) {
    SDL_Log("[E] <ImageMgr> Can't load image '%s': %s", path.c_str(),
            SDL_GetError());
    SDL_DestroySurface(surface);
    return false;
  }
  SDL_DestroySurface(surface);
  image_assets[name] = {tex, frame_count};
  return true;
}
void ImageMgr::draw(SDL_Renderer* renderer, const std::string& name, float x,
                    float y, int frame_idx, float scale) {
  if (!image_assets.count(name)) {
    SDL_Log("[E] <ImageMgr> Can't find image '%s': %s", name.c_str(),
            SDL_GetError());
    return;
  }
  auto& [_, obj] = *image_assets.find(name);
  SDL_Texture* tex = obj.first;
  int frame_count = obj.second;
  if (frame_idx < 0 || frame_idx >= frame_count) {
    SDL_Log("[E] <ImageMgr> Can't draw image '%s': frame index out of range",
            name.c_str());
    return;
  }
  // 横向平铺，x坐标为 total_x * frame_idx / frame_count
  float src_w, src_h;
  SDL_GetTextureSize(tex, &src_w, &src_h);
  SDL_FRect src = {src_w * frame_idx / frame_count, 0.0f, src_w / frame_count,
                   src_h};
  SDL_FRect dst = {x, y, src_w * scale, src_h * scale};

  SDL_RenderTexture(renderer, tex, &src, &dst);
}

void ImageMgr::clear() {
  for (auto& [_, obj] : image_assets) {
    SDL_DestroyTexture(obj.first);
  }
  image_assets.clear();
}
ImageMgr::~ImageMgr() { clear(); }