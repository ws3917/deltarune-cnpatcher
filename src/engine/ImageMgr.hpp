// 资源管理器
#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>

class AudioMgr
{
  public:
    // 单例
    static AudioMgr *get()
    {
        static AudioMgr instance;
        return &instance;
    }
    AudioMgr(const AudioMgr &) = delete;
    AudioMgr &operator=(const AudioMgr &) = delete;

    // 加载资源
    template <typename T> T *load(SDL_Renderer *renderer, const std::string &name, const std::string &path);
    template <typename T> T *get(const std::string &name);

    void clear()
    {
        for (auto &[name, tex] : sounds)
        {
            SDL_DestroyTexture(tex);
        }
        sounds.clear();
    }

  private:
    AudioMgr() = default;
    ~AudioMgr()
    {
        clear();
    }

    std::unordered_map<std::string, SDL_Texture *> sounds;
};