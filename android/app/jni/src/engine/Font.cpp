#include "Font.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <cstdint>
#include <fstream>
#include <string>
bool Font::load(SDL_Renderer *renderer, const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        SDL_Log("[C] <LoadFont> Fnt file not found: %s", path.c_str());
        return false;
    }

    // 加载字图
    std::string glyph_path = path;
    size_t pos = glyph_path.find(".fnt");
    if (pos != std::string::npos)
    {
        glyph_path.replace(pos, 4, "_0.png");
    }
    SDL_Surface *glyph_file = SDL_LoadBMP(glyph_path.c_str());
    if (!glyph_file)
    {
        SDL_Log("[C] <LoadFont> Can't open glyph file %s: %s", glyph_path.c_str(), SDL_GetError());
        return false;
    }
    atlas = SDL_CreateTextureFromSurface(renderer, glyph_file);
    SDL_DestroySurface(glyph_file);

    // 文件头
    char header[5];
    file.read(header, 4);
    if (std::string(header) != "BMF\x03")
    {
        SDL_Log("[C] <LoadFont> Invalid fnt Header: %s", header);
        return false;
    }

    // 读物内容
    while (file.peek() != EOF)
    {
        uint8_t block_type = read<uint8_t>(file);
        uint32_t block_size = read<uint32_t>(file);
        int num_chars = block_size / 20;
        std::streampos next_block_pos = file.tellg() + std::streamoff(block_size);

        switch (block_type)
        {
        case 2:
            line_height = read<uint16_t>(file);
            base = read<uint16_t>(file);
            scale_w = read<uint16_t>(file);
            scale_h = read<uint16_t>(file);
            read<uint16_t>(file);
            read<uint8_t>(file);
            read<uint8_t>(file);
            read<uint8_t>(file);
            read<uint8_t>(file);
            read<uint8_t>(file);
            break;
        case 4:
            for (int i = 0; i < num_chars; i++)
            {
                Glyph g;
                g.id = read<uint32_t>(file);
                g.x = read<uint16_t>(file);
                g.y = read<uint16_t>(file);
                g.width = read<uint16_t>(file);
                g.height = read<uint16_t>(file);
                g.x_offset = read<int16_t>(file);
                g.y_offset = read<int16_t>(file);
                g.x_advance = read<int16_t>(file);
                read<uint8_t>(file);
                read<uint8_t>(file);
                glyphs[g.id] = g;
            }
            break;
        default:
            break;
        }
        file.seekg(next_block_pos);
    }

    return true;
}