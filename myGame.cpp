#include "myGame.hpp"
#include "files/bg_river.jpg.h"
#include "files/bee_image.h"
#include "files/bg_river_image.h"

float voltageF = 0.0;

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

void animateBee(GameBuff *gameBuff) {
	int startX = (gameBuff->timeInMillis / 100 % 8) * 32;
	int screenXOffset = gameBuff->WIDTH/3;
	int screenYOffset = (gameBuff->HEIGHT - 50) * gameBuff->WIDTH;

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

void scrollBackgroundJpeg(GameBuff *gameBuff) {
	TJpgDec.drawJpg(0, gameBuff->timeInMillis / 50 % 320, bg_river_jpg, sizeof(bg_river_jpg));
	TJpgDec.drawJpg(0, gameBuff->timeInMillis / 50 % 320 - 320, bg_river_jpg, sizeof(bg_river_jpg));
}

void scrollBackground(GameBuff *gameBuff) {
	int startY1 = gameBuff->timeInMillis / 50 % 320;
	int imageY = 0;
	while (startY1 < gameBuff->HEIGHT) {
		for (int i = 0; i < gameBuff->WIDTH; i++) { 
			gameBuff->consoleBuffer[startY1 * gameBuff->WIDTH + i] = bg_river_image[imageY * gameBuff->WIDTH + i];
		}

		startY1++;
		imageY++;
	}

	startY1 = (gameBuff->timeInMillis / 50) % 320;
	int startY2 = (gameBuff->timeInMillis / 50) % 320 - 320;
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
			scrollBackground(gameBuff);
			animateBee(gameBuff);
			break;
		}
		return false;
	}
	else
	{
		if (!gameBuff->fireRunning)
			gameBuff->fireRunning = true;
		updateMenus(gameBuff);
		displayMenu(gameBuff);
		return false;
	}
}
