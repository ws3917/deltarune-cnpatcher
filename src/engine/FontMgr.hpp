// 资源管理器
#pragma once
#include <SDL3/SDL.h>

#include <fstream>
#include <string>
#include <unordered_map>

struct Font {
  struct Glyph {
    uint32_t id;
    uint16_t x, y, width, height;
    int16_t x_offset, y_offset;
    int16_t x_advance;
  };
  int line_height = 0;  // 基准行高
  int base = 0;         // 基准线
  std::unordered_map<uint32_t, Glyph> glyphs = {};
  SDL_Texture* atlas = nullptr;
};

class FontMgr {
 public:
  // 单例
  static FontMgr* get() {
    static FontMgr instance;
    return &instance;
  }
  FontMgr(const FontMgr&) = delete;
  FontMgr& operator=(const FontMgr&) = delete;

  // 加载资源
  bool load(SDL_Renderer* renderer, const std::string& name,
            const std::string& path);

  void draw(SDL_Renderer* renderer, const std::string& name,
            const std::string& text, float x, float y, float scale = 1.0f);
  void exit();

 private:
  // 读取str[i]位置的下一个UTF-8字符
  uint32_t getNextUTF8(const std::string& str, size_t& i);

  template <typename T>
  T read(std::ifstream& file) {
    T value;
    file.read(reinterpret_cast<char*>(&value), sizeof(T));
    return value;
  }
  FontMgr() = default;
  ~FontMgr() = default;
  std::unordered_map<std::string, Font*> font_assets;
};