#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "expressions/RobotExpression.h"

class RobotEyes {

public:

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

    void renderEyes();

    void drawEye(
        int x,
        int pupilX,
        int pupilY,
        const EyeStyle& style
    );

    void drawEyebrows(
        int x,
        const EyeStyle& style
    );

    void updateLook();
    void updateBlink();

};