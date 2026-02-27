#include "AssetMgr.hpp"

#include <physfs.h>

std::vector<std::byte> am::loadData(const std::string& filename) {
  if (!PHYSFS_isInit()) return {};
  PHYSFS_File* file = PHYSFS_openRead(filename.c_str());
  if (!file) {
    sf::err() << std::format("[E] Can't read file '{}'\n", filename);
    return {};
  }
  size_t size = PHYSFS_fileLength(file);
  if (size < 0) {
    sf::err() << std::format("[E] Can't read file '{}'\n", filename);
    PHYSFS_close(file);
    return {};
  }
  std::vector<std::byte> buffer(size);
  size_t read_size = PHYSFS_readBytes(file, buffer.data(), size);
  if (size != read_size) {
    sf::err() << std::format(
        "[E] File '{}' read size mismatch (interrupted?)\n", filename);
    PHYSFS_close(file);
    return {};
  }
  PHYSFS_close(file);
  return buffer;
}
const sf::Texture& am::loadTexture(const std::string& filename) {
  static std::unordered_map<std::string, sf::Texture> pool = {};
  auto [it, newfile] = pool.try_emplace(filename);
  // 如果缓存池命中就不会执行这部分
  if (newfile) {
    auto data = loadData("image/" + filename + ".png");
    // 没找到
    if (data.empty() || it->second.loadFromMemory(data.data(), data.size()))
      sf::err() << std::format("[AM] Failed to load texture {}", filename);
  }
  return it->second;
}
const sf::SoundBuffer& am::loadSound(const std::string& filename) {
  static std::unordered_map<std::string, sf::SoundBuffer> pool = {};
  // 和loadTexture基本一样
  auto [it, newfile] = pool.try_emplace(filename);
  if (newfile) {
    auto data = loadData("sound/" + filename + ".mp3");
    if (data.empty() || it->second.loadFromMemory(data.data(), data.size()))
      sf::err() << std::format("[AM] Failed to load sound {}", filename);
  }
  return it->second;
}

// [sf::Music, PhysFSStream]
sf::Music& am::loadMusic(const std::string& filename) {
  static std::unordered_map<std::string, am::MusicData> pool = {};
  auto [it, newfile] = pool.try_emplace(filename, "music/" + filename + ".mp3");
  auto& [music, stream] = it->second;
  if (newfile) {
    if (!music.openFromStream(stream))
      sf::err() << std::format("[AM] Failed to load sound {}", filename);
  }
  return music;
}