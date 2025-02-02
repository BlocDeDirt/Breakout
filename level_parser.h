#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H
#include "texture.h"
#include "brick.h"

typedef struct Level {
    int length;
    Brick** bricks;
} Level;

Level* level_ParseFile(const Dimension* brickDimension, int windowWidth, int windowHeight, SDL_Texture* brickTexture);

/**
 * Ok we loaded a level, but let's not check every god damn brick for collision. If a brick is surrounded by 4 others bricks or neat a wall 
 * mark it so we know to not check for collision (yet..)
 */
void level_updateCollisionCheck(Level* level, const Dimension* brickDimension);
void level_Free(Level* level);
#endif