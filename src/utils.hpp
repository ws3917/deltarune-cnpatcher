#pragma once
#include <nlohmann/json_fwd.hpp>
#include <SDL3/SDL_iostream.h>
#include <string>
namespace U {
SDL_IOStream* IOFromFile(const std::string& filepath);
nlohmann::json LoadJson(const std::string& filepath);
}  // namespace U