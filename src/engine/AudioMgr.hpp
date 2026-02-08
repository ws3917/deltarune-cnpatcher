// 资源管理器
#pragma once
#include <SDL3_mixer/SDL_mixer.h>

#include <array>
#include <string>
#include <unordered_map>

class AudioMgr {
 public:
  AudioMgr();
  ~AudioMgr();
  enum class AudioType { Music, Sound, Voice };
  bool load(const std::string& name, AudioType type, const std::string& path);
  void play(const std::string& name);
  void stop(const std::string& name);

 private:
  MIX_Mixer* mixer = nullptr;
  std::unordered_map<std::string, std::pair<MIX_Audio*, AudioType>>
      audio_assets = {};
  std::array<std::pair<MIX_Track*, SDL_PropertiesID>, 3> tracks = {};
};