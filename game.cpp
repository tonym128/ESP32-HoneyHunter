#define SERIAL_HACK 1
#include "game.hpp"

static GameBuff *gameBuff = nullptr;
static byte buttonVals;
static std::array<int, 8> buttonRaw;

#ifdef SDL2_FOUND
#include "platform/game_sdl.h"
#elif ESP32
#include "platform/esp32.h"
#endif

static unsigned long batteryWarningEnd = 0;

bool displayImageInfinite(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
  int screenpixel;
  int i, j, k;
  int r, g, b;
  uint8_t colour;

  for (i = 0; i < w; i++)
  {
    for (j = 0; j < h; j++)
    {
      k = bitmap[i + j * w];

      r = bitExtracted(k, 5, 12);
      g = bitExtracted(k, 6, 6);
      b = bitExtracted(k, 5, 1);

      colour = MakeColor565(r, g, b);
      screenpixel = x + i + (y + j) * gameBuff->WIDTH;
      if (screenpixel >= 0 && screenpixel < gameBuff->MAXPIXEL)
        { gameBuff->consoleBuffer[screenpixel] = colour; }
    }
  }

  return true;
}

bool displayImage(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
  int screenpixel;
  int i, j, k;
  int r, g, b;
  uint8_t colour;

  for (i = 0; i < w; i++)
  {
    for (j = 0; j < h; j++)
    {
      k = bitmap[i + j * w];

      r = bitExtracted(k, 5, 12);
      g = bitExtracted(k, 6, 6);
      b = bitExtracted(k, 5, 1);

      colour = MakeColor565(r, g, b);
      screenpixel = x + i + (y + j) * gameBuff->WIDTH;
      if (screenpixel >= 0 && screenpixel < gameBuff->MAXPIXEL)
        { gameBuff->consoleBuffer[screenpixel] = colour; }
    }
  }

  return true;
}

BadgeState *loadBadgeSettings()
{
  JSON_Value *user_data;
  char *fileData = gameLoadFile((char*)"Badge.json");
  user_data = json_parse_string(fileData);
  free(fileData);

  if (user_data == nullptr || user_data == NULL)
  {
    return nullptr;
  }

  BadgeState *state = (BadgeState *)malloc(sizeof(struct BadgeState));
  state->bt_addr = (char *)json_object_get_string(json_object(user_data), (char*)"bt_addr");

  // state->ssid = (char *)json_object_get_string(json_object(user_data), (char*)"ssid");
  // ssid = state->ssid;

  // state->password = (char *)json_object_get_string(json_object(user_data), (char*)"password");
  // password = state->password;

  if (json_object_has_value(json_object(user_data), (char*)"customBoot")) {
    state->customBoot = (bool)json_object_get_boolean(json_object(user_data), (char*)"customBoot");
    state->bootMenuItem = (int)json_object_get_number(json_object(user_data), (char*)"bootMenuItem");
  } else {
    state->customBoot = false;
    state->bootMenuItem = 0;
  }

  free(user_data);

  // Try to load Badge Name
  fileData = gameLoadFile((char*)"Name.json");
  user_data = json_parse_string(fileData);
  free(fileData);

  if (user_data != NULL)
  {
    state->name = (char *)json_object_get_string(json_object(user_data), (char*)"name");
    free(user_data);
  }
  else
  {
    state->name = (char *)DEVICE;
  }

  return state;
}

void saveBadgeSettings(BadgeState *state)
{
  JSON_Value *root_value = json_value_init_object();
  JSON_Object *root_object = json_value_get_object(root_value);
  json_object_set_string(root_object, "bt_addr", state->bt_addr);
  json_object_set_string(root_object, "ssid", state->ssid);
  json_object_set_string(root_object, "password", state->password);
  json_object_set_boolean(root_object, "customBoot", state->customBoot);
  json_object_set_number(root_object, "bootMenuItem", (double)state->bootMenuItem);
  char *stateString = json_serialize_to_string_pretty(root_value);

  gameSaveFile((char*)"Badge.json", stateString);
  json_free_serialized_string(stateString);
  json_value_free(root_value);
}


void gameSetup()
{
  if (gameBuff == nullptr)
    gameBuff = new GameBuff();
	  // Bee Game - gameBuff->gameMode = 1;
	  // Wolf Game - gameBuff->gameMode = 2;
	  gameBuff->gameMode = 1;
	  gameBuff->maxGameMode = 8;

	  gameBuff->enter = true;
  	gameBuff->firstRun = true;

  if (gameBuff->badgeState == nullptr || gameBuff->badgeState == NULL)
  {
    gameBuff->badgeState = loadBadgeSettings();

    if (gameBuff->badgeState == nullptr || gameBuff->badgeState == NULL)
    {
      gameBuff->badgeState = (BadgeState *)malloc(sizeof(struct BadgeState));

      gameBuff->badgeState->bt_addr = (char *)malloc(20 * sizeof(char));
      gameBuff->badgeState->ssid = ssid;
      gameBuff->badgeState->password = password;
      gameBuff->badgeState->customBoot = false;
      gameBuff->badgeState->bootMenuItem = 1; // Default to be game

      saveBadgeSettings(gameBuff->badgeState);
      gameBuff->badgeState = loadBadgeSettings();
      // It reboots after this if there isn't enough free memory to alloc the consoleBuffer, but that's fine it'll have the file on 2nd boot.
    }
  }

  gameBuff->playerKeys.debouncedInput = true;
  gameBuff->consoleBuffer = (uint8_t *)calloc(gameBuff->MAXPIXEL, sizeof(uint8_t));

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(displayImage);

  displayClear(gameBuff, 0x00);

#ifdef ESP32
  int wakeup = print_wakeup_reason();
  if (wakeup == ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    if (gameBuff->badgeState->bootMenuItem == 2)
    {
      gameBuff->badgeState->bootMenuItem = 1;
    }
    else
    {
      gameBuff->badgeState->bootMenuItem = 2;
    }

    saveBadgeSettings(gameBuff->badgeState);
  }

  gameBuff->gameMode = gameBuff->badgeState->bootMenuItem; // Boot into boot menu item
#endif

}

void processInput(byte buttonVals)
{
  gameBuff->timeInMillis = getTimeInMillis();

  gameBuff->playerKeys.up = processKey(buttonVals, P1_Top);
  gameBuff->playerKeys.down = processKey(buttonVals, P1_Bottom);

  if (gameBuff->playerKeys.debouncedInput && buttonVals > 0) {
    gameBuff->playerKeys.debouncedInput = false;
    gameBuff->playerKeys.debounceTimeout = gameBuff->playerKeys.debounceDelay + gameBuff->timeInMillis;
  } else if (!gameBuff->playerKeys.debouncedInput && gameBuff->playerKeys.debounceTimeout < gameBuff->timeInMillis) {
    gameBuff->playerKeys.debouncedInput = true;
  }

  if (gameBuff->playerKeys.up && gameBuff->playerKeys.down)
  {
    if (gameBuff->timeInMillis > esp32gameon_debounce)
    {
      esp32gameon_debug_fps = !esp32gameon_debug_fps;
      esp32gameon_debounce = gameBuff->timeInMillis + 1000;
      esp32gameon_debug_counter++;
    }
  }

  // if (gameBuff->playerKeys.select || gameBuff->playerKeys.start)
  // {
  //   if (gameBuff->playerKeys.up && gameBuff->playerKeys.down)
  //   {
  //     if (gameBuff->timeInMillis > esp32gameon_debounce)
  //     {
  //       esp32gameon_debug_fps = !esp32gameon_debug_fps;
  //       esp32gameon_debounce = gameBuff->timeInMillis + 1000;
  //     }
  //   }

  //   if (gameBuff->playerKeys.left && gameBuff->playerKeys.right)
  //   {
  //     if (gameBuff->timeInMillis > esp32gameon_debounce)
  //     {
  //       esp32gameon_debug_output = !esp32gameon_debug_output;
  //       esp32gameon_debounce = gameBuff->timeInMillis + 1000;
  //     }
  //   }

  //   if (gameBuff->playerKeys.a && gameBuff->playerKeys.b)
  //   {
  //     format();
  //   }
  // }
}

void serialInput() {
}

void gameLoop()
{
  // put your main code here, to run repeatedly:
  processInput(getReadShift());
  serialInput();

  if (myGameLoop(gameBuff))
  {
    gameSetup();
  }

	voltageF = getVoltage();
  if (voltageF > 3.0 && voltageF < 3.8) {
        if (batteryWarningEnd == 0) batteryWarningEnd = 5000 + getTimeInMillis();
        else if (batteryWarningEnd < getTimeInMillis()) {
          heavySleep();
          drawString(gameBuff,(char*)"DEEPSLEEP!",0,gameBuff->HEIGHT-16,0xE0,0);
        }
        
        drawString(gameBuff,(char*)"LOW BATTERY!",0,0,0xE0,0);
        drawString(gameBuff,(char*)"PLEASE CHARGE!",0,16,0xE0,0);
        return;
  }

  calcFPS();
  if (gameBuff->consoleBuffer != nullptr)
  {
    if (esp32gameon_debug_fps)
    {
#ifdef ESP32
      if (!heap_caps_check_integrity_all(true))
      {
        Dimensions dimRed;
        dimRed.x = 0;
        dimRed.y = 0;
        dimRed.width = 30;
        dimRed.height = 30;

        drawBlock(gameBuff, dimRed, 0xE0);
        drawString(gameBuff,(char*)"heap!",0,0,0xFF,0);
      }

      switch (esp32gameon_debug_counter) {
        case 3: 
          drawString(gameBuff,(char*)"Connecting",0,0,0xFF,0);
          esp32gameon_debug_counter++;
          break;
        case 4: 
          initOTA();
          esp32gameon_debug_counter++;
          break;
        case 5: 
          char IP[] = "xxx.xxx.xxx.xxx";          // buffer
          IPAddress ip = WiFi.localIP();
          ip.toString().toCharArray(IP, 16);
          drawString(gameBuff,(char*)IP,0,0,0xFF,0);
          ArduinoOTA.handle();
          break;
      }

#endif
      drawFPS(gameBuff);
      drawVoltage(gameBuff);
    }

#ifdef ESP32
    if (esp32gameon_debug_fps_serial) {
      Serial.println(currentFPS());
    }
#endif

    if (esp32gameon_debug_output)
    {
      std::array<int, 8> rawValues = getRawInput();
      byte readShift = getReadShift();
      for (int i = 0; i < 8; i += 1)
      {
        char fpsString[17];
        sprintf(fpsString, "%d:%03d", i, rawValues[i]);
        bool pressed = (readShift >> i) & 1;
        drawString(gameBuff, fpsString, 0, i * 8, pressed ? 0x80 : 0xFF, -1);
      }
    }

    sendToScreen();
    updateMinTime(33);
  }
}
