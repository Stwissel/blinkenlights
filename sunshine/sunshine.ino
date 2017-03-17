#include "LPD8806.h"
#include "SPI.h"

// Parameters to configure a strip
#define dataPin 5
#define clockPin 6
#define numOfPixels 32


LPD8806 strip = LPD8806(numOfPixels, dataPin, clockPin);

// Colors
uint32_t colorPixel[numOfPixels];
uint32_t orange = strip.Color(90, 15, 0);
uint32_t yellow = strip.Color(127, 127, 0);
uint32_t gray = strip.Color(30, 20, 30);


void setup() {
  resetColors(0);
  strip.begin();
  spreadColor(50, orange, 0);
}

void loop() {
  int loopie;
  for (loopie = 0; loopie < 100; loopie++) {
    int raindrop = random(numOfPixels);
    if (colorPixel[raindrop] == orange) {
      int cpic = random(2);
      if (cpic == 0) {
        colorPixel[raindrop] = yellow;
      } else {
        colorPixel[raindrop] = gray;
      }
    } else {
      colorPixel[raindrop] = orange;
    }
    showStrip(10);
  }
  pushColorFromCenter(100, orange, 10);
  pushColorFromCenter(100, yellow, 3);
  pushColorFromCenter(100, gray, 10);
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
