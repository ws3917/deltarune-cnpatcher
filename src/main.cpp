#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <physfs.h>

#include <nlohmann/json.hpp>

#include "save.hpp"
#include "utils.hpp"
#include "values.hpp"

int main(int, char** argv) {
  // 读取一张图片并显示
  if (!SDL_Init(SDL_INIT_VIDEO)) return 1;
  if (!MIX_Init()) return 1;
  bool data_mount_success = false, save_mount_success = false;
#ifdef SDL_PLATFORM_ANDROID
  if (!PHYSFS_init(reinterpret_cast<char*>(new PHYSFS_AndroidInit{
          SDL_GetAndroidJNIEnv(), SDL_GetAndroidActivity()})))
    return 1;
  mount_success = PHYSFS_mount(PHYSFS_getBaseDir(), "/", 0);
  save_mount_success =
      PHYSFS_mount(SDL_GetAndroidExternalStoragePath(), "/user", 1);
  PHYSFS_setWriteDir(SDL_GetAndroidExternalStoragePath());
#else
  if (!PHYSFS_init(argv[0])) return 1;
  char data_path[1024];
  SDL_snprintf(data_path, sizeof(data_path), "%smain.pak", PHYSFS_getBaseDir());
  data_mount_success = PHYSFS_mount(data_path, "/assets", 0);
  save_mount_success = PHYSFS_mount(
      SDL_GetPrefPath(V::GAME_AUTHOR, V::GAME_SHORTNAME), "/user", 1);
  PHYSFS_setWriteDir(SDL_GetPrefPath(V::GAME_AUTHOR, V::GAME_SHORTNAME));
#endif
  if (!data_mount_success || !save_mount_success) {
    return 1;
  }
  auto j = U::LoadJson("text/zh_CN.json");
  SDL_Log("hello, %s", j["myname"].get<std::string>().c_str());
  Save save;
  SDL_CreateWindowAndRenderer(
      "hello", static_cast<int>(V::WINDOW_SCALE * V::RENDER_WIDTH),
      static_cast<int>(V::WINDOW_SCALE * V::RENDER_HEIGHT),
      SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE, &save.meta.window,
      &save.meta.renderer);
  SDL_SetRenderVSync(save.meta.renderer, 1);
  SDL_SetRenderLogicalPresentation(save.meta.renderer, V::RENDER_WIDTH,
                                   V::RENDER_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  SDL_Event event;
  SDL_Surface* img = SDL_LoadPNG_IO(U::IOFromFile("image/bg_static.png"), true);
  SDL_Texture* tex = SDL_CreateTextureFromSurface(save.meta.renderer, img);
  SDL_DestroySurface(img);
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) running = false;
      SDL_ConvertEventToRenderCoordinates(save.meta.renderer, &event);
    }
    SDL_RenderClear(save.meta.renderer);
    SDL_RenderTexture(save.meta.renderer, tex, nullptr, nullptr);
    SDL_RenderPresent(save.meta.renderer);
  }
  PHYSFS_deinit();
  MIX_Quit();
  SDL_Quit();
  return 0;
}