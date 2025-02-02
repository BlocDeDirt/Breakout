#include "level_parser.h"
#include "app.h"
#include "texture.h"
#include <stdio.h>
Level* level_ParseFile(const Dimension* brickDimension, int windowWidth, int windowHeight, SDL_Texture* brickTexture) {
    // FILE* filePointer = fopen("./levels/space_invaders.txt", "r");
    FILE* filePointer = fopen("./levels/wall.txt", "r");
    if (filePointer == NULL) {
        printf("God fricking dammit the file doesnt want to open D:<\n");
        exit(EXIT_FAILURE);
    }

    int maxColumnBrick = windowWidth / brickDimension->w;
    int maxRowBrick = ((float)windowHeight * 0.5f) / brickDimension->h;

    Level* level = malloc(sizeof(Level));
    level->length = 0;
    // YOLO, here we go I allocate a shit tons of memory, I will just reallocate after the length update and parsing of the file :D
    level->bricks = malloc((maxColumnBrick * maxRowBrick) * sizeof(Brick));

    int countColumn = 0;
    int countRow = 0;
    int i = 0;

    int ch;
    while ((ch = fgetc(filePointer)) && !feof(filePointer) && countRow < maxRowBrick) {
        if (ch == '\n') {
            countRow++;
            countColumn = 0;
            i = 0;
        } else {
            if (ch == '#' && countColumn < maxColumnBrick) {
                level->bricks[level->length] = brick_Construct(brickTexture, vector2_Construct(i * brickDimension->w, countRow * brickDimension->h), brickDimension, RED);
                countColumn++;
                level->length++;
            }
            i++;
        }
    }
    fclose(filePointer);
    // let's free the memory that i don't use :D
    level->bricks = realloc(level->bricks, level->length * sizeof(Brick));

    level_updateCollisionCheck(level, brickDimension);
    return level;
}

void level_updateCollisionCheck(Level* level, const Dimension* brickDimension) {

    for (int i = 0; i < level->length; i++) {
        int countBrickFound = 0;

        for (int j = 0; j < level->length; j++) {
            // check brick to the right
            if (level->bricks[i]->position->y == level->bricks[j]->position->y &&
                level->bricks[i]->position->x + brickDimension->w == level->bricks[j]->position->x) {
                countBrickFound++;
            } // check brick to the left
            else if (level->bricks[i]->position->y == level->bricks[j]->position->y &&
                     level->bricks[i]->position->x - brickDimension->w == level->bricks[j]->position->x) {
                countBrickFound++;
            }
            // check brick downward
            else if (level->bricks[i]->position->x == level->bricks[j]->position->x &&
                     level->bricks[i]->position->y + brickDimension->h == level->bricks[j]->position->y) {
                countBrickFound++;
            } // check brick upward
            else if (level->bricks[i]->position->x == level->bricks[j]->position->x &&
                     level->bricks[i]->position->y - brickDimension->h == level->bricks[j]->position->y) {
                countBrickFound++;
            }

            if(countBrickFound == 4)
                break;
        }

        //can think that it would be better in a single condition connected with OR but IMO it's easier to read
        // if in the middle
        if (countBrickFound == 4) {
            level->bricks[i]->color = GREEN;
            level->bricks[i]->checkCollision = false;
        } // if stuck to a wall/ceiling
        else if (countBrickFound == 3 &&
                 (level->bricks[i]->position->x == 0 ||
                  level->bricks[i]->position->x == WINDOW_WIDTH - brickDimension->w ||
                  level->bricks[i]->position->y == 0)) {
            level->bricks[i]->color = GREEN;
            level->bricks[i]->checkCollision = false;
        } // if in a top corner
        else if (countBrickFound == 2 && level->bricks[i]->position->y == 0 &&
                 (level->bricks[i]->position->x == 0 || level->bricks[i]->position->x == WINDOW_WIDTH - brickDimension->w)) {
            level->bricks[i]->color = GREEN;
            level->bricks[i]->checkCollision = false;
        }
    }
}

void level_Free(Level* level) {
    for (int i = 0; i < level->length; i++) {
        brick_Free(level->bricks[i]);
    }

    free(level);
    level = NULL;
}
