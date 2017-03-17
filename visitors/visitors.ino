#include "LPD8806.h"
#include "SPI.h"

// Parameters to configure a strip
#define dataPin 5
#define clockPin 6
#define numOfPixels 32

// Colors
uint32_t colorPixel[numOfPixels];

LPD8806 strip = LPD8806(numOfPixels, dataPin, clockPin);

void setup() {
  resetColors(0);
  strip.begin();
}

// A phone vibrates left to right, so we simulate that with
// a blue/white phone
// 3px is the green visitor 1px is the white margin
void loop() {
  int i;
  for (i = 28; i < 31; i++) {
    colorPixel[i] = strip.Color(0, 50, 0);
  }
  showStrip(100);
  announceVisiors(4);
  for (i = 0; i < numOfPixels; i++) {
    colorPixel[numOfPixels-1] = 0;
    shiftPixelDown(numOfPixels);
    showStrip(50);
  }
}

void announceVisiors(int times) {
  int i;
  for (i = 0; i < times; i++) {
    colorPixel[31] = strip.Color(50, 50, 50);
    colorPixel[27] = 0;
    showStrip(800);
    colorPixel[31] = 0;
    colorPixel[27] = strip.Color(50, 50, 50);
    showStrip(800);
  }
}

void shiftPixelDown(int startPixel) {
  int i;
  for (i = 0; i < startPixel; i++) {
    colorPixel[i] = colorPixel[i + 1];
  }
}

/**
   Show the strip and prep for next
*/
void showStrip(int wait) {
  int i;
  for (i = 0; i < numOfPixels; i++) {
    strip.setPixelColor(i, colorPixel[i]);
  }
  strip.show();
  delay(wait);
}

/**
   Set all LED to one color
*/
void resetColors(uint32_t color) {
  int i;
  for (i = 0; i < numOfPixels; i++) {
    colorPixel[i] = color;
    strip.setPixelColor(i, color);
  }
}
