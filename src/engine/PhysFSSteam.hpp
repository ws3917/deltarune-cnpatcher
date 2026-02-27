#pragma once
#include <physfs.h>

#include <SFML/Audio.hpp>

class PhysFSStream : public sf::InputStream {
 public:
  PhysFSStream(const std::string& filename) {
    file = PHYSFS_openRead(filename.c_str());
  }
  ~PhysFSStream() {
    if (file) PHYSFS_close(file);
  }
  // 后面的几个函数抄模板
  std::optional<size_t> read(void* data, size_t size) override {
    return PHYSFS_readBytes(file, data, size);
  }
  std::optional<size_t> seek(size_t position) override {
    return PHYSFS_seek(file, position) ? PHYSFS_tell(file) : -1;
  }
  std::optional<size_t> tell() override { return PHYSFS_tell(file); }
  std::optional<size_t> getSize() override { return PHYSFS_fileLength(file); }

 private:
  PHYSFS_File* file = nullptr;
};