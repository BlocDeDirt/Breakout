#ifndef BALL_H
#define BALL_H
#include "collider.h"
#include "color.h"
#include "paddle.h"
#include "vector.h"
#include "brick.h"
#include <SDL2/SDL.h>

typedef struct BallTrail {
    struct BallTrail* next;
    SDL_Texture* texture;
    Uint8 alpha;
    SDL_Rect* rect;
    Vector2* position;
} BallTrail;

typedef struct BallTrailList {
    BallTrail* head;
    BallTrail* tail;
} BallTrailList;

typedef struct Ball {
    SDL_Texture* texture;
    SDL_Rect* rect;
    AABBCollider* collider;
    CircleCollider* circleCollider;
    int size;
    Vector2* position;
    Vector2* positionCenter;
    Vector2* velocity;
    float r;

    BallTrailList* trailList;
    bool shouldDisplayTrail;
    float speed;
    bool wasLauched;

    bool wasXHit;
    bool wasYHit;
    unsigned int startTimeAnimation;
    int durationAnimationMs;
} Ball;


/**
 * The caller is the owner
 */
Ball* ball_Construct(SDL_Texture* texture, Vector2* position, float speed, int size);
void ball_Draw(Ball* ball);

/**
 * Free every property of the ball (even the trail list)
 */
void ball_Free(Ball* ball);

/**
 * Will switch the color of the ball. The pointer given to this function will be freed so call color_Construct()
 */
void ball_SwitchColor(Ball* ball, Color* color);
void ball_Update(Ball* ball, Paddle* paddle, Brick** bricks, Dimension *brickDimension, int brickLength);
void ball_CollideWithBricks(Ball* ball, Brick** bricks, int brickLength, Dimension* brickDimension);
void ball_Event(Ball* ball, const Uint8* keyboardState);

/**
 * Will detect if collide with bounds and will respond to it
 */
void ball_CollideWithBounds(Ball* ball, int xMaxBound, int xMinBound, int yMaxBound, int yMinBound);

/**
 * Will detect if collide with the paddle and will respond to it
 */
void ball_CollideWithPaddle(Ball* ball, Paddle* paddle);

/**
 * Will only affect the render of the ball to give it a "squish effect" (plot twist, I just shrink the height of the dest rect :O)
 */
void ball_Hit(Ball* ball);

/**
 * Ye it got 2 colliders, one square which is fast to calculate for the PC when it hit the bounds
 * or the paddle and the circle one is slower but more precise, so it's for the bricks >:D
 * So.. I need to update the collider from one to another and vice versa ¯\_(ツ)_/¯
 */
void ball_UpdateCircleColliderFromPosition(Ball* ball);
void ball_UpdatePositionFromCircleCollider(Ball* ball);


/****************************************************************/
/*************************BALL TRAIL*****************************/
/****************************************************************/

/**
 * Will decay & remove the trails of the ball from the linked list
 */
void ballTrailList_DecayTrails(BallTrailList* list, float decayFactor);
BallTrailList* ballTrailList_Construct();
void ballTrailList_Draw(BallTrailList* list);

/**
 * Will add a node to the linked list. The texture given is the texture of the ball, so do not free the texture
 * except if you want to frick everything up. The ball is the only responsable of the texture.
 */
void ballTrailList_Add(BallTrailList* list, SDL_Texture* texture, Vector2* position, int size);

/**
 * Free every node and their properties except the texture
 */
void ballTrailList_Free(BallTrailList* list);

/**
 * Free everything except its texture
 */
void ballTrail_Free(BallTrail* trail);

#endif