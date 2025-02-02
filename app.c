#include "app.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
bool app_Init(App* app) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return false;

    app->window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);

    if (!app->window)
        return false;

    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (!app->renderer)
        return false;

    SDL_RenderSetScale(app->renderer, WINDOW_SCALE, WINDOW_SCALE);
    SDL_DisplayMode displayMode;
    SDL_GetWindowDisplayMode(app->window, &displayMode);

    app->shouldCapFrameRate = displayMode.refresh_rate > FPS;
    app->isRunning = true;
    app->deltaTime = 0;
    return true;
}

void app_Free(App* app) {
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
}

void app_SetDeltaTime(App* app) {
    app->startFrame = SDL_GetTicks();
    app->deltaTime = (app->startFrame - app->endFrame) / 1000.0f;
}

void app_CapFrameRate(App* app) {
    unsigned int frameTime = SDL_GetTicks() - app->startFrame;

    if (TARGET_DELAY > frameTime) {
        SDL_Delay(TARGET_DELAY - frameTime);
    }
}

void app_printFrameRate(App* app){
    printf("%f\n", 1000.0f / (SDL_GetTicks() - app->startFrame));
}