#define SDL 1
#include "SDL.h"
#include "SDL_main.h"

SDL_Renderer *renderer;
SDL_Window *window;

static const char *DEVICE = "SDL";
const Uint8 *keystate;

float getVoltage() {
	return 0.0;
}

byte getReadShift()
{
	byte buttonVals = 0;
	SDL_PumpEvents();
	keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_UP])
		buttonVals = buttonVals | (1 << P1_Top);
	if (keystate[SDL_SCANCODE_DOWN])
		buttonVals = buttonVals | (1 << P1_Bottom);
	if (keystate[SDL_SCANCODE_LEFT])
		buttonVals = buttonVals | (1 << P1_Left);
	if (keystate[SDL_SCANCODE_RIGHT])
		buttonVals = buttonVals | (1 << P1_Right);
	if (keystate[SDL_SCANCODE_W])
		buttonVals = buttonVals | (1 << P2_Top);
	if (keystate[SDL_SCANCODE_A])
		buttonVals = buttonVals | (1 << P2_Left);
	if (keystate[SDL_SCANCODE_S])
		buttonVals = buttonVals | (1 << P2_Bottom);
	if (keystate[SDL_SCANCODE_D])
		buttonVals = buttonVals | (1 << P2_Right);
	if (keystate[SDL_SCANCODE_Q]) {
		SDL_DestroyWindow(window);
		exit(0);
	}
	return buttonVals;
}

std::array<int,8> getRawInput() {
  std::array<int, 8> rawValues;
	for (int i = 0; i < 8; i++) {
		rawValues[i] = 0;
	}

	SDL_PumpEvents();
	keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_LEFT])
		rawValues[0] = 1;
	if (keystate[SDL_SCANCODE_UP])
		rawValues[1] = 1;
	if (keystate[SDL_SCANCODE_RIGHT])
		rawValues[2] = 1;
	if (keystate[SDL_SCANCODE_DOWN])
		rawValues[3] = 1;

	if (keystate[SDL_SCANCODE_D])
		rawValues[4] = 1;
	if (keystate[SDL_SCANCODE_S])
		rawValues[5] = 1;
	if (keystate[SDL_SCANCODE_A])
		rawValues[6] = 1;
	if (keystate[SDL_SCANCODE_W])
		rawValues[7] = 1;

	return rawValues;
}

const static uint8_t blueMask = 0x03;
const static uint8_t greenMask = 0x1C;
const static uint8_t redMask = 0xE0;
static int colour,r,g,b,x,y;

void sendToScreen()
{
	if (!gameBuff->rotate) 
		for (int i = 0; i < gameBuff->MAXPIXEL; i++)
		{
			x = i % gameBuff->WIDTH;
			y = i / gameBuff->WIDTH;

			colour = gameBuff->consoleBuffer[i];

			r = (colour >> 5) * 32;
			g = ((colour & 28) >> 2) * 32;
			b = (colour & 3) * 64;

			SDL_SetRenderDrawColor(renderer, r, g, b, 0);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	else
      for (int x = gameBuff->WIDTH-1; x >= 0; x--) {
	      for (int y = 0; y < gameBuff->HEIGHT; y++) {

			colour = gameBuff->consoleBuffer[x + y * gameBuff->WIDTH];

			r = (colour >> 5) * 32;
			g = ((colour & 28) >> 2) * 32;
			b = (colour & 3) * 64;

			SDL_SetRenderDrawColor(renderer, r, g, b, 0);
			SDL_RenderDrawPoint(renderer, y, gameBuff->WIDTH - (x + 1));
		}
	  }

	SDL_RenderPresent(renderer);
}

void heavySleep() {

}

void gameInit()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"ESP32GameOn",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		gameBuff->WIDTH,
		gameBuff->HEIGHT,
		0);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void printBLEDeviceAddress(char* bd_addr) {
  sprintf(bd_addr,"00:11:22:33:44:55");
}

void format() {
}

char *gameLoadFile(char *fileName) {
    #ifdef __EMSCRIPTEN__
	return nullptr;
	#else
	SDL_RWops *rw = SDL_RWFromFile(fileName, "rb");
    if (rw == NULL) return nullptr;

    size_t res_size = (size_t)SDL_RWsize(rw);
    char* fileData = (char*)malloc(res_size + 2);

	size_t nb_read_total = 0, nb_read = 1;
	char* buf = fileData;
	while (nb_read_total < res_size && nb_read != 0) {
			nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
			nb_read_total += nb_read;
			buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
			free(fileData);
			return nullptr;
	}

	fileData[nb_read_total] = '\0';

	return fileData;
	#endif
}

bool gameSaveFile(char *fileName, char *data) {
    #ifdef __EMSCRIPTEN__
	return false;
	#else
	SDL_RWops *rw = SDL_RWFromFile(fileName, "w");
	if(rw != NULL) {
		size_t len = SDL_strlen(data);
		if (SDL_RWwrite(rw, data, 1, len) != len) {
			return false;
		}

		SDL_RWclose(rw);
		return true;
	}

	return false;
	#endif
}

const char *getDevicePlatform() {
	return DEVICE;
}

int main(int argc, char*argv[])
{
  #ifdef _WIN32
  HWND hWnd = GetConsoleWindow();
  ShowWindow(hWnd, SW_HIDE);
  #endif

  gameInit();
  gameSetup();
  while (1)
  {
    gameLoop();
  }
  return 1;
}
