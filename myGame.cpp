#include "myGame.hpp"
#include "beeGame.hpp"
#include "wolfGame.hpp"

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

uint8_t boredCounter = 0;
bool firstRun = true;
bool myGameLoop(GameBuff *gameBuff)
{
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

			if (!beeGame.win) {
				displayClear(gameBuff,0x00);
				scrollBackground(gameBuff);
				drawFlowers(gameBuff);
				animateBee(gameBuff);
				drawBeeStatus(gameBuff);
			} else {
				if (currentTimeInMillis + 10000 < getTimeInMillis()) {
					heavySleep();
				}
			}
			break;
		case 2:
			if (firstRun) {
				initWolfGame(gameBuff);
				firstRun = false;
			}

			inputWolf(gameBuff);
			updateWolfGame(gameBuff);

			if (!wolfGame.win) {
				displayClear(gameBuff,0x00);
				wolf_scrollBackground(gameBuff);
				drawSnowFlakes(gameBuff);
				animateWolf(gameBuff);
				drawWolfStatus(gameBuff);
			} else {
				if (currentTimeInMillis + 10000 < getTimeInMillis()) {
					heavySleep();
				}
			}
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
