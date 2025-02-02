#ifndef APP_H
#define APP_H

#define WINDOW_SCALE 1.0f

// for the love of god make it a multiple of the brick width if not, well everything will not break but some stuff will be
// fricked for now (level_updateCollisionCheck)
#define WINDOW_WIDTH 780.0f 
#define WINDOW_HEIGHT 800.0f
#define FPS 60.0f
#define TARGET_DELAY 1000.0f / FPS 

#include <SDL2/SDL.h>
#include <stdbool.h>
typedef struct App {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event e;
    float deltaTime;
    bool isRunning;
    bool shouldCapFrameRate;
    unsigned int startFrame;
    unsigned int endFrame;
} App;

bool app_Init(App* app);
void app_Free(App* app);
void app_SetDeltaTime(App* app);
void app_CapFrameRate(App* app);
void app_printFrameRate(App* app);

#endif