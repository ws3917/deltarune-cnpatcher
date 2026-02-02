// 资源管理器
#pragma once
#include <string>
#include <unordered_map>
#include "E_Font.hpp"

class E_Font;
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
        for (auto &[name, font] : font_cache)
        {
            delete font;
        }
        sounds.clear();
        font_cache.clear();
    }

  private:
    AudioMgr() = default;
    ~AudioMgr()
    {
        clear();
    }

    std::unordered_map<std::string, SDL_Texture *> sounds;
    std::unordered_map<std::string, E_Font *> font_cache;
};