#ifndef PADDLE_H
#define PADDLE_H
#include "app.h"
#include "collider.h"
#include "color.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct Paddle {
    SDL_Rect* middle;
    SDL_Rect* leftTip;
    SDL_Rect* rightTip;
    Color* color;
    Color* colorTip;
    Vector2* position;
    Vector2* velocity;
    AABBCollider* collider;
    float maxVelocity;
    bool goingLeft;
    bool goingRight;
    bool hasStartDecceleration;
    bool hasStartAcceleration;

    bool wasHit;
    unsigned int startTimeHitAnimation;
    int durationAnimationMs;
    float saveYPosition;

    float acceleration;
    float decceleration;
    float time;
    float mass;
} Paddle;

/**
 * Return a pointer the caller is the owner
 */
Paddle* paddle_Construct(unsigned int paddleWidthWithTips, unsigned int paddleTipWidth, unsigned int paddleHeight, float paddleMass, Color* color, Vector2* position);

/**
 * Free every property of the paddle and itself.
 */
void paddle_Free(Paddle* paddle);
void paddle_Draw(Paddle* paddle);
void paddle_Update(Paddle* paddle);
void paddle_Event(Paddle* paddle, const Uint8* keyboardState);

/**
 * Will return true if has collision with given bounds. If a collision occurs, the collider will be set with the value of the bound.
 */
bool paddle_hasCollisionWithBounds(Paddle* paddle, unsigned int maxBound, unsigned int minBound);

/**
 * Because the paddle is from 3 rects, i will set & calculate their position here
 */
void paddle_SetRectPosition(Paddle* paddle);

/**
 * Will see if the paddle was hit by a ball, if yes, the paddle will do a subtle movement of up & down (or down & up)
 */
void paddle_HitByBall(Paddle* paddle);

#endif