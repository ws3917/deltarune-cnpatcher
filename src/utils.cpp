#include "utils.hpp"

#include <SDL3/SDL.h>
#include <physfs.h>

#include <nlohmann/json.hpp>

SDL_IOStream* U::IOFromFile(const std::string& filepath) {
  const std::string full_path = "assets/" + filepath;
  PHYSFS_File* file = PHYSFS_openRead(full_path.c_str());
  if (!file) return nullptr;

  PHYSFS_sint64 size = PHYSFS_fileLength(file);
  void* buffer = SDL_malloc(size);
  if (!buffer) {
    PHYSFS_close(file);
    return nullptr;
  }

  if (PHYSFS_readBytes(file, buffer, size) < size) {
    SDL_free(buffer);
    PHYSFS_close(file);
    return nullptr;
  }
  PHYSFS_close(file);
  return SDL_IOFromMem(buffer, size);
}
nlohmann::json U::LoadJson(const std::string& filepath) {
  SDL_IOStream* io = U::IOFromFile(filepath);
  if (!io) {
    return nlohmann::json::object();  // 返回空对象而非崩溃
  }
  Sint64 size = SDL_GetIOSize(io);
  std::vector<uint8_t> buffer(static_cast<size_t>(size));
  SDL_ReadIO(io, buffer.data(), buffer.size());
  try {
    nlohmann::json j = nlohmann::json::parse(buffer);
    SDL_CloseIO(io);
    return j;
  } catch (const nlohmann::json::parse_error& e) {
    SDL_CloseIO(io);
    return nlohmann::json::object();
  }
}