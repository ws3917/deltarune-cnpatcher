#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class AssetMgr {
 public:
  static sf::Texture loadTexture();
  static sf::Sound loadSound();

 private:
  static std::vector<std::byte> loadData(const std::string& filename);
};
using am = AssetMgr;