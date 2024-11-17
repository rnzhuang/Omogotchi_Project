#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "BetterButton.h"
#include "Stepper.h"
#include "Bitmaps.h"

int buttonPinR = 24;
int buttonPinL = 25;
int buttonPinM = 26;
int channelDisplay = 0;
int framecount = 0;
bool lastButtonState = LOW;
bool buttonState = LOW;

BetterButton buttonR(buttonPinR, 1);
BetterButton buttonL(buttonPinL, 2);
Stepper stepper1(4, 10);

#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 128  // OLED display height, in pixels
#define OLED_RESET -1      // can set an oled reset pin if desired
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

#define LOGO_HEIGHT 106
#define LOGO_WIDTH 106

void setup() {
  pinMode(buttonPinR, INPUT);
  Serial.begin(9600);

  delay(250);
  display.begin(0x3D, true);

  buttonR.pressHandler(switchChannelForward);
  buttonL.pressHandler(switchChannelBackward);
}

void checkChannelPics() {
  if (channelDisplay == 0) {
    display.clearDisplay();
    if (++framecount > 2) framecount = 0; 
    display.drawBitmap(0,0, omori[framecount], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  } else if (channelDisplay == 1) {
    display.clearDisplay();
    display.drawBitmap(
      (display.width() - LOGO_WIDTH) / 2,
      (display.height() - LOGO_HEIGHT) / 2,
      aubrey, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(5);
  }
}

void switchChannelForward() {
  display.clearDisplay();
  channelDisplay = channelDisplay + 1;
  if (channelDisplay > 5) {
    channelDisplay = 0;
  }
}

void switchChannelBackward() {
  display.clearDisplay();
  channelDisplay = channelDisplay - 1;
  if (channelDisplay < 0) {
    channelDisplay = 5;
  }
}

void characterSelection(){
  switchChannelForward();
  switchChannelBackward();
}


void loop() {
  buttonR.process();
  buttonL.process();
  checkChannelPics();
}
