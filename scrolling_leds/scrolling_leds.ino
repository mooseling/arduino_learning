#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

byte baseFrame[8][12] = {
  { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};


byte translatedFrame[8][12];

int counter = 0;


void setup() {
  Serial.begin(115200);
  matrix.begin();
}


void loop() {
  updateTranslatedFrame(counter);
  matrix.renderBitmap(translatedFrame, 8, 12);
  counter = counter + 1;
  delay(300);
}


void updateTranslatedFrame(int offset) {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 12; x++) {
      translatedFrame[y][x] = baseFrame[y][(x + offset) % 12];
    }
  }
}
