#ifndef COLOR_H
#define COLOR_H
#include <SDL2/SDL.h>

enum EnumColor {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    PURPLE,
    WHITE
};

typedef struct Color {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
} Color;

/**
 * Return a pointer, the caller is the owner
 */
Color* color_Construct(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
enum EnumColor color_getRandom();
#endif