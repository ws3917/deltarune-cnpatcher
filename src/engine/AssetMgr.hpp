#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "PhysFSSteam.hpp"

class AssetMgr {
 public:
  static const sf::Texture& loadTexture(const std::string& filename);
  static const sf::SoundBuffer& loadSound(const std::string& filename);
  static sf::Music& loadMusic(const std::string& filename);

 private:
  struct MusicData {
    MusicData(const std::string filename) : stream(filename) {}
    sf::Music music;
    PhysFSStream stream;
  };
  static std::vector<std::byte> loadData(const std::string& filename);
};
using am = AssetMgr;