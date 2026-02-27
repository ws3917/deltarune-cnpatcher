#include "AssetMgr.hpp"

#include <physfs.h>

std::vector<std::byte> loadData(const std::string& filename) {
  if (!PHYSFS_isInit()) return {};
  PHYSFS_File* file = PHYSFS_openRead(filename.c_str());
  if (!file) return {};
  size_t size = PHYSFS_fileLength(file);
  if (size < 0) {
    PHYSFS_close(file);
    return {};
  }
  std::vector<std::byte> buffer = {};
  buffer.reserve(size);
  size_t read_size = PHYSFS_readBytes(file, buffer.data(), size);
  if (size != read_size)
  {
    
    return {};
  }
}
sf::Texture am::loadTexture() {}