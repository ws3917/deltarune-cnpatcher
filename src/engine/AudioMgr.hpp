// 资源管理器
#pragma once
#include <SDL3_mixer/SDL_mixer.h>

#include <array>
#include <string>
#include <unordered_map>

class AudioMgr {
 public:
  enum class AudioType { Music, Sound, Voice };
  // 单例
  static AudioMgr* get() {
    static AudioMgr instance;
    return &instance;
  }
  AudioMgr(const AudioMgr&) = delete;
  AudioMgr& operator=(const AudioMgr&) = delete;

  bool init();

  // 加载资源
  bool load(AudioType type, const std::string& name, const std::string& path);

  // loop = true 代表循环播放，resume = true代表继续播放
  void play(const std::string& name);
  void stop(const std::string& name);
  void clear();

 private:
  AudioMgr() = default;
  ~AudioMgr();

  MIX_Mixer* mixer = nullptr;
  std::unordered_map<std::string, std::pair<MIX_Audio*, AudioType>> audio_assets = {};
  std::array<MIX_Track*, 3> tracks = {};
};