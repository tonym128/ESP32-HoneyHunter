#ifndef _BEEGAME_H_
#define _BEEGAME_H_
#include "game.hpp"

#include "files/bee_image.h"
#include "files/bee_comb_image.h"
#include "files/bg_river_image.h"
#include "files/flowers_image.h"
#include "files/winner.jpg.h"
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
	unsigned long flowerSpawnTimer, nextFlowerSpawn;
	bool onFlower = false;
	bool bitmask[135];
	bool win = false;
};

void inputBee(GameBuff *gameBuff);
void updateBeeGame(GameBuff *gameBuff);
void animateBee(GameBuff *gameBuff);
void scrollBackground(GameBuff *gameBuff);
void initBeeGame(GameBuff *gameBuff);
void drawFlowers(GameBuff *gameBuff);
void drawBeeStatus(GameBuff *gameBuff);
bool beeGameWin();
bool beeGameDone();

#endif