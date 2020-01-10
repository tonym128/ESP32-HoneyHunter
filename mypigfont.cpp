#include "mypigfont.hpp"
using namespace std;
uint8_t *pigEmptyString = nullptr;

void drawTopLine(uint8_t *currentFont, uint8_t colour) {
    int i = 8;
    while (i < 16) {
        currentFont[i] = colour;
        i++;
    }
}

void drawBottomLine(uint8_t *currentFont, uint8_t colour) {
    int i = 63;
    while (i > 63-8) {
        currentFont[i-8] = colour;
        i--;
    }
}
void drawLeftLine(uint8_t *currentFont, uint8_t colour) {
    int i = 0;
    while (i < 64) {
        currentFont[i+1] = colour;
        i+=8;
    }
}

void drawRightLine(uint8_t *currentFont, uint8_t colour) {
    int i = 7;
    while (i < 64) {
        currentFont[i-1] = colour;
        i+=8;
    }
}

void drawDot(uint8_t *currentFont, uint8_t colour) {
    currentFont[24 + 3] = currentFont[24 + 4] = currentFont[32 + 3] = currentFont[32 + 4] = colour;
}

void drawV(uint8_t *currentFont, uint8_t colour) {
    currentFont[0] = currentFont[8] = currentFont[16] = colour;
    currentFont[16+1] = currentFont[24+1] = currentFont[32+1] = colour;
    currentFont[32+2] = currentFont[40+2] = currentFont[48+2] = colour;
    currentFont[48+3] = currentFont[56+3] = colour;
    currentFont[48+4] = currentFont[56+4] = colour;
    currentFont[32+5] = currentFont[40+5] = currentFont[48+5] = colour;
    currentFont[16+6] = currentFont[24+6] = currentFont[32+6] = colour;
    currentFont[0+7] = currentFont[8+7] = currentFont[16+7] = colour;
}

void drawN(uint8_t *currentFont, uint8_t colour) {
    currentFont[63-(0)] = currentFont[63-(8)] = currentFont[63-(16)] = colour;
    currentFont[63-(16+1)] = currentFont[63-(24+1)] = currentFont[63-(32+1)] = colour;
    currentFont[63-(32+2)] = currentFont[63-(40+2)] = currentFont[63-(48+2)] = colour;
    currentFont[63-(48+3)] = currentFont[63-(56+3)] = colour;
    currentFont[63-(48+4)] = currentFont[63-(56+4)] = colour;
    currentFont[63-(32+5)] = currentFont[63-(40+5)] = currentFont[63-(48+5)] = colour;
    currentFont[63-(16+6)] = currentFont[63-(24+6)] = currentFont[63-(32+6)] = colour;
    currentFont[63-(0+7)] = currentFont[63-(8+7)] = currentFont[63-(16+7)] = colour;
}

void drawLessThan(uint8_t *currentFont, uint8_t colour) {
    currentFont[0] = currentFont[1] = currentFont[2] = colour;
    currentFont[2+8] = currentFont[3+8] = currentFont[4+8] = colour;
    currentFont[5+16] = currentFont[6+16] = currentFont[7+16] = colour;
    currentFont[6+24] = currentFont[7+24] = colour;
    currentFont[6+32] = currentFont[7+32] = colour;
    currentFont[5+40] = currentFont[6+40] = currentFont[7+40] = colour;
    currentFont[3+48] = currentFont[4+48] = currentFont[5+48] = colour;
    currentFont[1+56] = currentFont[2+56] = currentFont[3+56] = colour;
}

void drawMoreThan(uint8_t *currentFont, uint8_t colour) {
    currentFont[63-0] = currentFont[63-1] = currentFont[63-2] = colour;
    currentFont[63-(2+8)] = currentFont[63-(3+8)] = currentFont[63-(4+8)] = colour;
    currentFont[63-(5+16)] = currentFont[63-(6+16)] = currentFont[63-(7+16)] = colour;
    currentFont[63-(6+24)] = currentFont[63-(7+24)] = colour;
    currentFont[63-(6+32)] = currentFont[63-(7+32)] = colour;
    currentFont[63-(5+40)] = currentFont[63-(6+40)] = currentFont[63-(7+40)] = colour;
    currentFont[63-(3+48)] = currentFont[63-(4+48)] = currentFont[63-(5+48)] = colour;
    currentFont[63-(1+56)] = currentFont[63-(2+56)] = currentFont[63-(3+56)] = colour;
}

uint8_t* mypigfont(char x, int colour) {
    if (pigEmptyString == nullptr) pigEmptyString = (uint8_t *)malloc(sizeof(uint8_t)*64);
    for (int i = 0; i < 64; i++) pigEmptyString[i] = 0;

	if (x == ' ' || ( x >= 'a' && x <= 'z'))
		switch (x) { // a-z and ' '
            case ' ': 
                break;
            case 'a':
                drawBottomLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'b':
                drawBottomLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'c':
                drawBottomLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                break;
            case 'd':
                drawTopLine(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'e':
                drawTopLine(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'f':
                drawTopLine(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                break;
            case 'g':
                drawTopLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'h':
                drawTopLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'i':
                drawTopLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                break;
            case 'j':
                drawDot(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'k':
                drawDot(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'l':
                drawDot(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                break;
            case 'm':
                drawDot(pigEmptyString,colour);
                drawTopLine(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'n':
                drawDot(pigEmptyString,colour);
                drawTopLine(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'o':
                drawDot(pigEmptyString,colour);
                drawTopLine(pigEmptyString,colour);
                drawBottomLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                break;
            case 'p':
                drawDot(pigEmptyString,colour);
                drawTopLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'q':
                drawDot(pigEmptyString,colour);
                drawTopLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                drawRightLine(pigEmptyString,colour);
                break;
            case 'r':
                drawDot(pigEmptyString,colour);
                drawTopLine(pigEmptyString,colour);
                drawLeftLine(pigEmptyString,colour);
                break;
            case 's':
                drawV(pigEmptyString,colour);
                break;
            case 't':
                drawMoreThan(pigEmptyString,colour);
                break;
            case 'u':
                drawLessThan(pigEmptyString,colour);
                break;
            case 'v':
                drawN(pigEmptyString,colour);
                break;
            case 'w':
                drawDot(pigEmptyString,colour);
                drawV(pigEmptyString,colour);
                break;
            case 'x':
                drawDot(pigEmptyString,colour);
                drawMoreThan(pigEmptyString,colour);
                break;
            case 'y':
                drawDot(pigEmptyString,colour);
                drawLessThan(pigEmptyString,colour);
                break;
            case 'z':
                drawDot(pigEmptyString,colour);
                drawN(pigEmptyString,colour);
                break;
        }

    return pigEmptyString;
}

char mypigfontCharacter(char x) {
    if (x >= 65 && x <= 90) x += 32;

    return x;
}

void drawPigFont(uint8_t *GameBuff, int width, int height, int maxpixel, char character, int colour, int alpha, int x, int y) {
	int xPos = x;
	int yPos = y;
	int pixelScreen = 0;

	if (character == ' ') {
		if (alpha == 0) { return; }

		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				if (xPos > 0 && xPos < width && yPos > 0 && yPos < height) {
					pixelScreen = xPos + yPos * width;
					if (pixelScreen < maxpixel) GameBuff[pixelScreen] = 0;
				}
				xPos++;
			}
			yPos++;
			xPos = x;
		}
	}

	char lookup = mypigfontCharacter(character);
	mypigfont(lookup,colour);

	int pixel = 0;
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			pixel = i + j * 8;
			if (xPos > 0 && xPos < width && yPos > 0 && yPos < height) {
				pixelScreen = xPos + yPos * width;
				if (!(alpha == 0 && pigEmptyString[pixel] == 0) && pixel < maxpixel) GameBuff[pixelScreen] = pigEmptyString[pixel] == 0 ? 0 : colour;
			}
			xPos++;
		}
		yPos++;
		xPos = x;
	}
}

void drawPigFont2x(uint8_t *gameBuff, int width, int height, int maxpixel, char character, int colour, int alpha, int x, int y) {
	int xPos = x;
	int yPos = y;
	int pixelScreen = 0;

	if (character == ' ') {
		if (alpha == 0) { return; }

		for (int j = 0; j < 16; j++) {
			for (int i = 0; i < 16; i++) {
				if (xPos > 0 && xPos < width && yPos > 0 && yPos < height) {
					pixelScreen = xPos + yPos * width;
					if ((pixelScreen+1+width) < maxpixel)
						gameBuff[pixelScreen] = 0;
						gameBuff[pixelScreen+1] = 0;
						gameBuff[pixelScreen+width] = 0;
						gameBuff[pixelScreen+1+width] = 0;
				}
				xPos++;
			}
			yPos++;
			xPos = x;
		}
	}

	char lookup = mypigfontCharacter(character);
	mypigfont(lookup,colour);

	int pixel = 0;
	bool pixelV = false;

	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			pixel = i + j * 8;
			if (xPos >= 0 && xPos < width && yPos >= 0 && yPos < height) {
				pixelScreen = xPos + yPos * width;
				pixelV = pigEmptyString[pixel];
				if (!(alpha == 0 && pixelV == 0) && pixelScreen+1+width < maxpixel) {
					gameBuff[pixelScreen] = pixelV == 0 ? 0 : colour;
					gameBuff[pixelScreen+1] = pixelV == 0 ? 0 : colour;
					gameBuff[pixelScreen+width] = pixelV == 0 ? 0 : colour;
					gameBuff[pixelScreen+1+width] = pixelV == 0 ? 0 : colour;
				}
			}
			xPos+=2;
		}
		yPos+=2;
		xPos = x;
	}
}
