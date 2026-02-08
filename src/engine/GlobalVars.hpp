// Global.hpp
#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

class Scene;
class ImageMgr;
class AudioMgr;
class FontMgr;
class GameMgr;
class SceneMgr;

namespace G {
inline SDL_Renderer* renderer = nullptr;
inline SDL_Window* window = nullptr;
inline Scene* current_scene = nullptr;

inline SDL_AppResult status = SDL_APP_CONTINUE;

inline GameMgr* game = nullptr;
inline ImageMgr* image = nullptr;
inline AudioMgr* audio = nullptr;
inline FontMgr* font = nullptr;
inline SceneMgr* scene = nullptr;

inline uint64_t last_tick = 0;

inline constexpr int WIDTH = 640;
inline constexpr int HEIGHT = 480;
inline constexpr const char* NAME = "Project Spacetime 0";
}  // namespace G