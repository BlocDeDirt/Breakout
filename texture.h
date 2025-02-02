#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL2/SDL.h>

typedef struct Dimension{
    int w;
    int h;
} Dimension;

/**
 * Returns a pointer to a texture, the caller is the owner
 */
SDL_Texture* texture_Load(const char* pathToTexture);

Dimension getTextureDimension(SDL_Texture *texture);
#endif