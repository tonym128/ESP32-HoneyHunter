#include <TFT_eSPI.h>
#include "Button2.h"
#include "esp_adc_cal.h"

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 86400       //Time ESP32 will go to sleep (in seconds)

#define BUTTON_1 0
#define BUTTON_2 35
#define ADC_PIN 34
#define ADC_EN 14

Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

int btn1Press = false;
int btn2Press = false;

static const byte TOUCH_SENSE_MAX = 50;
static const byte TOUCH_SENSE_MIN = 20;
int inputVal = 0;
static std::array<int, 8> TOUCH_SENSE;
static const char *DEVICE = (char *)"ESP32";

TFT_eSPI tft = TFT_eSPI(135, 240);

bool readAnalogSensor(int pin, int touch_sense)
{
  inputVal = touchRead(pin);
  return inputVal < touch_sense && inputVal > 0;
}

int readAnalogSensorRaw(int pin)
{
  inputVal = touchRead(pin);
  return inputVal;
}

static int vref = 1100;
static float battery_voltage = 0.0f;
static uint64_t timeStamp = 0;

float getVoltage()
{
  if (millis() - timeStamp > 2000)
  {
    float v = ((float)analogRead(ADC_PIN) / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    if (v > 0)
    {
      battery_voltage = v;
    }
    timeStamp = millis();
  }

  return battery_voltage;
}

byte getReadShiftAnalog()
{
  byte buttonVals = 0;
  buttonVals = buttonVals | (btn1Press << P1_Top);
  buttonVals = buttonVals | (btn2Press << P1_Bottom);

  return buttonVals;
}

std::array<int, 8> getRawInput()
{
  std::array<int, 8> rawValues;
  for (int i = 0; i < 8; i++)
  {
    rawValues[i] = 0;
  }

  int i = 0;
  // rawValues[i++] = readAnalogSensorRaw(32); // Left
  // rawValues[i++] = readAnalogSensorRaw(14) || btn2Press; // Up
  // rawValues[i++] = readAnalogSensorRaw(27); // Right
  // rawValues[i++] = readAnalogSensorRaw(33) || btn1Press; // Down
  // rawValues[i++] = readAnalogSensorRaw(15); // A
  // rawValues[i++] = readAnalogSensorRaw(13); // Start
  // rawValues[i++] = readAnalogSensorRaw(2);  // B
  // rawValues[i++] = readAnalogSensorRaw(12); // Select

  return rawValues;
}

void button_loop()
{
  btn1.loop();
  btn2.loop();
}

byte getReadShift()
{
  button_loop();
  return getReadShiftAnalog();
}

const static unsigned int RGB332to565lookupTable[256] = {
    0x0000, 0x000a, 0x0015, 0x001f, 0x0120, 0x012a, 0x0135, 0x013f,
    0x0240, 0x024a, 0x0255, 0x025f, 0x0360, 0x036a, 0x0375, 0x037f,
    0x0480, 0x048a, 0x0495, 0x049f, 0x05a0, 0x05aa, 0x05b5, 0x05bf,
    0x06c0, 0x06ca, 0x06d5, 0x06df, 0x07e0, 0x07ea, 0x07f5, 0x07ff,
    0x2000, 0x200a, 0x2015, 0x201f, 0x2120, 0x212a, 0x2135, 0x213f,
    0x2240, 0x224a, 0x2255, 0x225f, 0x2360, 0x236a, 0x2375, 0x237f,
    0x2480, 0x248a, 0x2495, 0x249f, 0x25a0, 0x25aa, 0x25b5, 0x25bf,
    0x26c0, 0x26ca, 0x26d5, 0x26df, 0x27e0, 0x27ea, 0x27f5, 0x27ff,
    0x4800, 0x480a, 0x4815, 0x481f, 0x4920, 0x492a, 0x4935, 0x493f,
    0x4a40, 0x4a4a, 0x4a55, 0x4a5f, 0x4b60, 0x4b6a, 0x4b75, 0x4b7f,
    0x4c80, 0x4c8a, 0x4c95, 0x4c9f, 0x4da0, 0x4daa, 0x4db5, 0x4dbf,
    0x4ec0, 0x4eca, 0x4ed5, 0x4edf, 0x4fe0, 0x4fea, 0x4ff5, 0x4fff,
    0x6800, 0x680a, 0x6815, 0x681f, 0x6920, 0x692a, 0x6935, 0x693f,
    0x6a40, 0x6a4a, 0x6a55, 0x6a5f, 0x6b60, 0x6b6a, 0x6b75, 0x6b7f,
    0x6c80, 0x6c8a, 0x6c95, 0x6c9f, 0x6da0, 0x6daa, 0x6db5, 0x6dbf,
    0x6ec0, 0x6eca, 0x6ed5, 0x6edf, 0x6fe0, 0x6fea, 0x6ff5, 0x6fff,
    0x9000, 0x900a, 0x9015, 0x901f, 0x9120, 0x912a, 0x9135, 0x913f,
    0x9240, 0x924a, 0x9255, 0x925f, 0x9360, 0x936a, 0x9375, 0x937f,
    0x9480, 0x948a, 0x9495, 0x949f, 0x95a0, 0x95aa, 0x95b5, 0x95bf,
    0x96c0, 0x96ca, 0x96d5, 0x96df, 0x97e0, 0x97ea, 0x97f5, 0x97ff,
    0xb000, 0xb00a, 0xb015, 0xb01f, 0xb120, 0xb12a, 0xb135, 0xb13f,
    0xb240, 0xb24a, 0xb255, 0xb25f, 0xb360, 0xb36a, 0xb375, 0xb37f,
    0xb480, 0xb48a, 0xb495, 0xb49f, 0xb5a0, 0xb5aa, 0xb5b5, 0xb5bf,
    0xb6c0, 0xb6ca, 0xb6d5, 0xb6df, 0xb7e0, 0xb7ea, 0xb7f5, 0xb7ff,
    0xd800, 0xd80a, 0xd815, 0xd81f, 0xd920, 0xd92a, 0xd935, 0xd93f,
    0xda40, 0xda4a, 0xda55, 0xda5f, 0xdb60, 0xdb6a, 0xdb75, 0xdb7f,
    0xdc80, 0xdc8a, 0xdc95, 0xdc9f, 0xdda0, 0xddaa, 0xddb5, 0xddbf,
    0xdec0, 0xdeca, 0xded5, 0xdedf, 0xdfe0, 0xdfea, 0xdff5, 0xdfff,
    0xf800, 0xf80a, 0xf815, 0xf81f, 0xf920, 0xf92a, 0xf935, 0xf93f,
    0xfa40, 0xfa4a, 0xfa55, 0xfa5f, 0xfb60, 0xfb6a, 0xfb75, 0xfb7f,
    0xfc80, 0xfc8a, 0xfc95, 0xfc9f, 0xfda0, 0xfdaa, 0xfdb5, 0xfdbf,
    0xfec0, 0xfeca, 0xfed5, 0xfedf, 0xffe0, 0xffea, 0xfff5, 0xffff};

static void sendToScreen()
{
  int colour = -1;
  int counter = 0;

  tft.startWrite();
  tft.setAddrWindow(0, 0, gameBuff->WIDTH, gameBuff->HEIGHT);

  if (!gameBuff->rotate)
  {
    for (int i = 0; i < gameBuff->WIDTH * gameBuff->HEIGHT;)
    {
      counter = 1;

      colour = gameBuff->consoleBuffer[i];
      while (colour == gameBuff->consoleBuffer[i + counter])
      {
        counter++;
      }

      tft.writeColor(RGB332to565lookupTable[colour], counter);
      i += counter;
    }
  }
  else
  {
    counter = 0;
    for (int x = gameBuff->WIDTH - 1; x >= 0; x--)
    {
      for (int y = 0; y < gameBuff->HEIGHT; y++)
      {
        if (counter > 0 && colour != gameBuff->consoleBuffer[x + y * gameBuff->WIDTH])
        {
          tft.writeColor(RGB332to565lookupTable[colour], counter);
          counter = 1;
          colour = gameBuff->consoleBuffer[x + y * gameBuff->WIDTH];
        }
        else
        {
          counter++;
        }
      }
    }
    tft.writeColor(RGB332to565lookupTable[colour], counter);
  }
  tft.endWrite();
}

void format()
{
  // When in doubt... format!
  Serial.println("Format SPIFFS");
  SPIFFS.format();
}

int print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
  case 1:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case 2:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case 3:
    Serial.println("Wakeup caused by timer");
    break;
  case 4:
    Serial.println("Wakeup caused by touchpad");
    break;
  case 5:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.println("Wakeup was not caused by deep sleep");
    break;
  }

  return wakeup_reason;
}

void heavySleep()
{
  tft.writecommand(TFT_DISPOFF);
  tft.writecommand(TFT_SLPIN);

  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_deep_sleep_start();
}

void gameInit()
{
  Serial.println("gameInit");
  // setupBLE();
  Serial.println("Screen Init");

  // Button Setup for built in buttons
  btn1.setPressedHandler([](Button2 &b) {
    btn1Press = true;
  });

  btn2.setPressedHandler([](Button2 &b) {
    btn2Press = true;
  });

  btn1.setReleasedHandler([](Button2 &b) {
    btn1Press = false;
  });

  btn2.setReleasedHandler([](Button2 &b) {
    btn2Press = false;
  });

  // VRef Setup for Voltage
  pinMode(ADC_PIN, INPUT);
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
  //Check type of calibration value used to characterize ADC
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
  {
    Serial.printf("eFuse Vref:%u mV\n", adc_chars.vref);
    vref = adc_chars.vref;
  }
  else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
  {
    Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
  }
  else
  {
    Serial.println("Default Vref: 1100mV");
  }

  tft.begin();            // initialize a ST7789 chip
  tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness
  tft.fillScreen(TFT_BLACK);

  // Serial.println("Input Init");
  // TOUCH_SENSE = getRawInput();
  // for (int i = 0; i < 8; i++)
  // {
  //   TOUCH_SENSE[i] /= 2;
  //   if (TOUCH_SENSE[i] > TOUCH_SENSE_MAX)
  //     TOUCH_SENSE[i] = TOUCH_SENSE_MAX;
  //   if (TOUCH_SENSE[i] < TOUCH_SENSE_MIN)
  //     TOUCH_SENSE[i] = TOUCH_SENSE_MIN;
  //   Serial.print("Touch Sense : ");
  //   Serial.print(i);
  //   Serial.print(" - ");
  //   Serial.println(TOUCH_SENSE[i]);
  // }

  Serial.println("SPIFFS Init");
  if (!SPIFFS.begin(true))
  {
    Serial.println("Mount Failed");
    SPIFFS.format();
    Serial.println("File formatted");
    return;
  }

  Serial.println("File system mounted");
}

char *gameLoadFile(char *fileName)
{
  Serial.print("Loading file : ");
  Serial.println(fileName);
  char *prepend = (char *)malloc(strlen(fileName) + 2);
  sprintf(prepend, "/%s", fileName);
  fs::File file = SPIFFS.open(prepend, "r");
  free(prepend);

  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return nullptr;
  }

  if (file.size() == 0)
  {
    Serial.println("Empty file");
    return nullptr;
  }

  char *fileData = (char *)malloc(file.size() + 1);
  file.readBytes(fileData, file.size());
  fileData[file.size()] = '\0';
  return fileData;
}

bool gameSaveFile(char *fileName, char *data)
{
  Serial.print("Saving file :");
  Serial.println(fileName);
  char *prepend = (char *)malloc(strlen(fileName) + 2);
  sprintf(prepend, "/%s", fileName);
  fs::File file = SPIFFS.open(prepend, FILE_WRITE);
  free(prepend);

  if (!file)
  {
    Serial.println("There was an error opening the file for writing");
    return false;
  }

  if (file.print(data))
  {
    Serial.println("File was written");
  }
  else
  {
    Serial.println("File write failed");
    return false;
  }

  file.close();
  return true;
}

const char *getDevicePlatform()
{
  return DEVICE;
}
