#include <SDL3/SDL.h>
#include <physfs.h>

#include <string>

namespace U {
inline void* ReadFile(const std::string& filepath, size_t* file_size) {
  const std::string full_path = "assets/" + filepath;
  PHYSFS_File* file = PHYSFS_openRead(full_path.c_str());
  if (file == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to open file. PhysFS Error: %s",
                 PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return nullptr;
  }

  PHYSFS_sint64 size = PHYSFS_fileLength(file);
  if (size < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to get file size. PhysFS Error: %s",
                 PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    return nullptr;
  }
  char* buffer = (char*)SDL_malloc(size);
  PHYSFS_readBytes(file, buffer, size);
  PHYSFS_close(file);
  *file_size = size;
  return buffer;
}
inline SDL_IOStream* IOFromFile(const std::string& filepath) {
  size_t size;
  void* data = ReadFile(filepath, &size);
  if (data == nullptr) return nullptr;
  return SDL_IOFromConstMem(data, size);
}
}  // namespace U