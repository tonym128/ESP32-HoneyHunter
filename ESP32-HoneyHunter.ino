#include "game.hpp"

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");
  
  gameInit();
  gameSetup();
}

void loop() {
  gameLoop();
}
