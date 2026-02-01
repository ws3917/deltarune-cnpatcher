#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

// 定义窗口大小
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int test()
{
    // 1. 初始化 SDL 核心 (视频 + 音频)
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("SDL_Init Failed: %s", SDL_GetError());
        return -1;
    }
    // 2. 创建窗口和渲染器
    SDL_Window *window = SDL_CreateWindow("SDL3 Full Stack Test", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (!window || !renderer)
    {
        SDL_Log("Window/Renderer Creation Failed: %s", SDL_GetError());
        return -1;
    }

    // ==========================================
    // 测试 1: SDL3_image 加载 PNG
    // ==========================================
    // 虽然 SDL3 Core 支持 BMP，但 PNG 需要 IMG_LoadTexture
    SDL_Texture *pngTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadPNG("test.png"));
    if (!pngTexture)
    {
        SDL_Log("Failed to load test.png: %s", SDL_GetError());
        // 为了演示不退出，但实际开发中应处理错误
    }
    else
    {
        SDL_Log("Success: Loaded test.png");
    }

    // ==========================================
    // 测试 3: SDL3_mixer 加载 OGG 音乐
    // ==========================================
    // SDL3 Mixer 初始化音频设备
    MIX_Init();
    SDL_AudioSpec spec;
    spec.freq = 44100;
    spec.format = SDL_AUDIO_F32;
    spec.channels = 2;
    MIX_Mixer *mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

    if (!mixer)
    { // 0 使用默认设备 ID
        SDL_Log("Mix_OpenAudio Failed: %s", SDL_GetError());
    }
    else
    {
        MIX_Audio *music = MIX_LoadAudio(mixer, "test.ogg", true);
        MIX_Track *track = MIX_CreateTrack(mixer);
        if (!music)
        {
            SDL_Log("Failed to load test.ogg: %s", SDL_GetError());
        }
        else
        {
            SDL_Log("Success: Loaded test.ogg");
            MIX_SetTrackAudio(track, music);
            MIX_PlayTrack(track, 0);
        }
    }

    // ==========================================
    // 主循环
    // ==========================================
    bool running = true;
    SDL_Event event;

    // 目标矩形：图片居中，文字在图片下方
    SDL_FRect pngRect = {(float)(WINDOW_WIDTH - 200) / 2, (float)(WINDOW_HEIGHT - 200) / 2, 200.0f, 200.0f};

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // 渲染背景 (深灰色)
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // 渲染 PNG
        if (pngTexture)
        {
            SDL_RenderTexture(renderer, pngTexture, NULL, &pngRect);
        }

        SDL_RenderPresent(renderer);
    }

    // ==========================================
    // 清理资源
    // ==========================================
    if (pngTexture)
        SDL_DestroyTexture(pngTexture);


    MIX_DestroyMixer(mixer);
    MIX_Quit();
    SDL_Quit();

    return 0;
}