#include "expressions/RobotExpressions.h"
#include <Adafruit_ILI9341.h>

namespace {

const EyeStyle NEUTRAL_STYLE = {
    82, 90, 25,
    23, 13,
    0,
    EyebrowStyle::NONE,
    ILI9341_WHITE,
    ILI9341_CYAN,
    ILI9341_BLACK,
    ILI9341_CYAN
};

const EyeStyle HAPPY_STYLE = {
    82, 52, 25,
    23, 13,
    3,
    EyebrowStyle::NONE,
    ILI9341_WHITE,
    ILI9341_CYAN,
    ILI9341_BLACK,
    ILI9341_CYAN
};

const EyeStyle SAD_STYLE = {
    82, 90, 25,
    23, 13,
    4,
    EyebrowStyle::SAD,
    ILI9341_WHITE,
    ILI9341_CYAN,
    ILI9341_BLACK,
    ILI9341_CYAN
};

const EyeStyle ANGRY_STYLE = {
    82, 90, 25,
    23, 13,
    0,
    EyebrowStyle::ANGRY,
    ILI9341_WHITE,
    ILI9341_CYAN,
    ILI9341_BLACK,
    ILI9341_RED
};

const EyeStyle SURPRISED_STYLE = {
    82, 100, 25,
    25, 14,
    0,
    EyebrowStyle::NONE,
    ILI9341_WHITE,
    ILI9341_CYAN,
    ILI9341_BLACK,
    ILI9341_CYAN
};

const EyeStyle SLEEPY_STYLE = {
    82, 38, 25,
    23, 13,
    3,
    EyebrowStyle::NONE,
    ILI9341_WHITE,
    ILI9341_CYAN,
    ILI9341_BLACK,
    ILI9341_CYAN
};

}

const EyeStyle& RobotExpressions::get(Expression expression) {

    switch (expression) {

        case Expression::HAPPY:
            return HAPPY_STYLE;

        case Expression::SAD:
            return SAD_STYLE;

        case Expression::ANGRY:
            return ANGRY_STYLE;

        case Expression::SURPRISED:
            return SURPRISED_STYLE;

        case Expression::SLEEPY:
            return SLEEPY_STYLE;

        case Expression::NEUTRAL:
        default:
            return NEUTRAL_STYLE;
    }
}