#include "paddle.h"
#include "app.h"
#include "color.h"
#include "vector.h"
#include <SDL2/SDL.h>
extern App app;

Paddle* paddle_Construct(unsigned int paddleWidthWithTips, unsigned int paddleTipWidth, unsigned int paddleHeight, float paddleMass, Color* color, Vector2* position) {
    Paddle* paddle = malloc(sizeof(Paddle));
    paddle->color = color;
    paddle->colorTip = color_Construct(84, 117, 138, 255);

    paddle->middle = malloc(sizeof(SDL_Rect));
    paddle->middle->w = paddleWidthWithTips - 2 * paddleTipWidth;
    paddle->middle->h = paddleHeight;
    paddle->middle->y = position->y;

    paddle->leftTip = malloc(sizeof(SDL_Rect));
    paddle->leftTip->w = paddleTipWidth;
    paddle->leftTip->h = paddleHeight;
    paddle->leftTip->y = position->y;

    paddle->rightTip = malloc(sizeof(SDL_Rect));
    paddle->rightTip->w = paddleTipWidth;
    paddle->rightTip->h = paddleHeight;
    paddle->rightTip->y = position->y;


    paddle->collider = AABBCollider_Construct(position->x, position->y, paddleWidthWithTips, paddleHeight);

    paddle->mass = 1.0f;
    paddle->position = position;
    paddle->velocity = vector2_Construct(0.0f, 0.0f);
    paddle->goingLeft = false;
    paddle->goingRight = false;

    paddle->wasHit = false;
    paddle->startTimeHitAnimation = 0;
    paddle->durationAnimationMs = 100;
    paddle->saveYPosition = position->y;

    paddle->acceleration = 1000.0f;
    paddle->decceleration = 1000.0f;
    paddle->maxVelocity = 500.0f;

    paddle->hasStartAcceleration = false;
    paddle->hasStartDecceleration = false;
    paddle->time = 0.0f;

    paddle_SetRectPosition(paddle);
    return paddle;
}

void paddle_SetRectPosition(Paddle* paddle) {
    const int position = SDL_round(paddle->position->x);
    paddle->leftTip->x = position;
    paddle->middle->x = paddle->leftTip->x + paddle->leftTip->w;
    paddle->rightTip->x = paddle->middle->x + paddle->middle->w;

    const int yPosition = SDL_round(paddle->position->y);
    paddle->leftTip->y = yPosition;
    paddle->middle->y = yPosition;
    paddle->rightTip->y = yPosition;
}


void paddle_Free(Paddle* paddle) {
    free(paddle->rightTip);
    free(paddle->leftTip);
    free(paddle->colorTip);
    paddle->colorTip = NULL;
    paddle->leftTip = NULL;
    paddle->rightTip = NULL;

    free(paddle->collider);
    paddle->collider = NULL;
    free(paddle->color);
    paddle->color = NULL;
    free(paddle->middle);
    paddle->middle = NULL;
    free(paddle->position);
    paddle->position = NULL;
    free(paddle->velocity);
    paddle->velocity = NULL;
    free(paddle);
    paddle = NULL;
}

bool paddle_hasCollisionWithBounds(Paddle* paddle, unsigned int maxBound, unsigned int minBound) {
    if (paddle->collider->x + paddle->collider->w > maxBound) {
        paddle->collider->x = maxBound - paddle->collider->w;
        return true;
    }

    if (paddle->collider->x < minBound) {
        paddle->collider->x = minBound;
        return true;
    }

    return false;
}

void paddle_Draw(Paddle* paddle) {
    paddle_SetRectPosition(paddle);

    SDL_SetRenderDrawColor(app.renderer, paddle->color->r, paddle->color->g, paddle->color->b, paddle->color->a);
    SDL_RenderFillRect(app.renderer, paddle->middle);

    SDL_SetRenderDrawColor(app.renderer, paddle->colorTip->r, paddle->colorTip->g, paddle->colorTip->b, paddle->colorTip->a);
    SDL_RenderFillRect(app.renderer, paddle->rightTip);
    SDL_RenderFillRect(app.renderer, paddle->leftTip);
}

void paddle_Event(Paddle* paddle, const Uint8* keyboardState) {
    paddle->goingLeft = false;
    paddle->goingRight = false;
    if (keyboardState[SDL_SCANCODE_LEFT]) {
        paddle->goingLeft = true;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT]) {
        paddle->goingRight = true;
    }
}

void paddle_HitByBall(Paddle* paddle) {
    //Idk if i should call this in the Draw function instead of the update
    if (!paddle->wasHit)
        return;

    if (paddle->startTimeHitAnimation == 0) {
        paddle->startTimeHitAnimation = SDL_GetTicks();
    }

    unsigned int elapsed = SDL_GetTicks() - paddle->startTimeHitAnimation;
    float progress = (float)elapsed / paddle->durationAnimationMs;

    if (progress < 0.5f) {
        float newYPos = 5 * (progress) * 2;
        paddle->position->y += newYPos;
    } else {
        float newYPos = 5 - (5 * ((progress - 0.5) * 2));
        paddle->position->y -= newYPos;
    }

    if (elapsed >= paddle->durationAnimationMs) {
        paddle->startTimeHitAnimation = 0;
        paddle->position->y = paddle->saveYPosition;
        paddle->wasHit = false;
    }
}

void paddle_Update(Paddle* paddle) {
    paddle->time += app.deltaTime;
    // if both keys are/aren't pressed start deccelerating.
    if ((!paddle->goingLeft && !paddle->goingRight) || (paddle->goingLeft && paddle->goingRight)) {
        if (!paddle->hasStartDecceleration) {
            paddle->time = 0.0f;
        }
        paddle->hasStartDecceleration = true;
        paddle->hasStartAcceleration = false;

        if (paddle->velocity->x != 0.0f) {
            float deccelerationStep = paddle->decceleration * paddle->time;
            paddle->velocity->x += (paddle->velocity->x > 0 ? -deccelerationStep : deccelerationStep);

            if (fabs(paddle->velocity->x) < deccelerationStep) {
                paddle->velocity->x = 0.0f;
            }
        }
    } else {
        if (!paddle->hasStartAcceleration) {
            paddle->time = 0.0f;
        }
        paddle->hasStartDecceleration = false;
        paddle->hasStartAcceleration = true;

        if (paddle->goingRight) {
            paddle->velocity->x += paddle->acceleration * paddle->time;
        }
        if (paddle->goingLeft) {
            paddle->velocity->x -= paddle->acceleration * paddle->time;
        }

        if (paddle->velocity->x > paddle->maxVelocity) {
            paddle->velocity->x = paddle->maxVelocity;
        } else if (paddle->velocity->x < -paddle->maxVelocity) {
            paddle->velocity->x = -paddle->maxVelocity;
        }
    }

    paddle->collider->x += paddle->velocity->x * app.deltaTime;

    if (paddle_hasCollisionWithBounds(paddle, WINDOW_WIDTH, 0)) {
        paddle->velocity->x = 0;
        paddle->time = 0.0f;
    }

    paddle->position->x = paddle->collider->x;

    paddle_HitByBall(paddle);
}