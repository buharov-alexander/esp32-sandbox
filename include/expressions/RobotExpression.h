#pragma once

#include <Arduino.h>

enum class Expression {
    NEUTRAL,
    HAPPY,
    SAD,
    ANGRY,
    SURPRISED,
    SLEEPY
};

enum class EyebrowStyle {
    NONE,
    SAD,
    ANGRY
};

struct EyeStyle {

    // Геометрия глаза
    int width;
    int height;
    int cornerRadius;

    // Геометрия зрачка
    int irisRadius;
    int pupilRadius;

    // Положение зрачка
    int pupilOffsetY;

    // Брови
    EyebrowStyle eyebrowStyle;

    // Цвета
    uint16_t eyeColor;
    uint16_t irisColor;
    uint16_t pupilColor;
    uint16_t eyebrowColor;
};