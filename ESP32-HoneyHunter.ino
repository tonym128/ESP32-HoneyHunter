#include "game.hpp"

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  
  gameInit();
  gameSetup();
}

void loop() {
  gameLoop();
}
