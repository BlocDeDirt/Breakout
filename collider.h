#ifndef COLLIDER_H
#define COLLIDER_H
typedef struct AABBCollider {
    float x;
    float y;
    float w;
    float h;
} AABBCollider;

typedef struct CircleCollider {
    float cx;
    float cy;
    float r;
} CircleCollider;

AABBCollider* AABBCollider_Construct(float x, float y, float w, float h);
CircleCollider* CircleCollider_Construct(float cx, float cy, float r);
#endif