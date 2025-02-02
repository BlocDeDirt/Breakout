#include "ball.h"
#include "app.h"
#include "brick.h"
#include "collider.h"
#include "color.h"
#include "math_helper.h"
#include "paddle.h"
#include "vector.h"
#include <SDL2/SDL.h>
#define MAX_REBOUND_ANGLE 160
#define MIN_REBOUND_ANGLE 90
#define TRAIL_DECAY_FACTOR 0.2f
#define FAKE_BOUNCE_FACTOR 0.70f // at 1 the ball will be a god damn pancake when hitting anything
extern App app;

Ball* ball_Construct(SDL_Texture* texture, Vector2* position, float speed, int size) {
    const int angle = 90;
    Ball* ball = malloc(sizeof(Ball));
    ball->rect = malloc(sizeof(SDL_Rect));
    ball->trailList = ballTrailList_Construct();
    ball->shouldDisplayTrail = true;
    ball->speed = speed;
    ball->position = position;
    ball->texture = texture;

    ball->velocity = vector2_Construct(cos(deg2Rad(-angle)) * speed, sin(deg2Rad(-angle)) * speed);

    ball->collider = AABBCollider_Construct(position->x, position->y, size, size);
    float radius = (float)size / 2.0f;
    ball->circleCollider = CircleCollider_Construct(position->x + radius, position->y + radius, radius);

    ball->size = size;
    ball->rect->w = size;
    ball->rect->h = size;
    ball->rect->x = SDL_round(position->x);
    ball->rect->y = SDL_round(position->y);

    ball->wasLauched = false;

    ball->wasXHit = false;
    ball->wasYHit = false;
    ball->startTimeAnimation = 0;
    ball->durationAnimationMs = 150;

    ball_UpdateCircleColliderFromPosition(ball);
    return ball;
}

void ball_CollideWithBricks(Ball* ball, Brick** bricks, int brickLength, Dimension* brickDimension) {
    Vector2 closestPoints = { 0.0f, 0.0f };
    Vector2 distanceBtwCPAndCircle = { 0.0f, 0.0f };
    Vector2 pushVector = { 0.0f, 0.0f };
    for (int i = 0; i < brickLength; i++) {
        if (bricks[i]->collider == NULL || !bricks[i]->checkCollision)
            continue;

        closestPoints.x = max(bricks[i]->collider->x, min(ball->circleCollider->cx, bricks[i]->collider->x + bricks[i]->collider->w));
        closestPoints.y = max(bricks[i]->collider->y, min(ball->circleCollider->cy, bricks[i]->collider->y + bricks[i]->collider->h));

        distanceBtwCPAndCircle.x = ball->circleCollider->cx - closestPoints.x;
        distanceBtwCPAndCircle.y = ball->circleCollider->cy - closestPoints.y;

        // by the power of vector i compels you
        if (vector2_Magnitude(&distanceBtwCPAndCircle) < pow(ball->circleCollider->r, 2)) {
            float magnitude = vector2_MagnitudeSqrt(&distanceBtwCPAndCircle);
            float penetrationDepth = ball->circleCollider->r - magnitude;

            vector2_NormalizeWithMagnitude(&distanceBtwCPAndCircle, magnitude);
            pushVector.x = distanceBtwCPAndCircle.x * penetrationDepth;
            pushVector.y = distanceBtwCPAndCircle.y * penetrationDepth;

            ball->circleCollider->cx += pushVector.x;
            ball->circleCollider->cy += pushVector.y;

            float dot = vector2_DotProduct(ball->velocity, &distanceBtwCPAndCircle);
            // V′=V−2(V⋅n)n Reflection formula
            ball->velocity->x -= 2 * dot * distanceBtwCPAndCircle.x;
            ball->velocity->y -= 2 * dot * distanceBtwCPAndCircle.y;

            ball_UpdatePositionFromCircleCollider(ball);

            brick_UpdateCollisionCheck(bricks, brickDimension, brickLength, i);

            // i tried freeing the brick itself when it should be destroyed BUT, it gave me a segmentation fault.
            // My guess ? I free the god damn brick with its address, and the index can't manage to find the next element in the array lol
            free(bricks[i]->collider);
            bricks[i]->collider = NULL;
            bricks[i]->shouldRender = false;
            ball->wasYHit = true;
            ball->wasXHit = true;
            return;
        }
    }
}

void ball_CollideWithBounds(Ball* ball, int xMaxBound, int xMinBound, int yMaxBound, int yMinBound) {
    if (ball->collider->x + ball->collider->w > xMaxBound) {
        ball->collider->x = xMaxBound - ball->collider->w;
        ball->velocity->x = -ball->velocity->x;
        ball->wasXHit = true;
    } else if (ball->collider->x < xMinBound) {
        ball->collider->x = xMinBound;
        ball->velocity->x = -ball->velocity->x;
        ball->wasXHit = true;
    }

    if (ball->collider->y < yMinBound) {
        ball->collider->y = yMinBound;
        ball->velocity->y = -ball->velocity->y;
        ball->wasYHit = true;
    } else if (ball->collider->y + ball->collider->h > yMaxBound) {
        ball->collider->y = yMaxBound - ball->collider->h;
        ball->velocity->y = -ball->velocity->y;
        ball->wasYHit = true;
    }
}

void ball_CollideWithPaddle(Ball* ball, Paddle* paddle) {

    if (ball->collider->x + ball->collider->w > paddle->collider->x && ball->collider->x < paddle->collider->x + paddle->collider->w) {
        if (ball->collider->y + ball->collider->h > paddle->collider->y) {
            float cx = ball->collider->x + ball->collider->w / 2;
            float px = paddle->collider->x + paddle->collider->w / 2;
            float diff = fabs(px - cx);

            float paddleExtremityLength = paddle->collider->w / 2 + ball->collider->w / 2;
            float percent = (diff / paddleExtremityLength);
            float angle = -deg2Rad(MIN_REBOUND_ANGLE + (percent * (MAX_REBOUND_ANGLE - MIN_REBOUND_ANGLE)));

            ball->collider->y = paddle->collider->y - ball->collider->h;

            // bounce right
            if (px - cx < 0) {
                vector2_Set(ball->velocity, -cos(angle) * ball->speed, sin(angle) * ball->speed);
            } else { // bounce left
                vector2_Set(ball->velocity, cos(angle) * ball->speed, sin(angle) * ball->speed);
            }

            paddle->wasHit = true;
            ball->wasYHit = true;
        }
    }
}

void ball_Hit(Ball* ball) {
    if (!ball->wasXHit && !ball->wasYHit)
        return;

    if (ball->startTimeAnimation == 0) {
        ball->startTimeAnimation = SDL_GetTicks();
    }

    unsigned int elapsed = SDL_GetTicks() - ball->startTimeAnimation;
    float progress = (float)elapsed / ball->durationAnimationMs;
    // there is a small offset that i did not fix in the position when changing the size of the ball to create a bounce effect
    if (progress < 0.5) {
        float newDimension = ball->size - ((ball->size * FAKE_BOUNCE_FACTOR) * (progress * 2));
        ball->rect->h = SDL_round(newDimension);
    } else {
        float newDimension = (ball->size - (ball->size * FAKE_BOUNCE_FACTOR)) + ((ball->size * FAKE_BOUNCE_FACTOR) * ((progress - 0.5) * 2));
        ball->rect->h = SDL_round(newDimension);
    }

    if (elapsed >= ball->durationAnimationMs) {
        ball->startTimeAnimation = 0;
        ball->rect->h = ball->size;
        ball->wasXHit = false;
        ball->wasYHit = false;
    }
}

void ball_Event(Ball* ball, const Uint8* keyboardState){
    if (keyboardState[SDL_SCANCODE_UP]) {
        ball->wasLauched = true;
    }
}

void ball_Update(Ball* ball, Paddle* paddle, Brick** bricks, Dimension* brickDimension, int brickLength) {
    if (!ball->wasLauched) {
        ball->collider->x = (paddle->collider->x + paddle->collider->w / 2) - ball->collider->w / 2;
        ball->collider->y = (paddle->collider->y - ball->collider->h - 5);
        ball->position->x = ball->collider->x;
        ball->position->y = ball->collider->y;
        ball_UpdateCircleColliderFromPosition(ball);
        return;
    }

    ball->collider->x += ball->velocity->x * app.deltaTime;
    ball->collider->y += ball->velocity->y * app.deltaTime;

    ball_CollideWithBricks(ball, bricks, brickLength, brickDimension);
    ball_CollideWithBounds(ball, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 0);
    ball_CollideWithPaddle(ball, paddle);

    if (ball->shouldDisplayTrail)
        ballTrailList_Add(ball->trailList, ball->texture, vector2_Construct(ball->position->x, ball->position->y), ball->size);

    ballTrailList_DecayTrails(ball->trailList, TRAIL_DECAY_FACTOR);

    ball->position->x = ball->collider->x;
    ball->position->y = ball->collider->y;
    ball_UpdateCircleColliderFromPosition(ball);
}

void ball_UpdatePositionFromCircleCollider(Ball* ball) {
    ball->collider->x = ball->circleCollider->cx - ball->circleCollider->r;
    ball->collider->y = ball->circleCollider->cy - ball->circleCollider->r;
}

void ball_UpdateCircleColliderFromPosition(Ball* ball) {
    ball->circleCollider->cx = ball->collider->x + ball->circleCollider->r;
    ball->circleCollider->cy = ball->collider->y + ball->circleCollider->r;
}

void ball_Draw(Ball* ball) {
    ball_Hit(ball);

    ball->rect->x = SDL_round(ball->position->x);
    ball->rect->y = SDL_round(ball->position->y);
    ballTrailList_Draw(ball->trailList);

    SDL_SetTextureColorMod(ball->texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(ball->texture, 255);
    // will normally rotate the (texture of the) ball in the direction to where its heading
    SDL_RenderCopyEx(app.renderer, ball->texture, NULL, ball->rect, rad2Deg(atan2(ball->velocity->y, ball->velocity->x)) - 90, NULL, SDL_FLIP_VERTICAL);
}

void ball_SwitchColor(Ball* ball, Color* color) {
    SDL_SetTextureColorMod(ball->texture, color->r, color->g, color->b);
    free(color);
    color = NULL;
}


void ball_Free(Ball* ball) {
    free(ball->velocity);
    free(ball->position);
    free(ball->positionCenter);
    free(ball->collider);
    free(ball->circleCollider);
    SDL_DestroyTexture(ball->texture);
    free(ball->rect);

    ball->collider = NULL;
    ball->circleCollider = NULL;
    ball->rect = NULL;
    ball->velocity = NULL;
    ball->position = NULL;
    ball->texture = NULL;
    ballTrailList_Free(ball->trailList);
    free(ball);
    ball = NULL;
}

void ballTrailList_DecayTrails(BallTrailList* list, float decayFactor) {
    if (list == NULL || list->head == NULL) {
        return;
    }

    BallTrail* current = list->head;
    BallTrail* prev = NULL;

    while (current != NULL) {
        current->alpha -= current->alpha * decayFactor;

        if (current->alpha <= 0) {
            BallTrail* temp = current;
            current = current->next;

            if (prev == NULL) {
                list->head = current;
            } else {
                prev->next = current;
            }

            ballTrail_Free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

BallTrailList* ballTrailList_Construct() {
    BallTrailList* list = malloc(sizeof(BallTrailList));

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void ballTrailList_Free(BallTrailList* list) {
    BallTrail* current = list->head;
    while (current != NULL) {
        BallTrail* temp = current;
        current = current->next;

        ballTrail_Free(temp);
    }

    list = NULL;
}

void ballTrail_Free(BallTrail* trail) {
    free(trail->position);
    free(trail->rect);

    trail->position = NULL;
    trail->rect = NULL;

    free(trail);
    trail = NULL;
}

void ballTrailList_Draw(BallTrailList* list) {
    BallTrail* current = list->head;
    while (current != NULL) {
        if (current->alpha < 255 && current->alpha > 100) {
            SDL_SetTextureColorMod(current->texture, 194, 68, 68);
        } else if (current->alpha <= 100 && current->alpha > 25) {
            SDL_SetTextureColorMod(current->texture, 227, 141, 36);
        } else {
            SDL_SetTextureColorMod(current->texture, 255, 240, 79);
        }
        SDL_SetTextureAlphaMod(current->texture, current->alpha);
        SDL_RenderCopy(app.renderer, current->texture, NULL, current->rect);
        current = current->next;
    }
}

void ballTrailList_Add(BallTrailList* list, SDL_Texture* texture, Vector2* position, int size) {
    BallTrail* trail = malloc(sizeof(BallTrail));
    trail->rect = malloc(sizeof(SDL_Rect));
    trail->alpha = 255;
    trail->next = NULL;
    trail->texture = texture;
    trail->position = position;
    trail->rect->w = size;
    trail->rect->h = size;
    trail->rect->x = SDL_round(position->x);
    trail->rect->y = SDL_round(position->y);

    if (list->head == NULL) {
        list->head = trail;
        list->tail = trail;
    } else {
        list->tail->next = trail;
        list->tail = trail;
    }
}