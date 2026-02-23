#include <SDL3/SDL.h>
#include <physfs.h>

namespace Utils {
inline void* LoadToMem(const char* filepath, size_t* datasize) {
#ifndef __ANDROID
  if (PHYSFS_isInit()) {
    PHYSFS_File* file = PHYSFS_openRead(filepath);
    if (file) {
      PHYSFS_sint64 size = PHYSFS_fileLength(file);
      void* buffer = SDL_malloc(size + 1);

      if (buffer) {
        if (PHYSFS_readBytes(file, buffer, size) == size) {
          static_cast<char*>(buffer)[size] = '\0';
          if (datasize) *datasize = (size_t)size;
          PHYSFS_close(file);
          return buffer;
        }
        // 读取失败
        SDL_free(buffer);
      }
      PHYSFS_close(file);
    }
  }

  // fallback
  SDL_Log(
      "[W] <LoadAsset> Can't Load %s to memory. Fallback to load from disk.",
      filepath);
#endif
  return SDL_LoadFile(filepath, datasize);
}
};  // namespace Utils