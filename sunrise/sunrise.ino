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
  sunrise();
  delay(1000);
  resetColors(0);
  showStrip(50);
}

void sunrise() {

  int spreadspeed = 350;
  // Phase 1 dawn simulation Blue spreads
  spreadColor(spreadspeed, strip.Color(0,0,1), 0);
  pushColorFromCenter(spreadspeed,strip.Color(0,0,32),2);
  pushColorFromCenter(spreadspeed,strip.Color(0,0,96),4);
  // Phase 2 Red 
  pushColorFromCenter(spreadspeed,strip.Color(50,0,0),4);
  // Phase 3 Orange brightening
  pushColorFromCenter(spreadspeed,strip.Color(70,45,0),2);
  pushColorFromCenter(spreadspeed,strip.Color(102,66,0),2);
  pushColorFromCenter(spreadspeed,strip.Color(119,77,0),4);
  //White
  //pushColorFromCenter(spreadspeed,strip.Color(127,127,127),4);
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

/*
 * Adds new color in the middle of the strip
 * "pushes" current values out towards the edge
 */
void pushColorFromCenter(int wait, uint32_t color, int howMany) {
  int spreadsize = numOfPixels / 2;
  int i;
   for (i = 0; i < howMany; i++) {
    shiftPixelUp(spreadsize);
    shiftPixelDown(spreadsize-1); 
    colorPixel[spreadsize+i] = color;
    colorPixel[spreadsize-(i+1)] = color;
    showStrip(wait);
  }
}

void shiftPixelUp(int startPixel) {
  int i;
  for(i = numOfPixels-1; i > startPixel; i--) {
     colorPixel[i] = colorPixel[i-1];
  }
}

void shiftPixelDown(int startPixel) {
 int i;
  for(i = 0; i < startPixel; i++) {
   colorPixel[i] = colorPixel[i+1];
  }      
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

