// 资源管理器
#pragma once
#include <SDL3_mixer/SDL_mixer.h>

#include <string>
#include <unordered_map>

class AudioMgr {
 public:
  AudioMgr(MIX_Mixer* mixer);
  ~AudioMgr();
  bool init();
  bool load(const std::string& name, const std::string& type,
            const std::string& path);
  bool loads(const std::string& config_path);
  void play(const std::string& name);
  void stop(const std::string& name);

 private:
  struct TrackProperty {
    MIX_Track* track = nullptr;
    SDL_PropertiesID options = 0;
  };
  struct AudioProperty {
    MIX_Audio* audio;
    TrackProperty* track;
  };
  MIX_Mixer* mixer = nullptr;
  std::unordered_map<std::string, AudioProperty> audio_assets = {};
  std::unordered_map<std::string, TrackProperty> tracks = {
      {"music", {}}, {"sound", {}}, {"voice", {}}};
};