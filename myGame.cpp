#include "myGame.hpp"
#include "files/bee_image.h"
#include "files/bee_comb_image.h"
#include "files/bg_river_image.h"
#include "files/flowers_image.h"
#include "files/winner.jpg.h"

void displayItemQRCode(GameBuff *gameBuff,char *url)
{
	qrcodeStruct QRPic = drawQRCode(url);
	Dimensions dim;
	dim.x = 0;
	dim.y = 0;
	dim.width = QRPic.size;
	dim.height = QRPic.size;

	if (QRPic.size > 0)
	{
		// Centre the QR Code
		int x = gameBuff->WIDTH / 2;
		while (QRPic.size < x)
		{
			x /= 2;
		}

		dim.x = dim.y = x - QRPic.size / 2;

		// Draw the QR Code
		drawObject(gameBuff, dim, QRPic.pic, 0x00, 0xFF, -1);
		free(QRPic.pic);

		// Double the screen till we fill as much as we can.
		x = gameBuff->WIDTH / 2;
		while (QRPic.size < x)
		{
			drawScreenDouble(gameBuff);
			x /= 2;
		}
	}

	drawString(gameBuff,url,3,3,0xFF,0);
}

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
	drawString(gameBuff, (char *)"0) Image ", x, i += 16, gameBuff->gameMode == gameBuff->maxGameMode++ ? 0x1C : 0xFF, 0);
	drawString(gameBuff, (char *)"1) QR ", x, i += 16, gameBuff->gameMode == gameBuff->maxGameMode++ ? 0x1C : 0xFF, 0);

	gameBuff->maxGameMode--;
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

uint8_t boredCounter = 0;
bool firstRun = true;
bool myGameLoop(GameBuff *gameBuff)
{
	if (gameBuff->enter)
	{
		switch (gameBuff->gameMode)
		{
		case 0:
			if (firstRun) {
				firstRun = false;
			}

			displayClear(gameBuff,0x00);
			TJpgDec.drawJpg(0,0,winner_jpg,sizeof(winner_jpg));

			if (gameBuff->playerKeys.down) gameBuff->enter = false;
			break;
		case 1:
			if (firstRun) {
				firstRun = false;
				displayClear(gameBuff,0x00);
			}
	
			displayClear(gameBuff,0x00);
			displayItemQRCode(gameBuff,"https://github.com/tonym128/ESP32-HoneyHunter");
			if (gameBuff->playerKeys.down) gameBuff->enter = false;
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
