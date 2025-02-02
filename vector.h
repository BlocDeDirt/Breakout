#ifndef VECTOR_H
#define VECTOR_H
// https://www.youtube.com/watch?v=0iNrGpwZwog vector dot product
// https://natureofcode.com/vectors/
// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

/**
 * Return a pointer the caller is the owner.
 */
Vector2* vector2_Construct(float x, float y);
void vector2_Set(Vector2* a, float x, float y);

void vector2_Add(Vector2* a, Vector2* b);
void vector2_AddNoFree(Vector2* a, Vector2* b);

void vector2_Subtract(Vector2* a, Vector2* b);
void vector2_SubtractNoFree(Vector2* a, Vector2* b);

void vector2_Multiply(Vector2* a, float b);
void vector2_Divide(Vector2* a, float b);

/**
 * The magnitude of a vector is often written as : 
 *   ->
 * || v || or |v|
 */
float vector2_Magnitude(Vector2* a);
float vector2_MagnitudeSqrt(Vector2* a);

void vector2_Normalize(Vector2* a);
void vector2_NormalizeWithMagnitude(Vector2* a, float mag);
void vector2_Free(Vector2* a);
float vector2_DotProduct(Vector2* a, Vector2* b);
float vector2_AngleBetweenVector(Vector2* a, Vector2* b);
#endif