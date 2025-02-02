#include "vector.h"
#include "SDL2/SDL.h"
#include <math.h>
#include <stdbool.h>
Vector2* vector2_Construct(float x, float y) {
    Vector2* vector = malloc(sizeof(Vector2));
    vector->x = x;
    vector->y = y;
    return vector;
}

void vector2_Set(Vector2* a, float x, float y) {
    a->x = x;
    a->y = y;
}

void vector2_Add(Vector2* a, Vector2* b) {
    a->x += b->x;
    a->y += b->y;

    free(b);
    b = NULL;
}


void vector2_AddNoFree(Vector2* a, Vector2* b) {
    a->x += b->x;
    a->y += b->y;
}

void vector2_Subtract(Vector2* a, Vector2* b) {
    a->x -= b->x;
    a->y -= b->y;

    free(b);
    b = NULL;
}

void vector2_SubtractNoFree(Vector2* a, Vector2* b) {
    a->x -= b->x;
    a->y -= b->y;
}

void vector2_Multiply(Vector2* a, float b) {
    a->x *= b;
    a->y *= b;
}
void vector2_Divide(Vector2* a, float b) {
    if (fabs(b) == 0) {
        printf("Divide by zero");
        exit(EXIT_FAILURE);
        return;
    }
    a->x /= b;
    a->y /= b;
}

float vector2_MagnitudeSqrt(Vector2* a) {
    return sqrtf(a->x * a->x + a->y * a->y);
}

float vector2_Magnitude(Vector2* a) {
    return a->x * a->x + a->y * a->y;
}

void vector2_Normalize(Vector2* a) {
    vector2_Divide(a, vector2_MagnitudeSqrt(a));
}

void vector2_NormalizeWithMagnitude(Vector2* a, float mag){
    vector2_Divide(a, mag);
}

float vector2_DotProduct(Vector2* a, Vector2* b) {
    return (a->x * b->x) + (a->y * b->y);
}

float vector2_AngleBetweenVector(Vector2* a, Vector2* b) {
    float _cos = vector2_DotProduct(a, b) / (vector2_Magnitude(a) * vector2_Magnitude(b));
    return acos(_cos);
}

void vector2_Free(Vector2* a) {
    free(a);
    a = NULL;
}