#include "infrastructure.h"

#include "MemoryController.h"

void bindPins() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(LEDS, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(BUTTON_PIN, INPUT);
}

int scanColor(color_index_t index) {
  int value = 0;
  uint8_t s2, s3;
  switch (index)
  {
  case RED_COLOR_INDEX: // red
    s2 = LOW;
    s3 = LOW;
    break;
  case GREEN_COLOR_INDEX: // green
    s2 = HIGH;
    s3 = HIGH;
    break;
  case BLUE_COLOR_INDEX: // blue
    s2 = LOW;
    s3 = HIGH;
    break;
  case CLEAR_COLOR_INDEX:
    s2 = HIGH;
    s3 = LOW;
    break;
  default:
    return value;
  }

  // Setting photodiodes of specified color
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  digitalWrite(LEDS, HIGH);
  delay(50);
  // Reading the output frequency
  value = pulseIn(sensorOut, LOW);
  delay(50);
  digitalWrite(LEDS, LOW);

  return value;
}