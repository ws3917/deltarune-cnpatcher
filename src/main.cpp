// Deltarune CN Patcher - 三角符文汉化安装器 (SDL3重构)
// ws3917, 2026

#include "scene/SInit.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <string>
#include <unordered_map>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct AppState
{
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    uint64_t last_tick = 0;
    Scene *current_scene = nullptr;

    // 图像：帧数
    std::unordered_map<std::string, std::pair<SDL_Texture *, int>> images = {};
    std::unordered_map<std::string, Scene *> scenes = {};
};

SDL_AppResult SDL_AppInit(void **appstate, int, char **)
{
    AppState *state = new AppState;
    SDL_Log("-- Log prefix meaning --\n[C] = Critical, [E] = Error, [W] = Warning, [I] = Info");
    constexpr int APP_WIDTH = 960, APP_HEIGHT = 720;
    if (!SDL_CreateWindowAndRenderer("Deltarune CN Patcher", APP_WIDTH, APP_HEIGHT, SDL_WINDOW_HIGH_PIXEL_DENSITY,
                                     &state->window, &state->renderer))
    {
        SDL_Log("[C] <Init> Creating Window failed: %s", SDL_GetError());
        delete state;
        return SDL_APP_FAILURE;
    }

    SDL_Surface *icon = SDL_LoadPNG("image/icon.png");
    if (icon)
    {
        SDL_SetWindowIcon(state->window, icon);
        SDL_DestroySurface(icon);
    }
    SDL_SetRenderVSync(state->renderer, 1);
    SDL_SetDefaultTextureScaleMode(state->renderer, SDL_SCALEMODE_PIXELART);
    SDL_SetRenderLogicalPresentation(state->renderer, APP_WIDTH, APP_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    float scale = SDL_GetWindowDisplayScale(state->window);
    SDL_SetWindowSize(state->window, static_cast<int>(scale * APP_WIDTH), static_cast<int>(scale * APP_HEIGHT));

    state->current_scene = new SInit();

    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = static_cast<AppState *>(appstate);
    const uint64_t now = SDL_GetTicks();

    if (state->last_tick == 0)
        state->last_tick = now;

    uint64_t delta = now - state->last_tick;
    state->last_tick = now;

    state->current_scene->update(delta);

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state->renderer);

    state->current_scene->draw(state->renderer);

    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *state = static_cast<AppState *>(appstate);
    SDL_ConvertEventToRenderCoordinates(static_cast<AppState *>(appstate)->renderer, event);
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;
    state->current_scene->input(event);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult)
{
    AppState *state = static_cast<AppState *>(appstate);
    if (state->renderer)
        SDL_DestroyRenderer(state->renderer);
    if (state->window)
        SDL_DestroyWindow(state->window);

    delete state;
}