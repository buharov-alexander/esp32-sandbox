#include <math.h>

#include "display/RobotEyes.h"
#include "expressions/RobotExpressions.h"

// --------------------------------------------------
// Constructor
// --------------------------------------------------

RobotEyes::RobotEyes(Adafruit_ILI9341& display)
    : tft(display),
      canvas(240, 120) {
}

// --------------------------------------------------
// Initialization
// --------------------------------------------------

void RobotEyes::begin() {

    randomSeed(micros());

    lastBlink = millis();
    lastLook = millis();

    renderEyes();
}

// --------------------------------------------------
// Expression
// --------------------------------------------------

void RobotEyes::setExpression(Expression newExpression) {

    expression = newExpression;
}

// --------------------------------------------------
// Eye movement
// --------------------------------------------------

void RobotEyes::look(int x, int y) {

    targetLookX = constrain(x, -12, 12);
    targetLookY = constrain(y, -10, 10);

    manualLook = true;
}

void RobotEyes::look(Direction direction) {

    switch (direction) {

        case Direction::LEFT:
            look(-12, 0);
            break;

        case Direction::RIGHT:
            look(12, 0);
            break;

        case Direction::UP:
            look(0, -10);
            break;

        case Direction::DOWN:
            look(0, 10);
            break;

        case Direction::CENTER:
            look(0, 0);
            break;
    }
}

// --------------------------------------------------
// Blink
// --------------------------------------------------

void RobotEyes::blink() {

    if (blinkState == BLINK_IDLE) {
        blinkState = BLINK_CLOSING;
    }
}

// --------------------------------------------------
// Draw one eye
// --------------------------------------------------

void RobotEyes::drawEye(
    int x,
    int pupilX,
    int pupilY,
    const EyeStyle& style
) {

    int currentHeight = min(style.height, eyeHeight);

    int offsetY = (EYE_HEIGHT - currentHeight) / 2;

    int eyeX = x + (EYE_WIDTH - style.width) / 2;

    int centerX = eyeX + style.width / 2;
    int centerY = EYE_Y + EYE_HEIGHT / 2;

    // Eye white
    canvas.fillRoundRect(
        eyeX,
        EYE_Y + offsetY,
        style.width,
        currentHeight,
        style.cornerRadius,
        style.eyeColor
    );

    // Pupil position
    int px = centerX + pupilX;
    int py = centerY + pupilY + style.pupilOffsetY;

    // Iris
    canvas.fillCircle(
        px,
        py,
        style.irisRadius,
        style.irisColor
    );

    // Pupil
    canvas.fillCircle(
        px,
        py,
        style.pupilRadius,
        style.pupilColor
    );

    // Reflection
    canvas.fillCircle(
        px - 7,
        py - 8,
        5,
        ILI9341_WHITE
    );
}

// --------------------------------------------------
// Eyebrows
// --------------------------------------------------

void drawThickLine(
    GFXcanvas16& canvas,
    int x0, int y0,
    int x1, int y1,
    uint16_t color,
    int thickness
) {
    float dx = x1 - x0;
    float dy = y1 - y0;

    float length = sqrt(dx * dx + dy * dy);
    int steps = max(1, (int)length);

    int radius = thickness / 2;

    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;

        int x = x0 + dx * t;
        int y = y0 + dy * t;

        canvas.fillCircle(x, y, radius, color);
    }
}

void RobotEyes::drawEyebrows(
    int x,
    const EyeStyle& style
) {

    int y = 8;

    switch (style.eyebrowStyle) {

        case EyebrowStyle::ANGRY:

            if (x == LEFT_EYE_X) {

                drawThickLine(
                    canvas,
                    x,
                    y,
                    x + EYE_WIDTH,
                    y + 12,
                    style.eyebrowColor,
                    6
                );

            } else {

                drawThickLine(
                    canvas,
                    x,
                    y + 12,
                    x + EYE_WIDTH,
                    y,
                    style.eyebrowColor,
                    6
                );
            }

            break;

        case EyebrowStyle::SAD:

            if (x == LEFT_EYE_X) {

                drawThickLine(
                    canvas,
                    x,
                    y + 12,
                    x + EYE_WIDTH,
                    y,
                    style.eyebrowColor,
                    6
                );

            } else {

                drawThickLine(
                    canvas,
                    x,
                    y,
                    x + EYE_WIDTH,
                    y + 12,
                    style.eyebrowColor,
                    6
                );
            }

            break;

        case EyebrowStyle::NONE:
        default:
            break;
    }
}

// --------------------------------------------------
// Render
// --------------------------------------------------

void RobotEyes::renderEyes() {

    canvas.fillScreen(ILI9341_BLACK);

    const EyeStyle& style = RobotExpressions::get(expression);

    drawEyebrows(LEFT_EYE_X, style);
    drawEyebrows(RIGHT_EYE_X, style);

    drawEye(
        LEFT_EYE_X,
        lookX,
        lookY,
        style
    );

    drawEye(
        RIGHT_EYE_X,
        lookX,
        lookY,
        style
    );

    tft.drawRGBBitmap(
        0,
        100,
        canvas.getBuffer(),
        240,
        120
    );
}

// --------------------------------------------------
// Update look
// --------------------------------------------------

void RobotEyes::updateLook() {

    unsigned long now = millis();

    if (!manualLook && now - lastLook > random(2000, 4000)) {

        targetLookX = random(-12, 13);
        targetLookY = random(-10, 11);

        lastLook = now;
    }

    if (lookX < targetLookX) lookX++;
    if (lookX > targetLookX) lookX--;

    if (lookY < targetLookY) lookY++;
    if (lookY > targetLookY) lookY--;
}

// --------------------------------------------------
// Update blink
// --------------------------------------------------

void RobotEyes::updateBlink() {

    switch (blinkState) {

        case BLINK_IDLE:

            if (millis() - lastBlink > random(2500, 5000)) {
                blinkState = BLINK_CLOSING;
            }

            break;

        case BLINK_CLOSING:

            eyeHeight -= 15;

            if (eyeHeight <= 5) {

                eyeHeight = 5;
                blinkState = BLINK_OPENING;
            }

            break;

        case BLINK_OPENING:

            eyeHeight += 15;

            if (eyeHeight >= EYE_HEIGHT) {

                eyeHeight = EYE_HEIGHT;
                blinkState = BLINK_IDLE;

                lastBlink = millis();
            }

            break;
    }
}

// --------------------------------------------------
// Main update
// --------------------------------------------------

void RobotEyes::update() {

    unsigned long now = millis();

    if (now - lastFrame < 33) {
        return;
    }

    lastFrame = now;

    updateLook();
    updateBlink();

    renderEyes();
}