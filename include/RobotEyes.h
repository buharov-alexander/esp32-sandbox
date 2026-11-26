#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class RobotEyes {

public:

    enum class Expression {
        NEUTRAL,
        HAPPY,
        SAD,
        ANGRY,
        SURPRISED,
        SLEEPY
    };

    enum class Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        CENTER
    };

    RobotEyes(Adafruit_ILI9341& display);

    void begin();
    void update();

    void look(int x, int y);
    void look(Direction direction);

    void blink();

    void setExpression(Expression expression);

private:

    Adafruit_ILI9341& tft;
    GFXcanvas16 canvas;

    static const int EYE_WIDTH = 82;
    static const int EYE_HEIGHT = 90;

    static const int LEFT_EYE_X = 30;
    static const int RIGHT_EYE_X = 128;

    static const int EYE_Y = 15;

    enum BlinkState {
        BLINK_IDLE,
        BLINK_CLOSING,
        BLINK_OPENING
    };

    Expression expression = Expression::NEUTRAL;

    BlinkState blinkState = BLINK_IDLE;

    int lookX = 0;
    int lookY = 0;

    int targetLookX = 0;
    int targetLookY = 0;

    int eyeHeight = EYE_HEIGHT;

    unsigned long lastBlink = 0;
    unsigned long lastLook = 0;
    unsigned long lastFrame = 0;

    bool manualLook = false;

    void drawEyes();

    void drawEye(
        int x,
        int y,
        int pupilX,
        int pupilY,
        int height
    );

    void drawEyebrows(int x, int y);

    void renderEyes();

    void updateLook();
    void updateBlink();
};