#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _GAME_H
#define _GAME_H

#include "parson.h"
#include "platform_core.h"
#include "fixpoint.h"

#include <stdlib.h>
#include <time.h>
#include <array>

#ifndef MYFONT_H_
#include "myfont.hpp"
#endif

#ifndef MYPIGFONT_H_
#include "mypigfont.hpp"
#endif

#include "TJpg_Decoder.h"

#include <stdlib.h>
#include <time.h>
#include "secrets.h"

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

static void initOTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print("Attempting to connect to network, SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(10000);
  }

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
#endif

static unsigned long lastTimeInMillis = 0;
static unsigned long currentTimeInMillis = 0;
static unsigned long frameTimeInMillis = 0;

static bool esp32gameon_debug_fps = false;
static bool esp32gameon_debug_fps_serial = false;
static bool esp32gameon_debug_output = false;
static unsigned long esp32gameon_debounce = 0;
static uint8_t esp32gameon_debug_counter = 0;

static float voltageF = 0.0;


// First Time Boot missing badge.json (assign a UID, BT_ADDR)
struct BadgeState
{
	char *bt_addr;
	char *name;
	char *ssid;
	char *password;
	bool customBoot = false;
	int bootMenuItem;
};

// Game Buffer
#define SCREEN_WIDTH 135;
#define SCREEN_HEIGHT 240;

struct GameBuff
{
	static const int WIDTH = SCREEN_WIDTH;
	static const int HEIGHT = SCREEN_HEIGHT;
	static const int MAXPIXEL = WIDTH * HEIGHT;
	uint8_t *consoleBuffer = nullptr;
	PlayerKeys playerKeys;
	BadgeState *badgeState = nullptr;
	bool rotate = false;
	unsigned long timeInMillis = 0;

	int gameMode = 0;
	int maxGameMode = 8;

	bool enter = false;
	bool firstRun = true;
};

// Object dimensions
struct Dimensions
{
	int x;
	int y;
	int width;
	int height;
};

#include "myGame.hpp"

void gameInit();
void gameSetup();
void gameLoop();
char *gameLoadFile(char *fileName);
bool gameSaveFile(char *fileName, char *data);

static bool processKey(byte buttonVals, int key);
static bool rectCollisionCheck(Dimensions rect1, Dimensions rect2);
static bool maskCollisionCheck(Dimensions rect1, Dimensions rect2, const bool *object1, const bool *object2);
static bool maskCollisionCheck(Dimensions rect1, Dimensions rect2, const bool *object1, const uint8_t *object2);
static bool maskCollisionCheck(Dimensions rect1, Dimensions rect2, const uint8_t *object1, const uint8_t *object2);
static void displayClear(GameBuff *gameBuff, uint8_t colour);
static void drawObject(GameBuff *gameBuff, Dimensions dim, const uint8_t *objectArray, int alpha);
static void drawObject(GameBuff *gameBuff, int x, int y, int width, int height, int tile, int linelength, const uint8_t *objectArray, int alpha);
bool displayImage(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
bool displayImageInfinite(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
static void drawScreenDouble(GameBuff *gameBuff);
static void drawObjectDouble(GameBuff *gameBuff, Dimensions dim, const uint8_t *objectArray, int alpha);
static void drawBlock(GameBuff *gameBuff, Dimensions dim, uint8_t colour);
static void rotateObject(GameBuff *gameBuff, Dimensions dim, double angle, double zoom, const uint8_t *object, uint8_t backgroundColour);
static void rotateObject(Dimensions dim, double angle, double zoom, const uint8_t *object, uint8_t *rotated, uint8_t backgroundColour);
static void flipObject(Dimensions dim, const uint8_t *objectArray, uint8_t *newObjectArray);
static void drawVertLine2(GameBuff *gameBuff, int x, int y1, int y2, uint8_t colour);
static void drawCharacter(GameBuff *gameBuff, char charPos, int x, int y, int colour, int alpha);
static void drawString(GameBuff *gameBuff, char *scrollText, int x, int y, int colour, int alpha);
static void drawCharacter2x(GameBuff *gameBuff, char charPos, int x, int y, int colour, int alpha);
static void drawString2x(GameBuff *gameBuff, char *scrollText, int x, int y, int colour, int alpha);
static void drawPigString(GameBuff *gameBuff, char *scrollText, int x, int y, int colour, int alpha);
static void drawPigString2x(GameBuff *gameBuff, char *scrollText, int x, int y, int colour, int alpha);
static uint8_t MakeColor(uint8_t r, uint8_t g, uint8_t b);
static uint8_t MakeColor2(uint8_t r, uint8_t g, uint8_t b);
static uint8_t MakeColor565(uint8_t r, uint8_t g, uint8_t b);
static double currentFPS();
static void calcFPS();
static void drawFPS(GameBuff *gameBuff);
static void setCurrentTime();
static void initTime();
static void updateMinTime(unsigned int sleepMiliseconds);
static bool checkTime(unsigned int Seconds);
static int getElapsedSeconds();
static int getElapsedMillis();
static void showImage(const unsigned char *image, int image_size, GameBuff *gameBuff);
static void showLogo(const uint8_t logo[], GameBuff *gameBuff);
static int bitExtracted(int number, int k, int p);
static bool fadeToBlack(GameBuff *gameBuff);
static bool fadeToWhite(GameBuff *gameBuff);
void saveBadgeSettings(BadgeState *state);
BadgeState *loadBadgeSettings();

// Input
static bool processKey(byte buttonVals, int key)
{
	return buttonVals & (1 << key);
}

static bool rectCollisionCheck(Dimensions rect1, Dimensions rect2)
{
	if (rect1.x < rect2.x + rect2.width &&
		rect1.x + rect1.width > rect2.x &&
		rect1.y < rect2.y + rect2.height &&
		rect1.y + rect1.height > rect2.y)
	{
		return true;
	}
	return false;
}

static bool maskCollisionCheck(Dimensions rect1, Dimensions rect2, const bool *object1, const bool *object2)
{
	for (int x1 = rect1.x; x1 < rect1.x + rect1.width; x1++)
		for (int x2 = rect2.x; x2 < rect2.x + rect2.width; x2++)
			if (x2 == x1)
				for (int y1 = rect1.y; y1 < rect1.y + rect1.height; y1++)
					for (int y2 = rect2.y; y2 < rect2.y + rect2.height; y2++)
						if (y2 == y1)
						{
							int y1Loc = (y1 - rect1.y) * rect1.width;
							int y2Loc = (y2 - rect2.y) * rect2.width;
							if (1 == object1[x1 - rect1.x + y1Loc] && object1[x1 - rect1.x + y1Loc] == object2[x2 - rect2.x + y2Loc])
							{
								return true;
							}
						}
	return false;
}

static bool maskCollisionBitCheck(Dimensions rect1, Dimensions rect2, const bool *object1, const bool *object2)
{
	int y1Loc;
	int y2Loc;
	int x1;
	int x2;
	int y1;
	int y2;

	for (x1 = rect1.x; x1 < rect1.x + rect1.width; x1++)
		for (x2 = rect2.x; x2 < rect2.x + rect2.width; x2++)
			if (x2 == x1)
				for (y1 = rect1.y; y1 < rect1.y + rect1.height; y1++)
					for (y2 = rect2.y; y2 < rect2.y + rect2.height; y2++)
						if (y2 == y1)
						{
							y1Loc = (y1 - rect1.y) * rect1.width;
							y2Loc = (y2 - rect2.y) * rect2.width;
							if (0 != object1[x1 - rect1.x + y1Loc] && 0 != object2[x2 - rect2.x + y2Loc])
							{
								return true;
							}
						}
	return false;
}

static bool maskCollisionCheck(Dimensions rect1, Dimensions rect2, const uint8_t *object1, const uint8_t *object2)
{
	for (int x1 = rect1.x; x1 < rect1.x + rect1.width; x1++)
		for (int x2 = rect2.x; x2 < rect2.x + rect2.width; x2++)
			if (x2 == x1)
				for (int y1 = rect1.y; y1 < rect1.y + rect1.height; y1++)
					for (int y2 = rect2.y; y2 < rect2.y + rect2.height; y2++)
						if (y2 == y1)
						{
							int y1Loc = (y1 - rect1.y) * rect1.width;
							int y2Loc = (y2 - rect2.y) * rect2.width;
							if (0 != object1[x1 - rect1.x + y1Loc] && 0 != object2[x2 - rect2.x + y2Loc])
							{
								return true;
							}
						}
	return false;
}

static void displayClear(GameBuff *gameBuff, uint8_t colour)
{
	for (int y = 0; y < gameBuff->HEIGHT; ++y)
		for (int x = 0; x < gameBuff->WIDTH; ++x)
			gameBuff->consoleBuffer[x + gameBuff->WIDTH * y] = colour;
}

static bool drawScroller(GameBuff *gameBuff, char **scrollerText, int topLine)
{
	int currLine = topLine;
	for (int line = 0; line < 9; line++)
	{
		drawString2x(gameBuff, scrollerText[line], 0, gameBuff->HEIGHT - currLine, MakeColor(currLine / 32, currLine / 32, currLine / 64), 0x00);
		currLine -= 16;
	}

	return topLine < 400;
}

static void drawObjectScrollLoop(GameBuff *gameBuff, Dimensions dim, Dimensions image, const uint8_t *objectArray, int alpha)
{
	int counter = 0;
	for (int y = 0; y < dim.height - dim.y; y++)
	{
		for (int x = 0; x < dim.width - dim.x; x++)
		{
			int pixelImage = (y + image.y) * image.width + ((x + image.x) % image.width);
			int pixelScreen = (y + dim.y) * gameBuff->WIDTH + (x + dim.x);
			if (pixelScreen < gameBuff->MAXPIXEL && (objectArray[pixelImage] != alpha))
				gameBuff->consoleBuffer[pixelScreen] = objectArray[pixelImage];
		}
	}
}

static void drawObject(GameBuff *gameBuff, Dimensions dim, const uint8_t *objectArray, int alpha, bool mask[])
{
	int counter = 0;
	for (int j = dim.y; j < dim.y + dim.height; j++)
	{
		int dimXmod = dim.x <= 0 ? 1 : dim.x;
		int firstLine = (dimXmod + gameBuff->WIDTH * j) / gameBuff->WIDTH;
		for (int i = dim.x; i < dim.x + dim.width; i++)
		{
			int pixel = i + gameBuff->WIDTH * j;
			if (pixel >= 0 && pixel < gameBuff->MAXPIXEL && firstLine == pixel / gameBuff->WIDTH)
			{
				if (objectArray[counter] != alpha)
				{
					mask[counter] = 1;
					gameBuff->consoleBuffer[pixel] = objectArray[counter];
				}
				else
				{
					mask[counter] = 0;
				}
			}
			counter++;
		}
	}
}

static void drawObject(GameBuff *gameBuff, int x, int y, int width, int height, int tile, int linelength, const uint8_t *objectArray, int alpha)
{
	int counter = 0;
	int bitmapPixel = 0;

	for (int j = 0; j < height; j++) {
		int dimXmod = x <= 0 ? 1 : x;
		for (int i = 0; i < width; i++) {
			int pixel = x + i + (gameBuff->WIDTH * (y + j));
			if (pixel >= 0 && pixel < gameBuff->MAXPIXEL && y + j == pixel / gameBuff->WIDTH) {
				bitmapPixel = tile * 32 + i + j * linelength;
				if (objectArray[bitmapPixel] != alpha)
					gameBuff->consoleBuffer[pixel] = objectArray[bitmapPixel];
			}
		}
	}
}

static void drawObject(GameBuff *gameBuff, Dimensions dim, const uint8_t *objectArray, int alpha)
{
	int counter = 0;
	for (int j = dim.y; j < dim.y + dim.height; j++)
	{
		int dimXmod = dim.x <= 0 ? 1 : dim.x;
		int firstLine = (dimXmod + gameBuff->WIDTH * j) / gameBuff->WIDTH;
		for (int i = dim.x; i < dim.x + dim.width; i++)
		{
			int pixel = i + gameBuff->WIDTH * j;
			if (pixel >= 0 && pixel < gameBuff->MAXPIXEL && firstLine == pixel / gameBuff->WIDTH)
			{
				if (objectArray[counter] != alpha)
					gameBuff->consoleBuffer[pixel] = objectArray[counter];
			}
			counter++;
		}
	}
}

// Writes directly to the buffer, no collision overrun detection, use with caution, only for logos currently.
static void drawObjectRLE(GameBuff *gameBuff, Dimensions dim, const uint8_t *objectArray, int size, int alpha)
{
	int counter = 0;
	int fullCount = dim.width * dim.height;

	int arrayIndex = 0;
	uint8_t repeat = 0;
	uint8_t value = 0;
	while (arrayIndex < size)
	{
		repeat = objectArray[arrayIndex++];
		value = objectArray[arrayIndex++];
		for (int i = 0; i < repeat; i++)
		{
			gameBuff->consoleBuffer[counter++] = value;
		}
	}
}

static void drawObject(GameBuff *gameBuff, Dimensions dim, const bool *objectArray, uint8_t cZero, uint8_t cOne, int alpha)
{
	int counter = 0;
	for (int j = dim.y; j < dim.y + dim.height; j++)
	{
		int dimXmod = dim.x <= 0 ? 1 : dim.x;
		int firstLine = (dimXmod + gameBuff->WIDTH * j) / gameBuff->WIDTH;
		for (int i = dim.x; i < dim.x + dim.width; i++)
		{
			int pixel = i + gameBuff->WIDTH * j;
			if (pixel >= 0 && pixel < gameBuff->MAXPIXEL && firstLine == pixel / gameBuff->WIDTH)
			{
				if (objectArray[counter] ? cOne : cZero != alpha)
					gameBuff->consoleBuffer[pixel] = objectArray[counter] ? cOne : cZero;
			}
			counter++;
		}
	}
}

// Screen Double 120x120 -> 240x240
static void drawScreenDouble(GameBuff *gameBuff)
{
	uint8_t currentPixelValue;
	int pixel = 0;
	int screenX = 0;
	int screenY = 0;
	int y = 0;
	int x = gameBuff->WIDTH/2 - 1;
	int twoWidth = gameBuff->WIDTH*2;
	int startHeightY = (gameBuff->HEIGHT/2 - 1) * gameBuff->WIDTH;
	int startHeightScreenY = (gameBuff->HEIGHT - 1) * gameBuff->WIDTH;

	while (x >= 0) {
		screenX = x * 2 + 1;
		y = startHeightY;
		screenY = startHeightScreenY;

		while (y >= 0) {
			currentPixelValue = gameBuff->consoleBuffer[x + y];

			pixel = screenX + screenY;
			gameBuff->consoleBuffer[pixel] = currentPixelValue;
			gameBuff->consoleBuffer[pixel - 1] = currentPixelValue;
			gameBuff->consoleBuffer[pixel - gameBuff->WIDTH] = currentPixelValue;
			gameBuff->consoleBuffer[pixel - 1 - gameBuff->WIDTH] = currentPixelValue;

			y -= gameBuff->WIDTH;
			screenY -= twoWidth;
		}

		x--;
	}
}

static void drawObjectDouble(GameBuff *gameBuff, Dimensions dim, const bool *objectArray, uint8_t cZero, uint8_t cOne, int alpha)
{
	int counter = 0;
	for (int j = dim.y; j < dim.y + dim.height * 2; j += 2)
	{
		int dimXmod = dim.x <= 0 ? 1 : dim.x;
		int firstLine = (dimXmod + gameBuff->WIDTH * j) / gameBuff->WIDTH;
		for (int i = dim.x; i < dim.x + dim.width * 2; i += 2)
		{
			int pixel = i + gameBuff->WIDTH * j;
			if (objectArray[counter] ? cOne : cZero != alpha)
			{
				if (pixel < gameBuff->MAXPIXEL && firstLine == pixel / gameBuff->WIDTH)
				{
					gameBuff->consoleBuffer[pixel] = objectArray[counter] ? cOne : cZero;
				}
				if ((pixel + 1) < gameBuff->MAXPIXEL && firstLine == (pixel + 1) / gameBuff->WIDTH)
				{
					gameBuff->consoleBuffer[pixel + 1] = objectArray[counter] ? cOne : cZero;
				}
				if ((pixel + gameBuff->WIDTH) < gameBuff->MAXPIXEL)
				{
					gameBuff->consoleBuffer[pixel + gameBuff->WIDTH] = objectArray[counter] ? cOne : cZero;
				}
				if ((pixel + 1 + gameBuff->WIDTH) < gameBuff->MAXPIXEL)
				{
					gameBuff->consoleBuffer[pixel + 1 + gameBuff->WIDTH] = objectArray[counter] ? cOne : cZero;
				}
			}
			counter++;
		}
	}
}

static void drawObjectDouble(GameBuff *gameBuff, Dimensions dim, const uint8_t *objectArray, int alpha)
{
	int counter = 0;
	for (int j = dim.y; j < dim.y + dim.height * 2; j += 2)
	{
		int dimXmod = dim.x <= 0 ? 1 : dim.x;
		int firstLine = (dimXmod + gameBuff->WIDTH * j) / gameBuff->WIDTH;
		for (int i = dim.x; i < dim.x + dim.width * 2; i += 2)
		{
			int pixel = i + gameBuff->WIDTH * j;
			if (objectArray[counter] != alpha)
			{
				if (pixel < gameBuff->MAXPIXEL && firstLine == pixel / gameBuff->WIDTH)
				{
					gameBuff->consoleBuffer[pixel] = objectArray[counter];
				}
				if ((pixel + 1) < gameBuff->MAXPIXEL && firstLine == (pixel + 1) / gameBuff->WIDTH)
				{
					gameBuff->consoleBuffer[pixel + 1] = objectArray[counter];
				}
				if ((pixel + gameBuff->WIDTH) < gameBuff->MAXPIXEL)
				{
					gameBuff->consoleBuffer[pixel + gameBuff->WIDTH] = objectArray[counter];
				}
				if ((pixel + 1 + gameBuff->WIDTH) < gameBuff->MAXPIXEL)
				{
					gameBuff->consoleBuffer[pixel + 1 + gameBuff->WIDTH] = objectArray[counter];
				}
			}
			counter++;
		}
	}
}

static void drawBlock(GameBuff *gameBuff, Dimensions dim, uint8_t colour, uint8_t style)
{
	switch (style) {
		case 0: 
			// Solid Fill
			drawBlock(gameBuff, dim, colour);
			break;
		case 1: 
			// Borderfill
			for (int x = dim.x; x <= dim.x + dim.width; x++) {
				if (x < gameBuff->WIDTH && (x + dim.y * gameBuff->WIDTH < gameBuff->MAXPIXEL)) {
					gameBuff->consoleBuffer[x + dim.y * gameBuff->WIDTH] = colour;
				}

				if (x < gameBuff->WIDTH && (x + (dim.y + dim.height) * gameBuff->WIDTH < gameBuff->MAXPIXEL)) {
					gameBuff->consoleBuffer[x + (dim.y + dim.height) * gameBuff->WIDTH] = colour;
				}
			}

			for (int y = dim.y; y <= dim.y + dim.height; y += 1) {
				if (dim.x + y * gameBuff->WIDTH < gameBuff->MAXPIXEL) {
					gameBuff->consoleBuffer[dim.x + y * gameBuff->WIDTH] = colour;
				}

				if (dim.x + (y * gameBuff->WIDTH) + dim.width < gameBuff->MAXPIXEL) {
					gameBuff->consoleBuffer[dim.x + (y * gameBuff->WIDTH) + dim.width] = colour;
				}
			}
		break;
	}
}

static void drawBlock(GameBuff *gameBuff, Dimensions dim, uint8_t colour)
{
	for (int j = dim.y; j <= dim.y + dim.height; j++)
	{
		int firstLine = (dim.x + gameBuff->WIDTH * j) / gameBuff->WIDTH;
		for (int i = dim.x; i <= dim.x + dim.width; i++)
		{
			int pixel = i + gameBuff->WIDTH * j;
			if (pixel < gameBuff->MAXPIXEL && firstLine == pixel / gameBuff->WIDTH && j > 0 && j < gameBuff->HEIGHT && i > 0 && i < gameBuff->WIDTH)
			{
				gameBuff->consoleBuffer[pixel] = colour;
			}
		}
	}
}

static void rotateObject(GameBuff *gameBuff, Dimensions dim, double angle, double zoom, const uint8_t *object)
{
	int xt;
	int xs;

	int yt;
	int ys;

	int xOffset = dim.x;
	int yOffset = dim.y;

	FIXPOINT cosmax;
	FIXPOINT sinmax;

	FIXPOINT sinma = FLOAT_TO_FIXP(sin(-angle) * zoom);
	FIXPOINT cosma = FLOAT_TO_FIXP(cos(-angle) * zoom);

	int hwidth = dim.width / 2;
	int hheight = dim.height / 2;

	for (int x = 0; x < dim.width; x++)
	{
		xt = x - hwidth;
		cosmax = cosma * xt;
		sinmax = sinma * xt;

		for (int y = 0; y < dim.height; y++)
		{
			yt = y - hheight;

			xs = FIXP_INT_PART((cosmax - (sinma * yt))) + hwidth;
			ys = FIXP_INT_PART((sinmax + (cosma * yt))) + hheight;
			int pixel = (xOffset + x) + ((yOffset + y) * gameBuff->WIDTH);
			if (xs >= 0 && xs < dim.width && ys >= 0 && ys < dim.height && pixel < gameBuff->MAXPIXEL && pixel >= 0)
			{
				gameBuff->consoleBuffer[pixel] = object[xs + ys * dim.width];
			}
		}
	}
}

static void rotateObjectAlpha(GameBuff *gameBuff, Dimensions dim, double angle, double zoom, const uint8_t *object, uint8_t alphaColour, bool mask[])
{
	int xt;
	int xs;

	int yt;
	int ys;

	int xOffset = dim.x;
	int yOffset = dim.y;

	FIXPOINT cosmax;
	FIXPOINT sinmax;

	FIXPOINT sinma = FLOAT_TO_FIXP(sin(-angle) * zoom);
	FIXPOINT cosma = FLOAT_TO_FIXP(cos(-angle) * zoom);

	int hwidth = dim.width / 2;
	int hheight = dim.height / 2;

	for (int x = 0; x < dim.width; x++)
	{
		xt = x - hwidth;
		cosmax = cosma * xt;
		sinmax = sinma * xt;

		for (int y = 0; y < dim.height; y++)
		{
			yt = y - hheight;

			xs = FIXP_INT_PART((cosmax - (sinma * yt))) + hwidth;
			ys = FIXP_INT_PART((sinmax + (cosma * yt))) + hheight;
			int pixel = (xOffset + x) + ((yOffset + y) * gameBuff->WIDTH);
			int maskPixel = x + y * dim.width;

			if (xs >= 0 && xs < dim.width && ys >= 0 && ys < dim.height && pixel < gameBuff->MAXPIXEL && pixel >= 0)
			{
				uint8_t pixelColour = object[xs + ys * dim.width];
				if (pixelColour != alphaColour)
				{
					mask[maskPixel] = 1;
					gameBuff->consoleBuffer[pixel] = pixelColour;
				}
				else
				{
					mask[maskPixel] = 0;
				}
			}
		}
	}
}

static void rotateObjectAlpha(GameBuff *gameBuff, Dimensions dim, double angle, double zoom, const uint8_t *object, uint8_t alphaColour)
{
	int xt;
	int xs;

	int yt;
	int ys;

	int xOffset = dim.x;
	int yOffset = dim.y;

	FIXPOINT cosmax;
	FIXPOINT sinmax;

	FIXPOINT sinma = FLOAT_TO_FIXP(sin(-angle) * zoom);
	FIXPOINT cosma = FLOAT_TO_FIXP(cos(-angle) * zoom);

	int hwidth = dim.width / 2;
	int hheight = dim.height / 2;

	for (int x = 0; x < dim.width; x++)
	{
		xt = x - hwidth;
		cosmax = cosma * xt;
		sinmax = sinma * xt;

		for (int y = 0; y < dim.height; y++)
		{
			yt = y - hheight;

			xs = FIXP_INT_PART((cosmax - (sinma * yt))) + hwidth;
			ys = FIXP_INT_PART((sinmax + (cosma * yt))) + hheight;
			int pixel = (xOffset + x) + ((yOffset + y) * gameBuff->WIDTH);
			uint8_t pixelColour = object[xs + ys * dim.width];
			if (pixelColour != alphaColour)
				if (xs >= 0 && xs < dim.width && ys >= 0 && ys < dim.height && pixel < gameBuff->MAXPIXEL && pixel >= 0)
				{
					gameBuff->consoleBuffer[pixel] = pixelColour;
				}
		}
	}
}

static void rotateObject(GameBuff *gameBuff, Dimensions dim, double angle, double zoom, const uint8_t *object, uint8_t backgroundColour)
{
	int xt;
	int xs;

	int yt;
	int ys;

	int xOffset = dim.x;
	int yOffset = dim.y;

	FIXPOINT cosmax;
	FIXPOINT sinmax;

	FIXPOINT sinma = FLOAT_TO_FIXP(sin(-angle) * zoom);
	FIXPOINT cosma = FLOAT_TO_FIXP(cos(-angle) * zoom);

	int hwidth = dim.width / 2;
	int hheight = dim.height / 2;

	for (int x = 0; x < dim.width; x++)
	{
		xt = x - hwidth;
		cosmax = cosma * xt;
		sinmax = sinma * xt;

		for (int y = 0; y < dim.height; y++)
		{
			yt = y - hheight;

			xs = FIXP_INT_PART((cosmax - (sinma * yt))) + hwidth;
			ys = FIXP_INT_PART((sinmax + (cosma * yt))) + hheight;
			int pixel = (xOffset + x) + ((yOffset + y) * gameBuff->WIDTH);
			if (pixel < gameBuff->MAXPIXEL && pixel >= 0)
				if (xs >= 0 && xs < dim.width && ys >= 0 && ys < dim.height)
				{
					gameBuff->consoleBuffer[pixel] = object[xs + ys * dim.width];
				}
				else 
				{
					gameBuff->consoleBuffer[pixel] = backgroundColour;
				}
		}
	}
}

static void rotateObject(Dimensions dim, double angle, double zoom, const uint8_t *object, uint8_t *rotated, uint8_t backgroundColour)
{
	int xt;
	int xs;

	int yt;
	int ys;

	FIXPOINT cosmax;
	FIXPOINT sinmax;

	FIXPOINT sinma = FLOAT_TO_FIXP(sin(-angle) * zoom);
	FIXPOINT cosma = FLOAT_TO_FIXP(cos(-angle) * zoom);

	int hwidth = dim.width / 2;
	int hheight = dim.height / 2;

	for (int x = 0; x < dim.width; x++)
	{
		xt = x - hwidth;
		cosmax = cosma * xt;
		sinmax = sinma * xt;

		for (int y = 0; y < dim.height; y++)
		{
			yt = y - hheight;

			xs = FIXP_INT_PART((cosmax - (sinma * yt))) + hwidth;
			ys = FIXP_INT_PART((sinmax + (cosma * yt))) + hheight;

			if (xs >= 0 && xs < dim.width && ys >= 0 && ys < dim.height)
			{
				rotated[x + y * dim.width] = object[xs + ys * dim.width];
			}
			else
			{
				rotated[x + y * dim.width] = backgroundColour;
			}
		}
	}
}

static void flipObject(Dimensions dim, const uint8_t *objectArray, uint8_t *newObjectArray)
{
	int counter = 0;
	for (int j = dim.height - 1; j >= 0; j--)
	{
		for (int i = dim.width - 1; i >= 0; i--)
		{
			int pixel = i + dim.width * j;
			newObjectArray[counter] = objectArray[pixel];
			counter++;
		}
	}
}

static void drawHorizLine2(GameBuff *gameBuff, int y, int x1, int x2, uint8_t colour)
{
	if (x1 < 0)
		x1 = 0;
	if (x1 > x2)
		return;
	int pixel = x1 + gameBuff->WIDTH * y;
	int end = x2 + gameBuff->WIDTH * y;
	if (end > gameBuff->MAXPIXEL)
		end = (gameBuff->HEIGHT - 1) * gameBuff->WIDTH + x2;
	while (pixel <= end)
	{
		gameBuff->consoleBuffer[pixel] = colour;
		++pixel;
	}
}

static void drawVertLine2(GameBuff *gameBuff, int x, int y1, int y2, uint8_t colour)
{
	if (y1 < 0)
		y1 = 0;
	if (y1 > y2)
		return;
	int pixel = x + gameBuff->WIDTH * y1;
	int end = x + gameBuff->WIDTH * y2;
	if (end > gameBuff->MAXPIXEL)
		end = (gameBuff->HEIGHT - 1) * gameBuff->WIDTH + x;
	while (pixel <= end)
	{
		gameBuff->consoleBuffer[pixel] = colour;
		pixel += gameBuff->WIDTH;
	}
}

static void drawLine(GameBuff *gameBuff, int x1, int x2, int y1, int y2, uint8_t colour ) {
	if (x1 == x2) // VertLine 
	{
		drawVertLine2(gameBuff,x1,y1,y2,colour);
	} else if (y1 == y2) {
		drawHorizLine2(gameBuff,y1,x1,x2,colour);
	}
}

static Dimensions FontDim = {0, 0, 8, 8};

static void drawCharacter(GameBuff *gameBuff, char charPos, int x, int y, int colour, int alpha)
{
	drawFont(gameBuff->consoleBuffer, gameBuff->WIDTH, gameBuff->HEIGHT, gameBuff->MAXPIXEL, charPos, colour, alpha, x, y);
}

static void drawString(GameBuff *gameBuff, char *scrollText, int x, int y, int colour, int alpha)
{
	for (int i = 0; i < static_cast<int>(strlen(scrollText)); i++)
	{
    drawCharacter(gameBuff, scrollText[i], x, y, colour, alpha);

    if(scrollText[i] == ':') {
      x += 1;
    } else {
      x += 8;
    }
	}
}

static void drawCharacter2x(GameBuff *gameBuff, char charPos, int x, int y, int colour, int alpha)
{
	drawFont2x(gameBuff->consoleBuffer, gameBuff->WIDTH, gameBuff->HEIGHT, gameBuff->MAXPIXEL, charPos, colour, alpha, x, y);
}

static void drawString2x(GameBuff *gameBuff, char *scrollText, int x, int y, int colour, int alpha)
{
	for (int i = 0; i < static_cast<int>(strlen(scrollText)); i++)
	{
    drawCharacter2x(gameBuff, scrollText[i], x, y, colour, alpha);

    if(scrollText[i] == ':') {
      x += 2;
    } else {
      x += 16;
    }
	}
}

static void drawPigString(GameBuff *gameBuff, char *scrollText, int x, int y, int colour, int alpha)
{
	for (int i = 0; i < static_cast<int>(strlen(scrollText)); i++)
	{
		drawPigFont(gameBuff->consoleBuffer, gameBuff->HEIGHT, gameBuff->WIDTH, gameBuff->MAXPIXEL, scrollText[i], colour, alpha, x + 8 * i, y);
	}
}

static void drawPigString2x(GameBuff *gameBuff, char *scrollText, int x, int y, int colour, int alpha)
{
	for (int i = 0; i < static_cast<int>(strlen(scrollText)); i++)
	{
		drawPigFont2x(gameBuff->consoleBuffer, gameBuff->HEIGHT, gameBuff->WIDTH, gameBuff->MAXPIXEL, scrollText[i], colour, alpha, x + 16 * i, y);
	}
}

static FIXPOINT fpsArray[10];
static int fpsItem = 0;
static int fpsItems = 0;
static int fpsMaxItems = 10;
static FIXPOINT thousand = INT_TO_FIXP(1000);

static double currentFPS()
{
	// Calculate Moving Average Time
	FIXPOINT averageFPS = 0;

	for (int i = 0; i < fpsItems; i++)
	{
		averageFPS += fpsArray[i];
	}

	if (fpsItems > 0)
		averageFPS /= fpsItems;

	return FIXP_TO_FLOAT(FIXP_DIV(thousand, averageFPS));
}

static void calcFPS()
{
	// Move pointer forwards
	fpsItem += 1;
	if (fpsItems < fpsMaxItems)
	{
		fpsItems += 1;
	}

	// If we have hit the max items loop around
	if (fpsItem == fpsMaxItems)
	{
		fpsItem = 0;
	}

	fpsTimer2 = fpsTimer1;
	fpsTimer1 = getTimeInMillis();

	// Calc Diff MS
	timeDiff = fpsTimer1 - fpsTimer2;
	if (timeDiff > 0)
	{
		// Add Item to Array
		fpsArray[fpsItem] = INT_TO_FIXP(timeDiff);
	}
}

static char fpsString[50];
static void drawFPS(GameBuff *gameBuff)
{
#ifdef ESP32
	sprintf(fpsString, "%3.2f fps %d - %s", currentFPS(), ESP.getFreeHeap(), getDevicePlatform());
#elif _WIN32
	sprintf_s(fpsString, "%3.2f fps - %s", currentFPS(), getDevicePlatform());
#else
	sprintf(fpsString, "%3.2f fps - %s", currentFPS(), getDevicePlatform());
#endif
	drawString(gameBuff, fpsString, 0, gameBuff->HEIGHT-8, 0xFF, 0);
}

static void drawVoltage(GameBuff *gameBuff)
{
#ifdef ESP32
	sprintf(fpsString,"Voltage : %3.2fV",voltageF);
#elif _WIN32
	sprintf_s(fpsString,"Voltage : %3.2fV",voltageF);
#else
	sprintf(fpsString,"Voltage : %3.2fV",voltageF);
#endif
	drawString(gameBuff, fpsString, 0, gameBuff->HEIGHT-16, 0xFF, 0);
}

static void setCurrentTime()
{
	frameTime = currentTime;
	currentTime = getTimeInMillis();
}

static void initTime()
{
	setCurrentTime();
	startTime = currentTime;
	frameTime = currentTime;
}

static void updateMinTime(unsigned int sleepMiliseconds)
{
	setCurrentTime();

	if (currentTime - frameTime < sleepMiliseconds)
	{
		gameSleep(sleepMiliseconds - (currentTime - frameTime));
	}

	setCurrentTime();
}

static bool checkTime(unsigned int Seconds)
{
	return (getTimeInMillis() - startTime > Seconds * 1000);
}

static int getElapsedSeconds()
{
	return (int)((getTimeInMillis() - startTime) / 1000);
}

static int getElapsedMillis()
{
	return (int)(getTimeInMillis() - startTime);
}

static void showLogo(const uint8_t logo[], GameBuff *gameBuff)
{
	Dimensions dim;
	dim.height = gameBuff->HEIGHT;
	dim.width = gameBuff->WIDTH;
	dim.x = 0;
	dim.y = 0;

	displayClear(gameBuff, 0x00);
	drawObject(gameBuff, dim, logo, -1);
	initTime();
}

static void showLogoRLE(const uint8_t logo[], const int size, GameBuff *gameBuff)
{
	Dimensions dim;
	dim.height = gameBuff->HEIGHT;
	dim.width = gameBuff->WIDTH;
	dim.x = 0;
	dim.y = 0;

	displayClear(gameBuff, 0x00);
	drawObjectRLE(gameBuff, dim, logo, size, -1);
	initTime();
}

static uint8_t MakeColor(uint8_t r, uint8_t g, uint8_t b)
{
	return (r << 5) | (g << 2) | b;
}

static uint8_t MakeColor2(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r / 32) << 5) | ((g / 32) << 2) | (b / 64);
}

static uint8_t MakeColor565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r / 4) << 5) | ((g / 9) << 2) | (b / 10);
}

static int bitExtracted(int number, int k, int p)
{
	return (((1 << k) - 1) & (number >> (p - 1)));
}

static bool fadeToBlack(GameBuff *gameBuff)
{
	int r = 0;
	int g = 0;
	int b = 0;

	int pixel = 0;
	bool change = false;
	while (pixel < gameBuff->MAXPIXEL)
	{
		if (gameBuff->consoleBuffer[pixel] > 0)
		{
			r = bitExtracted(gameBuff->consoleBuffer[pixel], 3, 6);
			g = bitExtracted(gameBuff->consoleBuffer[pixel], 3, 3);
			b = bitExtracted(gameBuff->consoleBuffer[pixel], 2, 1);

			if (r > 0)
			{
				r -= 1;
				change = true;
				gameBuff->consoleBuffer[pixel] = MakeColor(r, g, b);
			}
			else if (g > 0)
			{
				g -= 1;
				change = true;
				gameBuff->consoleBuffer[pixel] = MakeColor(r, g, b);
			}
			else if (b > 0)
			{
				b -= 1;
				change = true;
				gameBuff->consoleBuffer[pixel] = MakeColor(r, g, b);
			}
		}

		pixel += 1;
	}

	return !change;
}

static bool fadeToWhite(GameBuff *gameBuff)
{
	int r = 0;
	int g = 0;
	int b = 0;

	int pixel = 0;
	bool change = false;
	while (pixel < gameBuff->MAXPIXEL)
	{
		if (gameBuff->consoleBuffer[pixel] < 0xFF)
		{
			r = bitExtracted(gameBuff->consoleBuffer[pixel], 3, 6);
			g = bitExtracted(gameBuff->consoleBuffer[pixel], 3, 3);
			b = bitExtracted(gameBuff->consoleBuffer[pixel], 2, 1);

			if (r < 7)
			{
				r += 1;
				change = true;
				gameBuff->consoleBuffer[pixel] = MakeColor(r, g, b);
			}
			else if (g < 7)
			{
				g += 1;
				change = true;
				gameBuff->consoleBuffer[pixel] = MakeColor(r, g, b);
			}
			else if (b < 3)
			{
				b += 1;
				change = true;
				gameBuff->consoleBuffer[pixel] = MakeColor(r, g, b);
			}
		}

		pixel += 1;
	}

	return !change;
}

#endif
