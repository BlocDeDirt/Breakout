#ifndef CLOUD_H
#define CLOUD_H
#include "vector.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct Cloud {
    SDL_Texture* texture;
    Vector2* position;
    SDL_Rect* rect;
    int speed;
    unsigned int timeoutDuration;
    unsigned int startTimeout;
    SDL_RendererFlip flip;
} Cloud;

typedef struct CloudBackground {
    SDL_Texture** textures;
    Cloud** clouds;
    int cloudLength;
    int textureLength;
    int minYLevel;
    int maxYLevel; // use those 2 properties to have a range for the clouds
} CloudBackground;

CloudBackground* cloudbackground_Construct();
void cloudbackground_Free(CloudBackground* bg);
void cloudbackground_Draw(CloudBackground* bg);

Cloud* cloud_Construct(SDL_Texture* texture, Vector2* position, int size, int speed);
void cloud_Draw(Cloud* cloud);
void cloud_Free(Cloud* cloud);
#endif