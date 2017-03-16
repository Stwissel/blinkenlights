#include "LPD8806.h"
#include "SPI.h"

// Parameters to configure a strip
int dataPin = 5;
int clockPin = 6;
int numOfPixels = 32;

// Colors 
uint32_t red;

LPD8806 strip = LPD8806(numOfPixels, dataPin, clockPin);


void setup() {
  red = Color(127,0,0);
   strip.begin();
  strip.show();
}

void loop() {

  loopup(70, 4, red);
  loopdown(70, 4, red);
}

/* Looping up some pixels in a row
 */
void loopup(int wait, int howMany, uint32_t color) {
  int i;
  for (i=0; i < strip.numPixels()-howMany; i++) {
    setBlock(i, howMany, color);    
    strip.show();
    // Reset but don't show
    setBlock(i, howMany, 0);
    delay(wait);
  }
 }
/* Looping up 4 pixels in a row
 */
void loopdown(int wait, int howMany, uint32_t color) {
  int i;
  for (i = strip.numPixels() - howMany; i > -1; i--) {
    setBlock(i, howMany, color);    
    strip.show();
    // Reset but don't show
    setBlock(i, howMany, 0);
    delay(wait);
  }
 }
 

/* Sets a block of 4 pixels to one color */
void setBlock(int start, int howMany, uint32_t color) {
  int i;
  for (i=0; i < howMany; i++) {
    strip.setPixelColor(start+i, color);  
  }
}


/* Utility functions */
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
