#include "Font.h"
#include <SDL3/SDL_log.h>
#include <fstream>
#include <string>
bool Font::load(const std::string& path)
{
    std::ifstream fnt_file(path, std::ios::binary);
    if (!fnt_file.is_open())
    {
        SDL_Log("[C] <LoadFont> fnt file not found: %s", path.c_str());
        return false;
    }
    char header[5];
    fnt_file.read(header, 4);
    if (std::string(header) != "BMF\x03")
    {
        SDL_Log("[C] <LoadFont> Invalid fnt Header: %s", header);
    }
    return true;
}