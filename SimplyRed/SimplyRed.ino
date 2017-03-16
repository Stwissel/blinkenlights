#include "LPD8806.h"
#include "SPI.h"

// Parameters to configure a strip
int dataPin = 5;
int clockPin = 6;
int numOfPixels = 32; 

LPD8806 strip = LPD8806(numOfPixels, dataPin, clockPin);


void setup() {
   strip.begin();
  strip.show();
}

void loop() {
  int i;
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Color(127,0,0));
    strip.show();
    // Reset but don't show
    strip.setPixelColor(i, 0);
    delay(100);
  }
}


/* Utility functions */
/* Helper functions */
// Create a 24 bit color value from R,G,B
uint32_t Color(byte g, byte r, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
