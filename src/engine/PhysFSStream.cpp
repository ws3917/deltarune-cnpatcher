#include "PhysFSSteam.hpp"
PhysFSStream::PhysFSStream(const std::string& filename) {
  file = PHYSFS_openRead(filename.c_str());
}
PhysFSStream::~PhysFSStream() {
  if (file) PHYSFS_close(file);
}
// 后面的几个函数抄模板
std::optional<size_t> PhysFSStream::read(void* data, size_t size) {
  if (!file) return std::nullopt;
  PHYSFS_sint64 res = PHYSFS_readBytes(file, data, size);
  if (res < 0) return std::nullopt;
  return static_cast<size_t>(res);
}

std::optional<size_t> PhysFSStream::seek(size_t position) {
  if (!file) return std::nullopt;
  if (PHYSFS_seek(file, position) == 0) return std::nullopt;
  return tell();
}

std::optional<size_t> PhysFSStream::tell() {
  if (!file) return std::nullopt;
  PHYSFS_sint64 res = PHYSFS_tell(file);
  return res < 0 ? std::nullopt : std::optional<size_t>(res);
}

std::optional<size_t> PhysFSStream::getSize() {
  if (!file) return std::nullopt;
  PHYSFS_sint64 res = PHYSFS_fileLength(file);
  return res < 0 ? std::nullopt : std::optional<size_t>(res);
}