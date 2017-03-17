/*
   Blinkenlight main demo - web server to switch between demos
*/

#include <SPI.h>
#include <WiFi101.h>
#include "LPD8806.h"

// Parameters to configure a strip
#define dataPin 5
#define clockPin 6
#define numOfPixels 32
#define exmarksize 5

// WIFI Settings
char ssid[] = "blinkenlights"; // created AP name
char pass[] = "1234567890";      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
int keyIndex = 0;                // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);

// LED Strip
LPD8806 strip = LPD8806(numOfPixels, dataPin, clockPin);

// Colors
uint32_t colorPixel[numOfPixels];

uint32_t orange = strip.Color(90, 15, 0);
uint32_t yellow = strip.Color(127, 127, 0);
uint32_t lightblue = strip.Color(0, 0, 120);
uint32_t gray = strip.Color(30, 20, 30);
uint32_t blue = strip.Color(0, 0, 60);
uint32_t red = strip.Color(90, 0, 0);
uint32_t green = strip.Color(0, 55, 0);

// Various variables
// Exclamation mark
int exmk[] = {1, 3, 4, 5, 6};
int j = 0;
int toggle = 0;

void setup() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8, 7, 4, 2);

  // Some randomness
  randomSeed(analogRead(0));

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  strip.begin();
  resetColors(0);
  strip.show();

  // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, keyIndex, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}


void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      byte remoteMac[6];

      // a device has connected to the AP
      Serial.print("Device connected to AP, MAC address: ");
      WiFi.APClientMacAddress(remoteMac);
      Serial.print(remoteMac[5], HEX);
      Serial.print(":");
      Serial.print(remoteMac[4], HEX);
      Serial.print(":");
      Serial.print(remoteMac[3], HEX);
      Serial.print(":");
      Serial.print(remoteMac[2], HEX);
      Serial.print(":");
      Serial.print(remoteMac[1], HEX);
      Serial.print(":");
      Serial.println(remoteMac[0], HEX);
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:application/json");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("{ \"Status\" : \"OK\" }");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if it was a current request
        resetColors(0);
        if (currentLine.endsWith("GET /RD")) {
          randomColors();
        }
        if (currentLine.endsWith("GET /SR")) {
          sunRise();
        }
        if (currentLine.endsWith("GET /RE")) {
          j = 0;
          reminder();
        }
        if (currentLine.endsWith("GET /I")) {
          incomingCall();
        }
        if (currentLine.endsWith("GET /V")) {
          visitors();
        }
        if (currentLine.endsWith("GET /WG")) {
          weatherGood();
        }
        if (currentLine.endsWith("GET /WB")) {
          weatherBad();
        }
        if (currentLine.endsWith("GET /ST")) {
          stocks();
        }
        if (currentLine.endsWith("GET /N")) {
          notification();
        }
        if (currentLine.endsWith("GET /C")) {
          cylons();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

/*
   Show random colors as needed
*/
void randomColors() {
  int i;
  int j;
  for (j = 0; j < 10; j++) {
    for (i = 0; i < numOfPixels; i++) {
      long OnOrOff = random(0, 2);
      if  (OnOrOff == 0L) {
        colorPixel[i] = 0;
      } else {
        uint32_t nextColor = randomColor();
        colorPixel[i] = nextColor;
      }
    }
    showStrip(250);
  }
}

void sunRise() {
  resetColors(0);
  showStrip(50);
  int spreadspeed = 250;
  // Phase 1 dawn simulation Blue spreads
  spreadColor(spreadspeed, strip.Color(0, 0, 1), 0);
  pushColorFromCenter(spreadspeed, strip.Color(0, 0, 32), 2);
  pushColorFromCenter(spreadspeed, strip.Color(0, 0, 96), 4);
  pushColorFromCenter(spreadspeed, strip.Color(50, 0, 50), 4);
  // Phase 2 Red
  pushColorFromCenter(spreadspeed, strip.Color(50, 20, 10), 4);
  pushColorFromCenter(spreadspeed, strip.Color(50, 50, 0), 4);
  // Phase 3 Yellow brightening
  pushColorFromCenter(spreadspeed, strip.Color(70, 45, 20), 2);
  pushColorFromCenter(spreadspeed, strip.Color(102, 66, 30), 2);
  pushColorFromCenter(spreadspeed, strip.Color(119, 77, 40), 4);
  //White
  pushColorFromCenter(spreadspeed, strip.Color(120, 120, 60), 8);
}

void reminder() {
  int k;
  for (k = 0; k < 10; k++) {
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
}

void incomingCall() {
  // Set the phone
  int i;
  for (i = 13; i < 19; i++) {
    colorPixel[i] = strip.Color(0, 0, 50);
  }
  showStrip(100);
  for (i = 0; i < 10; i++) {
    // put your main code here, to run repeatedly:
    colorPixel[11] = 0;
    colorPixel[12] = 0;
    colorPixel[13] = strip.Color(50, 50, 50);
    colorPixel[18] = strip.Color(0, 0, 50);
    colorPixel[19] = strip.Color(50, 50, 50);
    colorPixel[20] = strip.Color(50, 50, 50);
    showStrip(800);
    colorPixel[11] = strip.Color(50, 50, 50);
    colorPixel[12] = strip.Color(50, 50, 50);
    colorPixel[13] = strip.Color(0, 0, 50);
    colorPixel[18] = strip.Color(50, 50, 50);
    colorPixel[19] = 0;
    colorPixel[20] = 0;
    showStrip(800);
  }
}

void visitors() {
  int j;
  for (j = 0; j < 3; j++) {
    int i;
    for (i = 28; i < 31; i++) {
      colorPixel[i] = strip.Color(0, 50, 0);
    }
    showStrip(100);
    announceVisiors(4);
    for (i = 0; i < numOfPixels; i++) {
      colorPixel[numOfPixels - 1] = 0;
      shiftPixelDown(numOfPixels);
      showStrip(50);
    }
  }
}

void announceVisiors(int times) {
  int i;
  for (i = 0; i < times; i++) {
    colorPixel[31] = strip.Color(50, 50, 50);
    colorPixel[27] = 0;
    showStrip(800);
    colorPixel[31] = 0;
    colorPixel[27] = strip.Color(50, 50, 50);
    showStrip(800);
  }
}

void weatherGood() {
  spreadColor(50, orange, 0);
  int i;
  for (i = 0; i < 2; i++) {
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
}

void weatherBad() {
  spreadColor(50, blue, 0);
  int i;
  for (i = 0; i < 300; i++) {
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
    showStrip(5);
  }
}

void stocks() {
  stockStart(5000);
  stockJump(5000);
  stockCollapse(5000);
  resetColors(0);
  showStrip(1000);
}

void notification() {
  int i;
  for (i = 0; i < 3; i++) {
    threeShort(300);
    threeLong(900, 400);
    threeShort(300);
    delay(1500);
  }
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

void cylons() {
  int i;
  for (i = 0; i < 3; i++) {
    loopup(70, 4, red);
    loopdown(70, 4, red);
  }
  resetColors(0);
}

void stockCollapse(int wait) {
  int i;
  for (i = 27; i > 15; i--) {
    colorPixel[i] = 0;
    showStrip(10);
  }
  for (i = 14; i > 8; i--) {
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
  for (i = 21; i < 27; i++) {
    colorPixel[i] = yellow;
    showStrip(20);
  }
  showStrip(wait);
}

void stockStart(int wait) {
  colorPixel[15] = blue;
  int i;
  for (i = 16; i < 21; i++) {
    colorPixel[i] = green;
  }
  colorPixel[21] = gray;
  colorPixel[22] = orange;
  colorPixel[23] = orange;
  showStrip(wait);
}


/** Spread and shift **/
void spreadColor(int wait, uint32_t color, int edgelimit) {
  int spreadsize = numOfPixels / 2;
  int i;
  for (i = 0; i < (spreadsize - edgelimit); i++) {
    colorPixel[spreadsize + i] = color;
    colorPixel[spreadsize - (i + 1)] = color;
    showStrip(wait);
  }
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

/**
   Create one random color value
*/
uint32_t randomColor() {
  long red = random(120);
  long blue = random(120);
  long green = random(120);
  return strip.Color(red, blue, green);
}

/**
   Sends an exclamation mark at the beginning
*/
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

/* Looping up some pixels in a row
*/
void loopup(int wait, int howMany, uint32_t color) {
  int i;
  for (i = 0; i < strip.numPixels() - howMany; i++) {
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
  for (i = 0; i < howMany; i++) {
    strip.setPixelColor(start + i, color);
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
