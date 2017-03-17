#include "LPD8806.h"
#include "SPI.h"

// Parameters to configure a strip
#define dataPin 5
#define clockPin 6
#define numOfPixels 32


LPD8806 strip = LPD8806(numOfPixels, dataPin, clockPin);

// Colors
uint32_t colorPixel[numOfPixels];
uint32_t blue = strip.Color(0,0,30);
uint32_t lightblue = strip.Color(0,0,120);
uint32_t gray = strip.Color(30,20,30);


void setup() {
  resetColors(0);
  strip.begin();
  spreadColor(600, blue,0);
}

void loop() {
  int raindrop = random(numOfPixels);
  if (colorPixel[raindrop] == blue) {
    int cpic = random(2);
    if (cpic == 0) {
      colorPixel[raindrop] = lightblue;
    } else {
      colorPixel[raindrop] = gray;
    }
  } else {
    colorPixel[raindrop] = blue;
  }
  showStrip(10);
}

void spreadColor(int wait, uint32_t color, int edgelimit) {
  int spreadsize = numOfPixels / 2;
  int i;
  for (i = 0; i < (spreadsize - edgelimit); i++) {
    colorPixel[spreadsize+i] = color;
    colorPixel[spreadsize-(i+1)] = color;
    showStrip(wait);
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
