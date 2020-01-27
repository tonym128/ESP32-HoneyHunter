#ifndef _WOLFGAME_H_
#define _WOLFGAME_H_
#include "game.hpp"

#include "files/wolf_bg_image.h"
#include "files/wolf_power.h"
#include "files/wolf_run_image.h"
#include "files/wolf_snowflake_image.h"
#include "files/wolf_win.jpg.h"

struct SnowFlake {
	bool visible;
	FIXPOINT x;
	FIXPOINT y;
	FIXPOINT nectar;
	int type;
};

static const int SnowFlake_COUNT = 10;
struct WolfGame {
	FIXPOINT x, y, power, maxpower;
	SnowFlake SnowFlakes[SnowFlake_COUNT];
	unsigned long SnowFlakeSpawnTimer, nextSnowFlakeSpawn;
	bool onSnowFlake = false;
	bool bitmask[135];
	bool win = false;
};

void inputWolf(GameBuff *gameBuff);
void updateWolfGame(GameBuff *gameBuff);
void animateWolf(GameBuff *gameBuff);
void wolf_scrollBackground(GameBuff *gameBuff);
void initWolfGame(GameBuff *gameBuff);
void drawSnowFlakes(GameBuff *gameBuff);
void drawWolfStatus(GameBuff *gameBuff);
bool wolfGameWin();
bool wolfGameDone();

#endif