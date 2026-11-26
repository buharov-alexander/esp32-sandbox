#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "RobotEyes.h"

#define TFT_DC 5
#define TFT_CS 15

Adafruit_ILI9341 tft(TFT_CS, TFT_DC);

RobotEyes eyes(tft);

void setup() {

    Serial.begin(115200);

    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(ILI9341_BLACK);

    eyes.begin();

    Serial.println("Robot initialized!");
}

void loop() {

    eyes.update();

}