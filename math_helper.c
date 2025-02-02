#include "math_helper.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int random(int min, int max) {
    static bool isSrandSet = false;
    if (!isSrandSet) {
        srand(time(NULL));
        isSrandSet = true;
    }

    return rand() % (max - min + 1) + min;
}

float deg2Rad(float deg) {
    return deg * PI / 180;
}

float rad2Deg(float rad) {
    return rad * 180 / PI;
}

double min(double a, double b) {
    if (a < b)
        return a;

    return b;
}

double max(double a, double b) {
    if (a > b)
        return a;

    return b;
}