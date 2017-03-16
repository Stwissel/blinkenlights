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
  randomSeed(analogRead(0));
  resetColors();
  strip.begin();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
 randomColors();
 showStrip(250);
}

/**
 * Set all LED to off
 */
void resetColors() {
    int i;
  for(i=0; i < numOfPixels; i++) {
    colorPixel[i] = 0;
  }
}

/**
 * Set all colors to random values
 */
void randomColors() {
  int i;
  for(i=0; i < numOfPixels; i++) {
      long OnOrOff = random(0,2);
    if  (OnOrOff == 0L) {
      colorPixel[i] = 0;
    } else {
      uint32_t nextColor = randomColor();
      colorPixel[i] = nextColor;
    }
  }
}

/**
 * Create one random color value
 */
uint32_t randomColor() {
  long red = random(120);
  long blue = random(120);
  long green = random(120);
  return strip.Color(red, blue, green);
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
  // setpixelColor is additive
  // But don't show it
  for(i = 0; i < numOfPixels; i++) {
      strip.setPixelColor(i, 0);
  }
  delay(wait);
}



