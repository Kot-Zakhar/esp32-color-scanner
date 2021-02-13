#ifndef _INFRASTRUCTURE_H_
#define _INFRASTRUCTURE_H_

#include <Arduino.h>
#include "ColorBoundaries.h"
#include "ColorIndexes.h"

#define S0 27
#define S1 26
#define S2 25
#define S3 33
#define LEDS 32
#define sensorOut 14

#define BUTTON_PIN 35

void bindPins();
int scanColor(color_index_t index);

#endif