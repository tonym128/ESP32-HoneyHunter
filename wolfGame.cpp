#include "wolfGame.hpp"
WolfGame wolfGame;

bool wolfGameWin() {
	return wolfGame.win;
}

bool wolfGameDone() {
	return currentTimeInMillis + 10000 < getTimeInMillis();
}

void inputWolf(GameBuff *gameBuff) {
	if (wolfGame.win) return;

	currentTimeInMillis = getTimeInMillis();
	frameTimeInMillis = (currentTimeInMillis - lastTimeInMillis);
	if (gameBuff->playerKeys.up) {
		wolfGame.x -=  frameTimeInMillis* 4000;
	}

	if (gameBuff->playerKeys.down) {
		wolfGame.x += frameTimeInMillis * 4000;
	}

	if (wolfGame.x > INT_TO_FIXP(gameBuff->WIDTH-32)) wolfGame.x = INT_TO_FIXP(gameBuff->WIDTH-32);
	if (wolfGame.x < INT_TO_FIXP(0)) wolfGame.x = INT_TO_FIXP(0);
	lastTimeInMillis = currentTimeInMillis;
}

void updateWolfGame(GameBuff *gameBuff) {
	if (wolfGame.win) return;

	Dimensions dimWolf;
	dimWolf.height = 32;
	dimWolf.width = 32;
	dimWolf.x = FIXP_TO_INT(wolfGame.x);
	dimWolf.y = FIXP_TO_INT(wolfGame.y);

	Dimensions dimSnowFlake;
	dimSnowFlake.width = 32;
	dimSnowFlake.height = 32;

	wolfGame.onSnowFlake = false;
	for (int i = 0; i < SnowFlake_COUNT; i++ ) {
		if (wolfGame.SnowFlakes[i].visible) {
			dimSnowFlake.x = FIXP_TO_INT(wolfGame.SnowFlakes[i].x);
			dimSnowFlake.y = FIXP_TO_INT(wolfGame.SnowFlakes[i].y);
			if (rectCollisionCheck(dimWolf,dimSnowFlake)) {
				wolfGame.onSnowFlake = true;
			}
		}
	}

	if (wolfGame.onSnowFlake) {
		wolfGame.power += frameTimeInMillis * 500;
	} else {
		wolfGame.power -= frameTimeInMillis * 70;
	}

	if (wolfGame.power > wolfGame.maxpower) wolfGame.power = wolfGame.maxpower;
	if (wolfGame.power < INT_TO_FIXP(0)) wolfGame.power = INT_TO_FIXP(0);

	// De spawn SnowFlakes as needed
	for (int i = 0; i < SnowFlake_COUNT; i++) {
		if (wolfGame.SnowFlakes[i].visible && wolfGame.SnowFlakes[i].y > INT_TO_FIXP(gameBuff->HEIGHT)) {
			wolfGame.SnowFlakes[i].visible = false;
		}
	}
	
	// Spawn new SnowFlakes as needed
	if (wolfGame.nextSnowFlakeSpawn < currentTimeInMillis) {
		for (int i = 0; i < SnowFlake_COUNT; i++) {
			if (!wolfGame.SnowFlakes[i].visible) {
				int x = rand() % (gameBuff->WIDTH-16);
				wolfGame.SnowFlakes[i].type = rand() % 6;
				wolfGame.SnowFlakes[i].x = INT_TO_FIXP(x);
				wolfGame.SnowFlakes[i].y = INT_TO_FIXP(-32);
				wolfGame.SnowFlakes[i].visible = true;
				wolfGame.nextSnowFlakeSpawn = currentTimeInMillis + wolfGame.SnowFlakeSpawnTimer + rand() % 1000;
				break;
			}
		}
	}

	if (wolfGame.power == wolfGame.maxpower) {
		wolfGame.win = true;
		TJpgDec.drawJpg(0,0,wolf_win_jpg,sizeof(wolf_win_jpg));
	}
}

void animateWolf(GameBuff *gameBuff) {
	int startX = (gameBuff->timeInMillis / 220 % 8) * 32;
	int screenXOffset = FIXP_TO_INT(wolfGame.x);
	int screenYOffset = FIXP_TO_INT(wolfGame.y) * gameBuff->WIDTH;

	int y = 0;
	int x = 0;
	int pixel = 0;

	while (y < 32) {
		pixel = startX + x + y * 256;
		while (x < 32) {
			if (wolf_run_image[pixel] != 0x00)
				gameBuff->consoleBuffer[screenXOffset + x + screenYOffset + y * gameBuff->WIDTH] = wolf_run_image[pixel];
			x++;
			pixel++;
		}
		
		x = 0;
		y++;
	}
}

// Draw directly from uncompressed 332 image bitmap, with some minor optimisations
int wolf_startY1 = 0;
void wolf_scrollBackground(GameBuff *gameBuff) {
	wolf_startY1 = (wolf_startY1 + frameTimeInMillis / 30) % 320;
	int startY1Mod = wolf_startY1;
	int imageY = 0;
	int pixel, imagepixel;

	while (startY1Mod < gameBuff->HEIGHT) {
		for (int i = 0; i < gameBuff->WIDTH; i++) { 
			pixel = startY1Mod * gameBuff->WIDTH + i;
			imagepixel = imageY * gameBuff->WIDTH + i;

			gameBuff->consoleBuffer[pixel] = wolf_bg_image[imagepixel];
		}

		startY1Mod++;
		imageY++;
	}

	int startY2 = wolf_startY1 - 320;
	imageY = 0;
	
	while (startY2 < wolf_startY1 && startY2 < gameBuff->HEIGHT) { 
		if (startY2 >= 0)
			for (int i = 0; i < gameBuff->WIDTH; i++) { 
				pixel = startY2 * gameBuff->WIDTH + i;
				imagepixel = imageY * gameBuff->WIDTH + i;

				gameBuff->consoleBuffer[pixel] = wolf_bg_image[imagepixel];
			}

		startY2++;
		imageY++;
	}
}

void initWolfGame(GameBuff *gameBuff) {
	wolfGame.win = false;
	wolfGame.x = INT_TO_FIXP(gameBuff->WIDTH/3);
	wolfGame.y = INT_TO_FIXP(gameBuff->HEIGHT - 50);
	wolfGame.power = 0;
	wolfGame.maxpower = INT_TO_FIXP(gameBuff->WIDTH);
	wolfGame.onSnowFlake = false;

	for (int i = 0; i < SnowFlake_COUNT; i++)
	{
		wolfGame.SnowFlakes[i].visible = false;
	}

	wolfGame.SnowFlakeSpawnTimer = 3000; // Spawn a SnowFlake every 5 seconds 
	wolfGame.nextSnowFlakeSpawn = currentTimeInMillis;

}

void drawSnowFlakes(GameBuff *gameBuff) {
	Dimensions dim;
	dim.height = 32;
	dim.width = 32;
	
	for (int i = 0; i < SnowFlake_COUNT; i++ ) {
		if (wolfGame.SnowFlakes[i].visible) {
			dim.x = FIXP_TO_INT(wolfGame.SnowFlakes[i].x);
			dim.y = FIXP_TO_INT(wolfGame.SnowFlakes[i].y);

			//drawObject(gameBuff,FIXP_TO_INT(wolfGame.SnowFlakes[i].x), FIXP_TO_INT(wolfGame.SnowFlakes[i].y),  32, 32, 0 ,192, wolf_snowflake_image,0xFF);
			rotateObjectAlpha(gameBuff,dim,currentTimeInMillis % 6000/1000.0f,1,wolf_snowflake_image,0x00);
			wolfGame.SnowFlakes[i].y += INT_TO_FIXP(frameTimeInMillis / 30);
		}
	}
}

void drawWolfStatus(GameBuff *gameBuff) {
	Dimensions dimScreen, dimImage;
	dimScreen.x = 0;
	dimScreen.y = 0;
	dimScreen.width = FIXP_TO_INT(wolfGame.power);
	dimScreen.height = 32;
	dimImage.x = 0;
	dimImage.y = 0;
	dimImage.height = 32;
	dimImage.width = 135;
	drawObjectScrollLoop(gameBuff,dimScreen,dimImage,wolf_power_image,0x00);
}