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
int gameStage = 0;
int omoCredit = 0;
bool lastButtonState = LOW;
bool buttonState = LOW;
unsigned long pressTime = 0;

BetterButton buttonFlipR(buttonPinR, 1);
BetterButton buttonFlipL(buttonPinL, 2);
BetterButton buttonStart(buttonPinM, 3);
BetterButton buttonConfirm(buttonPinM, 4);
BetterButton buttonPlay(buttonPinL, 5);
BetterButton buttonHug(buttonPinR, 6);
BetterButton buttonRead(buttonPinM, 7);

#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 128  // OLED display height, in pixels
#define OLED_RESET -1      // can set an oled reset pin if desired
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

void setup() {
  pinMode(buttonPinR, INPUT);
  Serial.begin(9600);

  delay(250);
  display.begin(0x3D, true);

  buttonFlipR.pressHandler(switchChannelForward);
  buttonFlipL.pressHandler(switchChannelBackward);
  buttonStart.pressHandler(switchChannelStage1);
  buttonConfirm.pressHandler(switchChannelStage2);
  buttonPlay.pressHandler(switchChannelStage3);
  buttonHug.pressHandler(switchChannelStage4);
  buttonRead.pressHandler(switchChannelStage5);
}

void displayStartScreen() {
  display.clearDisplay();
  if (++framecount > 2) framecount = 0;
  display.drawBitmap(0, 0, start[framecount], 128, 128, 1);
  display.display();
  delay(150);
  delay(5);
  buttonStart.process();
}

void checkChannelPics() {
  if (channelDisplay == 0) {
    display.clearDisplay();
    if (++framecount > 2) framecount = 0;
    display.drawBitmap(0, 0, omori[framecount], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  } else if (channelDisplay == 1) {
    display.clearDisplay();
    if (++framecount > 2) framecount = 0;
    display.drawBitmap(0, 0, aubrey[framecount], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  } else if (channelDisplay == 2) {
    display.clearDisplay();
    if (++framecount > 2) framecount = 0;
    display.drawBitmap(0, 0, basil[framecount], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  }
}

void switchChannelForward() {
  pressTime = millis();
  display.clearDisplay();
  channelDisplay = channelDisplay + 1;
  if (channelDisplay > 2) {
    channelDisplay = 0;
  }
}

void switchChannelBackward() {
  pressTime = millis();
  display.clearDisplay();
  channelDisplay = channelDisplay - 1;
  if (channelDisplay < 0) {
    channelDisplay = 2;
  }
}

void characterSelection() {
  buttonFlipR.process();
  buttonFlipL.process();
  buttonConfirm.process();
}

void omoriPage() {

  if (omoCredit < 3) {
    display.clearDisplay();
    if (++framecount > 2) framecount = 0;
    display.drawBitmap(0, 0, omoPage_1[framecount], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  }

  if (omoCredit > 2 and omoCredit < 30) {
    display.clearDisplay();
    if (++framecount > 2) framecount = 0;
    display.drawBitmap(0, 0, omoPage_2[framecount], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  }

  if (omoCredit > 29) {
    display.clearDisplay();
    if (++framecount > 2) framecount = 0;
    display.drawBitmap(0, 0, omoPage_3[framecount], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  }

  if (millis() - pressTime > 60000 and millis() - pressTime < 300000) {
    omoCredit = 3;
  }

  if (millis() - pressTime > 300000) {
    omoCredit = 0;
  }

  buttonPlay.process();
  buttonHug.process();
  buttonRead.process();
}

void playViolin() {
  display.clearDisplay();
  omoCredit = omoCredit + 5;
  for (int i = 0; i < 8; i++) {
    display.clearDisplay();
    display.drawBitmap(0, 0, violin[i], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  }
  gameStage = 2;
}

void playHug() {
  display.clearDisplay();
  omoCredit = omoCredit + 10;
  for (int i = 0; i < 15; i++) {
    display.clearDisplay();
    display.drawBitmap(0, 0, hug[i], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  }

  gameStage = 2;
}

void playRead() {
  display.clearDisplay();
  omoCredit = omoCredit + 5;
  for (int i = 0; i < 13; i++) {
    display.clearDisplay();
    display.drawBitmap(0, 0, read[i], 128, 128, 1);
    display.display();
    delay(150);
    delay(5);
  }

  gameStage = 2;
}

void switchChannelStage1() {
  pressTime = millis();
  gameStage = 1;
  delay(200);
}

void switchChannelStage2() {
  pressTime = millis();
  if (channelDisplay == 0) {
    gameStage = 2;
  }
}

void switchChannelStage3() {
  pressTime = millis();
  gameStage = 3;
}

void switchChannelStage4() {
  pressTime = millis();
  gameStage = 4;
}

void switchChannelStage5() {
  pressTime = millis();
  gameStage = 5;
}

void loop() {
  if (gameStage == 0) {
    displayStartScreen();
  }

  else if (gameStage == 1) {
    characterSelection();
    checkChannelPics();
  }

  else if (gameStage == 2) {
    omoriPage();
  }

  else if (gameStage == 3) {
    playViolin();
  }

  else if (gameStage == 4) {
    playHug();
  }

  else if (gameStage == 5) {
    playRead();
  }
}
