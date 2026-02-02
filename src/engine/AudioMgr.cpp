#include "AudioMgr.hpp"
#include "Game.hpp"
#include "SDL3/SDL_log.h"

AudioMgr::AudioMgr()
{
    ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS)
    {
        SDL_Log("[C] <AudioMgr> Failed to initialize audio engine, error code = %d", result);
        Game::get()->abort(true);
    }
}

AudioMgr::~AudioMgr()
{
    clear();
    ma_engine_uninit(&engine);
}

void AudioMgr::clear()
{
    for (auto &[_, sound] : sounds)
    {
        ma_sound_stop(&sound);
        ma_sound_uninit(&sound);
    }
    sounds.clear();
}

ma_sound *AudioMgr::find(const std::string &name)
{
    auto it = sounds.find(name);
    if (it == sounds.end())
    {
        SDL_Log("[E] <AudioMgr> Can't find sound: %s", name.c_str());
        return nullptr;
    }
    return &it->second;
}

void AudioMgr::play(const std::string &name, bool loop, bool resume)
{
    auto sound = find(name);
    ma_sound_set_looping(sound, loop);
    if (!resume)
        ma_sound_seek_to_pcm_frame(sound, 0);
    ma_sound_start(sound);
}

void AudioMgr::stop(const std::string &name)
{
    auto sound = find(name);
    ma_sound_stop(sound);
}

bool AudioMgr::load(AudioType type, const std::string &name, const std::string &path)
{
    if (sounds.count(name))
        return true;

    ma_sound_flags flags = type == AudioType::Music ? MA_SOUND_FLAG_STREAM : MA_SOUND_FLAG_DECODE;

    ma_sound sound{};
    ma_result res = ma_sound_init_from_file(&engine, path.c_str(), flags, nullptr, nullptr, &sound);
    if (res != MA_SUCCESS)
    {
        SDL_Log("[E] <AudioMgr> Can't load sound: %s", name.c_str());
        return false;
    }
    sounds.emplace(name, std::move(sound));
    return true;
}