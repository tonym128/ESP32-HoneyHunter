#ifndef PLATFORM_CORE_H
#define PLATFORM_CORE_H 1
// Button definitions
const int P1_Left = 0;
const int P1_Top = 1;
const int P1_Right = 2;
const int P1_Bottom = 3;

const int P2_Right = 4;
const int P2_Bottom = 5;
const int P2_Left = 6;
const int P2_Top = 7;

#ifdef _WIN32
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <Windows.h>
#include <cwchar>
#include <stdint.h>
#include <math.h> // Added for GCC
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#elif __linux
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <cwchar>
#include <stdint.h>
#include <string.h>
#include <math.h>

//typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
//typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t byte;     // BYTE = unsigned 8 bit value
#elif ESP32
#include <Arduino.h>  // for type definitions
#endif

unsigned long getTimeInMillis();
void gameSleep(int);
float getVoltage();
void heavySleep();

struct PlayerKeys {
  bool up = false;
  bool down = false;
  bool right = false;
  bool left = false;
  bool a = false;
  bool b = false;
  bool start = false;
  bool select = false;

  int debounceDelay = 300;
  bool debouncedInput = true;
  unsigned long debounceTimeout = 0;
};

const char *getDevicePlatform();

static unsigned long frameTime = getTimeInMillis();
static unsigned long startTime = getTimeInMillis();
static unsigned long currentTime = getTimeInMillis();
static unsigned long fpsTimer1 = getTimeInMillis();
static unsigned long fpsTimer2 = getTimeInMillis();
static unsigned int timeDiff = 0;
#endif