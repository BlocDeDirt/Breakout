#ifndef BRICK_H
#define BRICK_H
#define BRICK_WIDTH 60
#include "collider.h"
#include "vector.h"
#include "texture.h"
#include <SDL2/SDL.h>
#include "color.h"
#include <stdbool.h>

typedef struct Brick {
    SDL_Texture* texture;
    SDL_Rect* rect;
    AABBCollider* collider;
    Vector2* position;
    enum EnumColor color;
    bool checkCollision;
    bool shouldRender;
} Brick;

Brick* brick_Construct(SDL_Texture* texture, Vector2* position, const Dimension* brickDimension, enum EnumColor color);

/**
 * Will return the width & height of the brick based on the aspect ratio of the texture
 */
Dimension brick_getDimension(int brickWidth, SDL_Texture* texture);

/**
 * When a brick break, this function will check if the broken brick has neighbor to tell them to check for collision
 */
void brick_UpdateCollisionCheck(Brick** bricks, Dimension* brickDimension, int length, int indexBrickDestroyed);
void brick_Free(Brick* brick);
void brick_Draw(Brick* brick);

#endif