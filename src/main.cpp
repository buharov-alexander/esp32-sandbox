#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "display/RobotEyes.h"

#define TFT_DC 5
#define TFT_CS 15

Adafruit_ILI9341 tft(TFT_CS, TFT_DC);

RobotEyes eyes(tft);

unsigned long lastChange = 0;
int currentExpression = 0;

void setup() {

    Serial.begin(115200);

    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(ILI9341_BLACK);

    eyes.begin();
}

void loop() {

    eyes.update();

    if (millis() - lastChange > 4000) {

        lastChange = millis();

        currentExpression = (currentExpression + 1) % 6;

        eyes.setExpression(
            static_cast<Expression>(currentExpression)
        );
    }
}