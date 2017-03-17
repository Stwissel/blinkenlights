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
  // Set the phone
  int i;
  for (i = 13; i < 19; i++) {
    colorPixel[i] = strip.Color(0, 0, 50);
  }
  showStrip(100);
}

// A phone vibrates left to right, so we simulate that with
// a blue/white phone
// 4px is the blue phone 2px is the white margin
void loop() {
  // put your main code here, to run repeatedly:
  colorPixel[11] = 0;
  colorPixel[12] = 0;
  colorPixel[13] = strip.Color(50, 50, 50);
  colorPixel[18] = strip.Color(0, 0, 50);
  colorPixel[19] = strip.Color(50, 50, 50);
  colorPixel[20] = strip.Color(50, 50, 50);
  showStrip(800);
  colorPixel[11] = strip.Color(50, 50, 50);
  colorPixel[12] = strip.Color(50, 50, 50);
  colorPixel[13] = strip.Color(0, 0, 50);
  colorPixel[18] = strip.Color(50, 50, 50);
  colorPixel[19] = 0;
  colorPixel[20] = 0;
  showStrip(800);
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
