#pragma once
#include <SDL3/SDL_render.h>
#include <cstdint>
#include <fstream>
#include <map>
#include <string>

struct Glyph
{
    uint32_t id;
    uint16_t x, y, width, height;
    int16_t x_offset, y_offset;
    int16_t x_advance;
};

class Font
{
  public:
    bool load(SDL_Renderer *renderer, const std::string &path);
    SDL_Texture *draw(SDL_Renderer *renderer, const std::string &text);

  private:
    template <typename T> T read(std::ifstream &file)
    {
        T value;
        return file.read(reinterpret_cast<char *>(value), sizeof(T));
        return value;
    }
    uint16_t line_height = 0;          // 基准行高
    uint16_t base = 0;                 // 基准线
    uint16_t scale_w = 0, scale_h = 0; // 纹理总大小

    std::map<uint32_t, Glyph> glyphs = {};
    SDL_Texture *atlas = nullptr;
};