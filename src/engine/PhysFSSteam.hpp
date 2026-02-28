#pragma once
#include <physfs.h>

#include <SFML/Audio.hpp>

class PhysFSStream : public sf::InputStream {
 public:
  PhysFSStream(const std::string& filename);
  ~PhysFSStream();
  // 后面的几个函数抄模板
  std::optional<size_t> read(void* data, size_t size) override;
  std::optional<size_t> seek(size_t position) override;
  std::optional<size_t> tell() override;
  std::optional<size_t> getSize() override;

 private:
  PHYSFS_File* file = nullptr;
};