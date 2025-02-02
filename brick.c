#include "brick.h"
#include "app.h"
#include "ball.h"
#include "texture.h"
#include "color.h"
#include <SDL2/SDL.h>

extern App app;
extern Color AVAILABLE_COLORS[];

Brick* brick_Construct(SDL_Texture* texture, Vector2* position, const Dimension* brickDimension, enum EnumColor color) {
    Dimension dimension = getTextureDimension(texture);

    Brick* brick = malloc(sizeof(Brick));
    brick->texture = texture;
    brick->position = position;

    brick->collider = malloc(sizeof(AABBCollider));
    brick->rect = malloc(sizeof(SDL_Rect));

    brick->collider->x = position->x;
    brick->collider->y = position->y;
    brick->collider->w = brickDimension->w;
    brick->collider->h = brickDimension->h;

    brick->rect->x = SDL_round(position->x);
    brick->rect->y = SDL_round(position->y);
    brick->rect->w = brickDimension->w;
    brick->rect->h = brickDimension->h;
    brick->color = color;
    brick->checkCollision = true;
    brick->shouldRender = true;

    return brick;
}

Dimension brick_getDimension(int brickWidth, SDL_Texture* texture) {
    Dimension textureDimension = getTextureDimension(texture);
    Dimension dimension;
    dimension.w = brickWidth;
    dimension.h = brickWidth * textureDimension.h / textureDimension.w;

    return dimension;
}

void brick_UpdateCollisionCheck(Brick** bricks, Dimension* brickDimension, int length, int indexBrickDestroyed) {

    int countBrickFound = 0;
    for (int j = 0; j < length; j++) {
        if(bricks[j]->checkCollision || bricks[j]->collider == NULL)
            continue;

        // check brick to the right
        if (bricks[indexBrickDestroyed]->position->y == bricks[j]->position->y &&
            bricks[indexBrickDestroyed]->position->x + brickDimension->w == bricks[j]->position->x) {
            countBrickFound++;
            bricks[j]->color = PURPLE;
            bricks[j]->checkCollision = true;
        } // check brick to the left
        else if (bricks[indexBrickDestroyed]->position->y == bricks[j]->position->y &&
                 bricks[indexBrickDestroyed]->position->x - brickDimension->w == bricks[j]->position->x) {
            countBrickFound++;
            bricks[j]->color = PURPLE;
            bricks[j]->checkCollision = true;
        }
        // check brick downward
        else if (bricks[indexBrickDestroyed]->position->x == bricks[j]->position->x &&
                 bricks[indexBrickDestroyed]->position->y + brickDimension->h == bricks[j]->position->y) {
            countBrickFound++;
            bricks[j]->color = PURPLE;
            bricks[j]->checkCollision = true;
        } // check brick upward
        else if (bricks[indexBrickDestroyed]->position->x == bricks[j]->position->x &&
                 bricks[indexBrickDestroyed]->position->y - brickDimension->h == bricks[j]->position->y) {
            countBrickFound++;
            bricks[j]->color = PURPLE;
            bricks[j]->checkCollision = true;
        }

        if(countBrickFound == 3)
            return;
    }
}


void brick_Free(Brick* brick) {
    free(brick->position);

    if (brick->collider) // the brick->collider can be free when it get hits by a ball
        free(brick->collider);

    free(brick->rect);

    brick->rect = NULL;
    brick->collider = NULL;
    brick->position = NULL;

    free(brick);
    brick = NULL;
}

void brick_Draw(Brick* brick) {
    if (!brick->shouldRender)
        return;
    SDL_SetTextureColorMod(brick->texture, AVAILABLE_COLORS[brick->color].r, AVAILABLE_COLORS[brick->color].g, AVAILABLE_COLORS[brick->color].b);
    SDL_RenderCopy(app.renderer, brick->texture, NULL, brick->rect);
}