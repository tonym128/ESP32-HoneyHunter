#include "myGame.hpp"
#include "files/bee_image.h"
#include "files/bee_comb_image.h"
#include "files/bg_river_image.h"
#include "files/flowers_image.h"

float voltageF = 0.0;
struct Flower {
	bool visible;
	FIXPOINT x;
	FIXPOINT y;
	FIXPOINT nectar;
	int type;
};

static const int FLOWER_COUNT = 10;
struct BeeGame {
	FIXPOINT x, y, power, maxpower;
	Flower flowers[FLOWER_COUNT];
	bool onFlower = false;
} beeGame;

bool displayMenu(GameBuff *gameBuff)
{
	int i = 0;
	int x = 32;
	
	for (int i = 0; i < gameBuff->WIDTH; i++) {
		drawVertLine2(gameBuff,i,0,gameBuff->HEIGHT-1,(i +  gameBuff->timeInMillis / 50) % gameBuff->WIDTH);
	}

	gameBuff->maxGameMode = 0;
	drawString(gameBuff, (char *)"   ESP32    ", x, i += 16, 0xFF, 0);
	drawString(gameBuff, (char *)"  GameOn!   ", x, i += 16, 0xFF, 0);
	drawString(gameBuff, (char *)"----------- ", x, i += 16, 0xFF, 0);
	drawString(gameBuff, (char *)"0) Scroll ", x, i += 16, gameBuff->gameMode == gameBuff->maxGameMode++ ? 0x1C : 0xFF, 0);
	drawString(gameBuff, (char *)"1) Animate ", x, i += 16, gameBuff->gameMode == gameBuff->maxGameMode++ ? 0x1C : 0xFF, 0);
	drawString(gameBuff, (char *)"2) Demo ", x, i += 16, gameBuff->gameMode == gameBuff->maxGameMode++ ? 0x1C : 0xFF, 0);
	drawString(gameBuff, (char *)"3) Demo ", x, i += 16, gameBuff->gameMode == gameBuff->maxGameMode++ ? 0x1C : 0xFF, 0);

	gameBuff->maxGameMode--;

	char voltage[30]; 

#ifdef ESP32
	sprintf(voltage,"Voltage : %3.2fV",voltageF);
#elif _WIN32
	sprintf_s(voltage,"Voltage : %3.2fV",voltageF);
#else
	sprintf(voltage,"Voltage : %3.2fV",voltageF);
#endif
	drawString(gameBuff, voltage, 0, gameBuff->HEIGHT-16, gameBuff->gameMode == gameBuff->maxGameMode++ ? 0x1C : 0xFF, 0);
	return true;
}

bool updateMenus(GameBuff *gameBuff)
{
	if (gameBuff->playerKeys.debouncedInput)
	{
		if (gameBuff->playerKeys.down)
		{
			gameBuff->gameMode += 1;
		}

		if (gameBuff->playerKeys.up)
		{
			gameBuff->enter = true;
		}
	}

	// Loop around
	if (gameBuff->gameMode == -1)
	{
		gameBuff->gameMode = gameBuff->maxGameMode;
	}
	else if (gameBuff->gameMode == gameBuff->maxGameMode + 1)
	{
		gameBuff->gameMode = 0;
	}

	return true;
}

unsigned long lastTimeInMillis = 0;
unsigned long currentTimeInMillis = 0;
unsigned long frameTimeInMillis = 0;

void inputBee(GameBuff *gameBuff) {
	currentTimeInMillis = getTimeInMillis();
	frameTimeInMillis = (currentTimeInMillis - lastTimeInMillis);
	if (gameBuff->playerKeys.up) {
		beeGame.x -=  frameTimeInMillis* 4000;
	}

	if (gameBuff->playerKeys.down) {
		beeGame.x += frameTimeInMillis * 4000;
	}

	if (beeGame.x > INT_TO_FIXP(gameBuff->WIDTH-32)) beeGame.x = INT_TO_FIXP(gameBuff->WIDTH-32);
	if (beeGame.x < INT_TO_FIXP(0)) beeGame.x = INT_TO_FIXP(0);
	lastTimeInMillis = currentTimeInMillis;
}

void updateBeeGame(GameBuff *gameBuff) {
	Dimensions dimBee;
	dimBee.height = 32;
	dimBee.width = 32;
	dimBee.x = FIXP_TO_INT(beeGame.x);
	dimBee.y = FIXP_TO_INT(beeGame.y);

	Dimensions dimFlower;
	dimFlower.width = 32;
	dimFlower.height = 32;

	beeGame.onFlower = false;
	for (int i = 0; i < FLOWER_COUNT; i++ ) {
		if (beeGame.flowers[i].visible) {
			dimFlower.x = FIXP_TO_INT(beeGame.flowers[i].x);
			dimFlower.y = FIXP_TO_INT(beeGame.flowers[i].y);
			if (rectCollisionCheck(dimBee,dimFlower)) {
				beeGame.onFlower = true;
			}
		}
	}

	if (beeGame.onFlower) {
		beeGame.power += frameTimeInMillis * 1000;
	}

	beeGame.power -= frameTimeInMillis * 50;

	if (beeGame.power > beeGame.maxpower) beeGame.power = beeGame.maxpower;
	if (beeGame.power < INT_TO_FIXP(0)) beeGame.power = INT_TO_FIXP(0);
}

void animateBee(GameBuff *gameBuff) {
	int startX = (gameBuff->timeInMillis / 100 % 8) * 32;
	int screenXOffset = FIXP_TO_INT(beeGame.x);
	int screenYOffset = FIXP_TO_INT(beeGame.y) * gameBuff->WIDTH;

	int y = 0;
	int x = 0;
	int pixel = 0;

	while (y < 32) {
		pixel = startX + x + y * 256;
		while (x < 32) {
			if (bee_image[pixel] != 0x00)
				gameBuff->consoleBuffer[screenXOffset + x + screenYOffset + y * gameBuff->WIDTH] = bee_image[pixel];
			x++;
			pixel++;
		}
		
		x = 0;
		y++;
	}
}

// Draw directly from uncompressed 332 image bitmap, with some minor optimisations
int startY1 = 0;
void scrollBackground(GameBuff *gameBuff) {
	startY1 = (startY1 + frameTimeInMillis / 30) % 320;
	int startY1Mod = startY1;
	int imageY = 0;
	while (startY1Mod < gameBuff->HEIGHT) {
		for (int i = 0; i < gameBuff->WIDTH; i++) { 
			gameBuff->consoleBuffer[startY1Mod * gameBuff->WIDTH + i] = bg_river_image[imageY * gameBuff->WIDTH + i];
		}

		startY1Mod++;
		imageY++;
	}

	int startY2 = startY1 - 320;
	imageY = 0;
	
	while (startY2 < startY1 && startY2 < gameBuff->HEIGHT) { 
		if (startY2 >= 0)
			for (int i = 0; i < gameBuff->WIDTH; i++) { 
				gameBuff->consoleBuffer[startY2 * gameBuff->WIDTH + i] = bg_river_image[imageY * gameBuff->WIDTH + i];
			}

		startY2++;
		imageY++;
	}
}

void initBeeGame(GameBuff *gameBuff) {
	beeGame.x = INT_TO_FIXP(gameBuff->WIDTH/3);
	beeGame.y = INT_TO_FIXP(gameBuff->HEIGHT - 50);
	beeGame.power = 0;
	beeGame.maxpower = INT_TO_FIXP(gameBuff->WIDTH);
	beeGame.onFlower = false;

	for (int i = 0; i < FLOWER_COUNT; i++)
	{
		beeGame.flowers[i].visible = true;
		beeGame.flowers[i].type = i;
		beeGame.flowers[i].x = INT_TO_FIXP(i * 32 % gameBuff->WIDTH);
		beeGame.flowers[i].y = INT_TO_FIXP(rand() % 30 * -32);
	}
}

void drawFlowers(GameBuff *gameBuff) {
	for (int i = 0; i < FLOWER_COUNT; i++ ) {
		if (beeGame.flowers[i].visible) {
			drawObject(gameBuff,FIXP_TO_INT(beeGame.flowers[i].x), FIXP_TO_INT(beeGame.flowers[i].y),  32, 32, beeGame.flowers[i].type ,192, flowers_image,0xFF);
			beeGame.flowers[i].y += INT_TO_FIXP(frameTimeInMillis / 30);
		}
	}
}
void drawBeeStatus(GameBuff *gameBuff) {
	Dimensions dimScreen, dimImage;
	dimScreen.x = 0;
	dimScreen.y = 0;
	dimScreen.width = FIXP_TO_INT(beeGame.power);
	dimScreen.height = 32;
	dimImage.x = 0;
	dimImage.y = 0;
	dimImage.height = 32;
	dimImage.width = 32;
	drawObjectScrollLoop(gameBuff,dimScreen,dimImage,bee_comb_image,0x00);
}

bool firstRun = true;
bool myGameLoop(GameBuff *gameBuff)
{
	if (gameBuff->playerKeys.up) {
		voltageF = getVoltage();
	}

	displayClear(gameBuff,0x00);
	if (gameBuff->enter)
	{
		switch (gameBuff->gameMode)
		{
		case 0:
			scrollBackground(gameBuff);
			break;
		case 1:
			if (firstRun) {
				initBeeGame(gameBuff);
				firstRun = false;
			}

			inputBee(gameBuff);
			updateBeeGame(gameBuff);
			scrollBackground(gameBuff);
			drawFlowers(gameBuff);
			animateBee(gameBuff);
			drawBeeStatus(gameBuff);
			break;
		}
		return false;
	}
	else
	{
		updateMenus(gameBuff);
		displayMenu(gameBuff);
		return false;
	}
}
