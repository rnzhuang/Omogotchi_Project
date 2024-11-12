#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "BetterButton.h"
#include "Stepper.h"
#include "Bitmaps.h"

int buttonPin1 = 24;
int channelDisplay = 0;
bool lastButtonState = LOW;
bool buttonState = LOW;

BetterButton button1(buttonPin1, 1);
Stepper stepper1(4, 10);

#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 128  // OLED display height, in pixels
#define OLED_RESET -1      // can set an oled reset pin if desired
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

#define LOGO_HEIGHT 106
#define LOGO_WIDTH 106

void setup() {
  pinMode(buttonPin1, INPUT);
  Serial.begin(9600);

  delay(250);
  display.begin(0x3D, true);

  button1.pressHandler(switchChannelForward);
}

void checkChannelPics() {
  if (channelDisplay == 0) {
    display.clearDisplay();
    display.drawBitmap(
      (display.width() - LOGO_WIDTH) / 2,
      (display.height() - LOGO_HEIGHT) / 2,
      omori, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
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

void switchChannelForward(int b) {
  display.clearDisplay();
  channelDisplay = channelDisplay + 1;
  if (channelDisplay > 1) {
    channelDisplay = 0;
  }
}


void loop() {
  button1.process();
  checkChannelPics();
}
