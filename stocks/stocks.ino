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
uint32_t orange = strip.Color(90, 15, 0);
uint32_t red = strip.Color(90, 0, 0);
uint32_t green = strip.Color(0, 55, 0);
uint32_t yellow = strip.Color(127, 127, 0);
uint32_t gray = strip.Color(30, 20, 30);


void setup() {
  resetColors(0);
  strip.begin();
}

void loop() {
  stockStart(5000);
  stockJump(5000);
  stockCollapse(5000);
  resetColors(0);
  showStrip(1000);
}

void stockCollapse(int wait) {
  int i;
  for (i=27; i > 15; i--) {
    colorPixel[i] = 0;
    showStrip(10);
  }
  for (i=14; i > 8; i--) {
    colorPixel[i] = red;
    showStrip(10);
  }
  colorPixel[8] = gray;
  showStrip(wait);
}

void stockJump(int wait) {
   int i;
   colorPixel[21] = yellow;
   colorPixel[27] = gray;
   showStrip(1000);
   for (i=21; i<27; i++) {
    colorPixel[i] = yellow;
    showStrip(20); 
   }
   showStrip(wait);
}

void stockStart(int wait) {
   colorPixel[15] = blue;
  int i;
  for (i=16; i < 21; i++) {
    colorPixel[i] = green;
  }
  colorPixel[21] = gray;
  colorPixel[22] = orange;
  colorPixel[23] = orange;
  showStrip(wait);
}


/*
   Adds new color in the middle of the strip
   "pushes" current values out towards the edge
*/
void pushColorFromCenter(int wait, uint32_t color, int howMany) {
  int spreadsize = numOfPixels / 2;
  int i;
  for (i = 0; i < howMany; i++) {
    shiftPixelUp(spreadsize);
    shiftPixelDown(spreadsize - 1);
    colorPixel[spreadsize + i] = color;
    colorPixel[spreadsize - (i + 1)] = color;
    showStrip(wait);
  }
}

void spreadColor(int wait, uint32_t color, int edgelimit) {
  int spreadsize = numOfPixels / 2;
  int i;
  for (i = 0; i < (spreadsize - edgelimit); i++) {
    colorPixel[spreadsize + i] = color;
    colorPixel[spreadsize - (i + 1)] = color;
    showStrip(wait);
  }
}


void shiftPixelUp(int startPixel) {
  int i;
  for (i = numOfPixels - 1; i > startPixel; i--) {
    colorPixel[i] = colorPixel[i - 1];
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
