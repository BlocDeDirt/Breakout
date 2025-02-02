#include "cloud.h"
#include "app.h"
#include "math_helper.h"
#include "texture.h"
#include <SDL2/SDL.h>
extern App app;

CloudBackground* cloudbackground_Construct() {
    const char* texturePathes[] = {
        "./assets/cloud1_64.png",
        "./assets/cloud2_64.png",
        "./assets/cloud3_64.png",
        "./assets/cloud4_64.png",
    };

    CloudBackground* cloudBackground = malloc(sizeof(CloudBackground));
    cloudBackground->minYLevel = 10;
    cloudBackground->maxYLevel = WINDOW_HEIGHT - WINDOW_HEIGHT * 0.75f;

    cloudBackground->cloudLength = 2;
    cloudBackground->textureLength = sizeof(texturePathes) / sizeof(texturePathes[0]);

    cloudBackground->textures = malloc(cloudBackground->textureLength * sizeof(SDL_Texture*));
    for (int i = 0; i < cloudBackground->textureLength; i++) {
        cloudBackground->textures[i] = texture_Load(texturePathes[i]);
    }

    cloudBackground->clouds = malloc(cloudBackground->cloudLength * sizeof(Cloud));
    cloudBackground->clouds[0] = cloud_Construct(cloudBackground->textures[random(0, cloudBackground->textureLength - 1)], vector2_Construct(WINDOW_HEIGHT, random(cloudBackground->minYLevel, cloudBackground->maxYLevel)), 160, 20);
    cloudBackground->clouds[1] = cloud_Construct(cloudBackground->textures[random(0, cloudBackground->textureLength - 1)], vector2_Construct(WINDOW_HEIGHT, random(cloudBackground->minYLevel, cloudBackground->maxYLevel)), 300, 35);

    return cloudBackground;
}

void cloudbackground_Free(CloudBackground* bg) {
    for (int i = 0; i < bg->cloudLength; i++) {
        cloud_Free(bg->clouds[i]);
    }

    free(bg->clouds);
    bg->clouds = NULL;

    for (int i = 0; i < bg->textureLength; i++) {
        SDL_DestroyTexture(bg->textures[i]);
    }

    free(bg);
    bg = NULL;
}

void cloudbackground_Draw(CloudBackground* bg) {
    for (int i = 0; i < bg->cloudLength; i++) {
        //still on screen
        if (bg->clouds[i]->position->x + bg->clouds[i]->rect->w >= 0) {
            bg->clouds[i]->position->x -= bg->clouds[i]->speed * app.deltaTime;
            cloud_Draw(bg->clouds[i]);
        } else {
            //start a timer for their return on screen
            if (bg->clouds[i]->timeoutDuration == 0) {
                bg->clouds[i]->timeoutDuration = random(1000, 5000);
                bg->clouds[i]->startTimeout = SDL_GetTicks();
            }

            unsigned int elapsed = SDL_GetTicks() - bg->clouds[i]->startTimeout;
            if (elapsed >= bg->clouds[i]->timeoutDuration) {
                bg->clouds[i]->timeoutDuration = 0;
                bg->clouds[i]->position->x = WINDOW_WIDTH;

                bg->clouds[i]->texture = bg->textures[random(0, bg->textureLength - 1)];
                bg->clouds[i]->position->y = random(bg->minYLevel, bg->maxYLevel);


                // Stonk, twice the texture
                if (random(0, 1)) {
                    bg->clouds[i]->flip = SDL_FLIP_NONE;
                } else {
                    bg->clouds[i]->flip = SDL_FLIP_HORIZONTAL;
                }
            }
        }
    }
}

Cloud* cloud_Construct(SDL_Texture* texture, Vector2* position, int size, int speed) {
    Cloud* cloud = malloc(sizeof(Cloud));
    cloud->rect = malloc(sizeof(SDL_Rect));

    cloud->rect->x = position->x;
    cloud->rect->y = position->y;
    cloud->rect->w = size;
    cloud->rect->h = size;

    cloud->texture = texture;
    cloud->position = position;
    cloud->speed = speed;

    cloud->flip = SDL_FLIP_NONE;

    return cloud;
}

void cloud_Draw(Cloud* cloud) {
    cloud->rect->y = SDL_round(cloud->position->y);
    cloud->rect->x = SDL_round(cloud->position->x);
    SDL_RenderCopyEx(app.renderer, cloud->texture, NULL, cloud->rect, 0, NULL, cloud->flip);
}

void cloud_Free(Cloud* cloud) {
    free(cloud->rect);
    free(cloud->position);
    cloud->rect = NULL;
    cloud->position = NULL;
    free(cloud);
    cloud = NULL;
}