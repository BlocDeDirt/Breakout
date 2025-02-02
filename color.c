#include "color.h"
#include "math_helper.h"
#include <SDL2/SDL.h>

const Color AVAILABLE_COLORS[] = {
    { 191, 68, 44, 255 },   // red
    { 232, 155, 72, 255 },  // orange
    { 255, 245, 33, 255 },  // yellow
    { 96, 189, 30, 255 },   // green
    { 30, 125, 189, 255 },  // blue
    { 154, 30, 189, 255 },  // purple
    { 255, 255, 255, 255 }, // white
};

Color* color_Construct(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    Color* color = malloc(sizeof(Color));
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
    return color;
}

enum EnumColor color_getRandom() {
    enum EnumColor c[] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, WHITE };

    return c[random(0, (sizeof(c) / sizeof(c[0])) - 1)];
}