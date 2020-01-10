#include "game.hpp"

#ifndef MYPIGFONT_H_   /* Include guard */
#define MYPIGFONT_H_
// https://crypto.interactive-maths.com/pigpen-cipher.html#intro

uint8_t* mypigfont(char, int colour);
void drawPigFont(uint8_t *GameBuff, int width, int height, int maxpixel, char character, int colour, int alpha, int x, int y);
void drawPigFont2x(uint8_t *GameBuff, int width, int height, int maxpixel, char character, int colour, int alpha, int x, int y);

#endif // MYPIGFONT_H_
