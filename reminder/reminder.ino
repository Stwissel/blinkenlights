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
  strip.show();
}

void loop() {
  

}

/**
 * Show the strip and prep for next
 */
void showStrip(int wait) {
      int i;
  for(i = 0; i < numOfPixels; i++) {
      strip.setPixelColor(i, colorPixel[i]);
  }
  strip.show();
  // Need to reset the pixel since
  // setpixelColor seems to be additive?
  // But don't show it
  for(i = 0; i < numOfPixels; i++) {
      strip.setPixelColor(i, 0);
  }
  delay(wait);
}

/**
 * Set all LED to one color
 */
void resetColors(uint32_t color) {
    int i;
  for(i=0; i < numOfPixels; i++) {
    colorPixel[i] = color;
    strip.setPixelColor(i, color);
  }
}
