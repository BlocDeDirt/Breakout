#include "collider.h"
#include <SDL2/SDL.h>
AABBCollider* AABBCollider_Construct(float x, float y, float w, float h){
    AABBCollider* collider = malloc(sizeof(AABBCollider));
    collider->x = x;
    collider->y = y;
    collider->w = w;
    collider->h = h;

    return collider; 
}

CircleCollider* CircleCollider_Construct(float cx, float cy, float r){
    CircleCollider* collider = malloc(sizeof(CircleCollider));
    collider->cx = cx;
    collider->cy = cy;
    collider->r = r;
}