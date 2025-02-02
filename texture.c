#include "texture.h"
#include "app.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// https://gamedev.stackexchange.com/questions/131812/how-to-render-a-png-file-on-the-screen-with-sdl
// https://wiki.libsdl.org/SDL2/SDL_SetTextureColorMod
// https://wiki.libsdl.org/SDL2/SDL_RenderCopyEx
extern App app;
SDL_Texture* texture_Load(const char* pathToTexture) {
    SDL_Surface* surface = IMG_Load(pathToTexture);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    SDL_FreeSurface(surface);
    surface = NULL;

    if(texture == NULL){
        printf("Failed to load texture\n");
        exit(EXIT_FAILURE);
    }
    return texture;
}

Dimension getTextureDimension(SDL_Texture *texture) {
    Dimension dimension;
    SDL_QueryTexture(texture, NULL, NULL, &dimension.w, &dimension.h);
    return dimension;
}