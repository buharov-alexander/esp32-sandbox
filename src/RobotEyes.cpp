#include "RobotEyes.h"

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
    int y,
    int pupilX,
    int pupilY,
    int height
) {

    int currentHeight = height;
    int currentWidth = EYE_WIDTH;

    switch (expression) {

        case Expression::HAPPY:
            currentHeight = min(currentHeight, 48);
            break;

        case Expression::SLEEPY:
            currentHeight = min(currentHeight, 38);
            break;

        case Expression::SURPRISED:
            currentHeight = min(EYE_HEIGHT, currentHeight + 15);
            currentWidth = EYE_WIDTH - 4;
            break;

        default:
            break;
    }

    int offsetY = (EYE_HEIGHT - currentHeight) / 2;

    int eyeX = x + (EYE_WIDTH - currentWidth) / 2;

    // Almost closed eye
    if (currentHeight < 15) {

        canvas.drawFastHLine(
            eyeX + 5,
            y + EYE_HEIGHT / 2,
            currentWidth - 10,
            ILI9341_WHITE
        );

        return;
    }

    // Eye white
    canvas.fillRoundRect(
        eyeX,
        y + offsetY,
        currentWidth,
        currentHeight,
        25,
        ILI9341_WHITE
    );

    int centerX = x + EYE_WIDTH / 2;
    int centerY = y + EYE_HEIGHT / 2;

    pupilX = constrain(pupilX, -12, 12);
    pupilY = constrain(pupilY, -10, 10);

    int irisRadius = 23;
    int pupilRadius = 13;

    if (expression == Expression::SURPRISED) {
        irisRadius = 25;
        pupilRadius = 14;
    }

    // Iris
    canvas.fillCircle(
        centerX + pupilX,
        centerY + pupilY,
        irisRadius,
        ILI9341_CYAN
    );

    // Pupil
    canvas.fillCircle(
        centerX + pupilX,
        centerY + pupilY,
        pupilRadius,
        ILI9341_BLACK
    );

    // Reflection
    canvas.fillCircle(
        centerX + pupilX - 7,
        centerY + pupilY - 8,
        5,
        ILI9341_WHITE
    );
}

// --------------------------------------------------
// Eyebrows
// --------------------------------------------------

void RobotEyes::drawEyebrows(int x, int y) {

    int width = EYE_WIDTH;

    switch (expression) {

        case Expression::ANGRY:

            if (x == LEFT_EYE_X) {

                canvas.drawLine(
                    x, y + 5,
                    x + width, y + 20,
                    ILI9341_CYAN
                );

            } else {

                canvas.drawLine(
                    x, y + 20,
                    x + width, y + 5,
                    ILI9341_CYAN
                );
            }

            break;

        case Expression::SAD:

            if (x == LEFT_EYE_X) {

                canvas.drawLine(
                    x, y + 15,
                    x + width, y + 3,
                    ILI9341_CYAN
                );

            } else {

                canvas.drawLine(
                    x, y + 3,
                    x + width, y + 15,
                    ILI9341_CYAN
                );
            }

            break;

        default:
            break;
    }
}

// --------------------------------------------------
// Render eyes
// --------------------------------------------------

void RobotEyes::renderEyes() {

    canvas.fillScreen(ILI9341_BLACK);

    drawEye(
        LEFT_EYE_X,
        EYE_Y,
        lookX,
        lookY,
        eyeHeight
    );

    drawEye(
        RIGHT_EYE_X,
        EYE_Y,
        lookX,
        lookY,
        eyeHeight
    );

    drawEyebrows(LEFT_EYE_X, EYE_Y);
    drawEyebrows(RIGHT_EYE_X, EYE_Y);

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