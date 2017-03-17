#include "LPD8806.h"
#include "SPI.h"

// Parameters to configure a strip
#define dataPin 5
#define clockPin 6
#define numOfPixels 32
#define exmarksize 5

// Colors
uint32_t colorPixel[numOfPixels];

LPD8806 strip = LPD8806(numOfPixels, dataPin, clockPin);

// Exclamation mark
int exmk[] = {1, 3, 4, 5, 6};
int j = 0;
int toggle = 0;

void setup() {
  resetColors(0);
  strip.begin();
  strip.show();
}

void loop() {
  int i;
  uint32_t colori[2];
  colori[0] = strip.Color(127, 127, 0);
  colori[1] = strip.Color(127, 0, 0);

  if (j < 2) {
    for (i = 0; i < (exmarksize + 3) * 3; i++) {
      shiftPixelUp(0);
      showStrip(10);
    }
    showStrip(500);
    exMark(500, colori[j]);
    j++;
  } else {
    if (toggle == 0) {
      toggle = 1;
      for (i = numOfPixels; i > (numOfPixels - 8); i--) {
        strip.setPixelColor(i, 0);
      }
      strip.show();
      delay(500);
    } else {
      toggle = 0;
      for (i = 0; i < 7; i++) {
        strip.setPixelColor(i, 0);
      }
      strip.show();
      delay(500);
    }

  }
  showStrip(1000);
}


void exMark(int wait, uint32_t color) {
  int i;

  for (i = 0; i < exmarksize; i++) {
    int count = exmk[i];
    colorPixel[count] = color;
  }
  showStrip(wait);
}

void shiftPixelUp(int startPixel) {
  int i;
  for (i = numOfPixels - 1; i > startPixel; i--) {
    colorPixel[i] = colorPixel[i - 1];
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
