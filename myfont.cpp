#include "game.hpp"
#include "myfont.hpp"
using namespace std;

uint8_t* myfont(int x, int colour) {
	int line = x / 16 * 8;
	int col = x % 16 * 8;

	int width = 128;

	uint8_t* character = new uint8_t[64];
	int counter = 0;

	// Space
	if (x == 99) {
		for (int i = 0; i < 64; i++) character[i] = 0;
	}
	// Everything Else
	else {
		int pixel = 0;
		for (int j = line; j < line + 8; j++) {
			for (int i = col; i < col + 8; i++) {
				pixel = i + j * width;
				character[counter++] = fontData[pixel] == 0 ? 0 : colour;
			}
		}
	}

	return character;
}

int checkMap(char character) {
	switch (character) {
		case ' ': return 99;
		case '!': return 0;
		case '"': return 1;
		case '#': return 2;
		case '$': return 3;
		case '%': return 4;
		case '&': return 5;
		case '\'': return 6;
		case '(': return 7;
		case ')': return 8;
		case '*': return 9;
		case '+': return 10;
		case ',': return 11;
		case '-': return 12;
		case '.': return 13;
		case '/': return 14;
		case ':': return 25;
		case ';': return 26;
		case '<': return 27;
		case '=': return 28;
		case '>': return 29;
		case '?': return 30;
		case '@': return 31;
		case '[': return 58;
		case '\\': return 59;
		case ']': return 60;
		//case '(': return 91;
		case '|': return 92;
		//case ')': return 93;
		case '~': return 94;
		case 'A': return 32;
		case 'B': return 33;
		case 'C': return 34;
		case 'D': return 35;
		case 'E': return 36;
		case 'F': return 37;
		case 'G': return 38;
		case 'H': return 39;
		case 'I': return 40;
		case 'J': return 41;
		case 'K': return 42;
		case 'L': return 43;
		case 'M': return 44;
		case 'N': return 45;
		case 'O': return 46;
		case 'P': return 47;
		case 'Q': return 48;
		case 'R': return 49;
		case 'S': return 50;
		case 'T': return 51;
		case 'U': return 52;
		case 'V': return 53;
		case 'W': return 54;
		case 'X': return 55;
		case 'Y': return 56;
		case 'Z': return 57;
		case 'a': return 65;
		case 'b': return 66;
		case 'c': return 67;
		case 'd': return 68;
		case 'e': return 69;
		case 'f': return 70;
		case 'g': return 71;
		case 'h': return 72;
		case 'i': return 73;
		case 'j': return 74;
		case 'k': return 75;
		case 'l': return 76;
		case 'm': return 77;
		case 'n': return 78;
		case 'o': return 79;
		case 'p': return 80;
		case 'q': return 81;
		case 'r': return 82;
		case 's': return 83;
		case 't': return 84;
		case 'u': return 85;
		case 'v': return 86;
		case 'w': return 87;
		case 'x': return 88;
		case 'y': return 89;
		case 'z': return 90;
		case '0': return 15;
		case '1': return 16;
		case '2': return 17;
		case '3': return 18;
		case '4': return 19;
		case '5': return 20;
		case '6': return 21;
		case '7': return 22;
		case '8': return 23;
		case '9': return 24;

		default: return 0;
	};
}

uint8_t* myfont(char x, int colour) {
	return myfont(checkMap(x), colour);
}

int myfontCharacter(char x) {
	return checkMap(x);
}

void drawFont(uint8_t *GameBuff, int width, int height, int maxpixel, char character, int colour, int alpha, int x, int y) {
	int xPos = x;
	int yPos = y;
	int pixelScreen = 0;

	if (character == ' ') {
		if (alpha == 0) { return; }

		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				if (xPos >= 0 && xPos < width && yPos >= 0 && yPos < height) {
					pixelScreen = xPos + yPos * width;
					if (pixelScreen > 0 && pixelScreen < maxpixel) GameBuff[pixelScreen] = 0;
				}
				xPos++;
			}
			yPos++;
			xPos = x;
		}
	}

	int startPos = myfontCharacter(character);
	int line = startPos / 16 * 8;
	int col = startPos % 16 * 8;

	int fontwidth = 128;

	int pixel = 0;
	for (int j = line; j < line + 8; j++) {
		for (int i = col; i < col + 8; i++) {
			if (xPos > 0 && xPos < width && yPos > 0 && yPos < height) {
				pixel = i + j * fontwidth;
				pixelScreen = xPos + yPos * width;
				if (!(alpha == 0 && fontData[pixel] == 0) && pixelScreen < maxpixel) GameBuff[pixelScreen] = fontData[pixel] == 0 ? 0 : colour;
			}
			xPos++;
		}
		yPos++;
		xPos = x;
	}
}

void drawFont2x(uint8_t *gameBuff, int width, int height, int maxpixel, char character, int colour, int alpha, int x, int y) {
	int xPos = x;
	int yPos = y;
	int pixelScreen = 0;

	if (character == ' ') {
		if (alpha == 0) { return; }

		for (int j = 0; j < 16; j++) {
			for (int i = 0; i < 16; i++) {
				if (xPos >= 0 && xPos < width && yPos >= 0 && yPos < height) {
					pixelScreen = xPos + yPos * width;
					if ((pixelScreen >= 0) && (pixelScreen+1+width) < maxpixel)
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

	int startPos = myfontCharacter(character);
	int line = startPos / 16 * 8;
	int col = startPos % 16 * 8;

	int fontwidth = 128;

	int pixel = 0;
	bool pixelV;

	for (int j = line; j < line + 8; j++) {
		for (int i = col; i < col + 8; i++) {
			pixel = i + j * fontwidth;
			if (xPos >= 0 && xPos < width && yPos >= 0 && yPos < height) {
				pixelScreen = xPos + yPos * width;
				pixelV = fontData[pixel];
				if (!(alpha == 0 && pixelV == 0) && pixelScreen > 0 && pixelScreen+1+width < maxpixel) {
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
