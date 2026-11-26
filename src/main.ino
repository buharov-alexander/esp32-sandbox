#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>

#define TFT_DC 5
#define TFT_CS 15

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    tft.begin();
    tft.setRotation(0);

    tft.fillScreen(ILI9341_BLACK);

    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.setCursor(30, 100);
    tft.println("HELLO ROBOT!");

    Serial.println("TFT initialized!");
}

void loop() {

}