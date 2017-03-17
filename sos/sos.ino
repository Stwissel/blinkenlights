#include "LPD8806.h"
#include "SPI.h"

// Parameters to configure a strip
#define dataPin 5
#define clockPin 6
#define numOfPixels 32


LPD8806 strip = LPD8806(numOfPixels, dataPin, clockPin);

// Colors
uint32_t colorPixel[numOfPixels];
uint32_t blue = strip.Color(0, 0, 60);
uint32_t red = strip.Color(90, 0, 0);


void setup() {
  resetColors(0);
  strip.begin();
}

void loop() {
  threeShort(300);
  threeLong(900,400);
  threeShort(300);
  delay(1500);
}

void threeShort(int wait) {
  int i;
  for (i = 0 ; i < 3; i++) {
    colorPixel[2] = red;
    colorPixel[6] = red;
    colorPixel[10] = red;
    colorPixel[14] = red;
    colorPixel[18] = red;
    colorPixel[22] = red;
    colorPixel[26] = red;
    colorPixel[30] = red;        
    showStrip(wait);
    resetColors(0);
    showStrip(wait);
  }
}


void threeLong(int on, int wait) {
int i;
  for (i = 0 ; i < 3; i++) {
    colorPixel[3] = blue;
    colorPixel[4] = blue;
    colorPixel[5] = blue;
    colorPixel[7] = blue;
    colorPixel[8] = blue;
    colorPixel[9] = blue;
    colorPixel[11] = blue;
    colorPixel[12] = blue;
    colorPixel[13] = blue;
    colorPixel[15] = blue;
    colorPixel[16] = blue;
    colorPixel[17] = blue;
    colorPixel[19] = blue;
    colorPixel[20] = blue;
    colorPixel[21] = blue;
    colorPixel[23] = blue;
    colorPixel[24] = blue;
    colorPixel[25] = blue;
    colorPixel[27] = blue;
    colorPixel[28] = blue;
    colorPixel[29] = blue;
    showStrip(on);
    resetColors(0);
    showStrip(wait);
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
