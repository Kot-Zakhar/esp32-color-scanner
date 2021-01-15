#include <Arduino.h>
#include <BluetoothSerial.h>
// todo: add calibration on default colors

#define S0 12
#define S1 14
#define S2 27
#define S3 26
#define LEDS 25
#define sensorOut 13

#define BUTTON_PIN 33

#define EOL '\0'

#define RED_COLOR_INDEX 0
#define GREEN_COLOR_INDEX 1
#define BLUE_COLOR_INDEX 2

BluetoothSerial BT;

int srcRedLow = 0, srcGreenLow = 0, srcBlueLow = 0;
int srcRedHigh = 1000, srcGreenHigh = 1000, srcBlueHigh = 1000;
int destLow = 0, destHigh = 255;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(LEDS, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  // Setting frequency-scaling to 100%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  // Turning sensor's leds off
  digitalWrite(LEDS, LOW);

  Serial.begin(115200);

  BT.begin("Color sensor esp32");
  BT.setTimeout(60000);
}

void scanColor(int index, int *value) {
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
  default:
    return;
  }

  // Setting photodiodes of specified color
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  digitalWrite(LEDS, HIGH);
  delay(50);
  // Reading the output frequency
  *value = pulseIn(sensorOut, LOW);
  delay(50);
  digitalWrite(LEDS, LOW);
}

void scanColors(int *r, int *g, int *b) {
  scanColor(RED_COLOR_INDEX, r);
  Serial.printf("R= %d ", *r);

  scanColor(GREEN_COLOR_INDEX, g);
  Serial.printf("G= %d ", *g);

  scanColor(BLUE_COLOR_INDEX, b);
  Serial.printf("B= %d ", *b);

  Serial.println();
}

int mapColor(int index, int value) {
  int srcLow, srcHigh;
  switch (index)
  {
  case RED_COLOR_INDEX:
    srcLow = srcRedLow;
    srcHigh = srcRedHigh;
    break;
  case GREEN_COLOR_INDEX:
    srcLow = srcGreenLow;
    srcHigh = srcGreenHigh;
    break;
  case BLUE_COLOR_INDEX:
    srcLow = srcBlueLow;
    srcHigh = srcBlueHigh;
    break;
  default:
    return 0;
  }

  return (int)((float)(value - srcLow) / (srcHigh - srcLow) * (destHigh - destLow)) + destLow;
}

void calibration() {
  BT.println("Write 'white'/'black'/'red'/'green'/'blue' to scan a calibration color.\nWrite 'end' to stop calibration.");
  String input;
  do {
    input = BT.readStringUntil(EOL);
    if (input == "white" || input == "w") {
      scanColors(&srcRedHigh, &srcGreenHigh, &srcBlueHigh);
      BT.printf("{\"r\": %d, \"g\": %d, \"b\": %d}\n", srcRedHigh, srcGreenHigh, srcBlueHigh);
    }
    if (input == "black" || input == "b") {
      scanColors(&srcRedLow, &srcGreenLow, &srcBlueLow);
      BT.printf("{\"r\": %d, \"g\": %d, \"b\": %d}\n", srcRedLow, srcGreenLow, srcBlueLow);
    }
    if (input == "red") {
      scanColor(RED_COLOR_INDEX, &srcRedHigh);
      BT.println(srcRedHigh);
    }
    if (input == "green") {
      scanColor(GREEN_COLOR_INDEX, &srcGreenHigh);
      BT.println(srcGreenHigh);
    }
    if (input == "blue") {
      scanColor(BLUE_COLOR_INDEX, &srcBlueHigh);
      BT.println(srcBlueHigh);
    }
  } while (input != "end" && input != "");
  BT.println("Exiting calibration.");
}

void manualCalibration() {
  BT.println("Manual calibration.");
  
  BT.println("red low value:");
  srcRedLow = BT.readStringUntil(EOL).toInt();
  BT.println("red high value:");
  srcRedHigh = BT.readStringUntil(EOL).toInt();

  BT.println("green low value:");
  srcGreenLow = BT.readStringUntil(EOL).toInt();
  BT.println("green high value:");
  srcGreenHigh = BT.readStringUntil(EOL).toInt();

  BT.println("blue low value:");
  srcBlueLow = BT.readStringUntil(EOL).toInt();
  BT.println("blue high value:");
  srcBlueHigh = BT.readStringUntil(EOL).toInt();

  BT.println("Manual calibration done.");
}

void scanAndMap() {
  int r, g, b;

  scanColors(&r, &g, &b);
  r = mapColor(RED_COLOR_INDEX, r);
  g = mapColor(GREEN_COLOR_INDEX, g);
  b = mapColor(BLUE_COLOR_INDEX, b);

  BT.printf("{\"r\": %d, \"g\": %d, \"b\": %d}\n", r, g, b);
}

void scan() {
  int r, g, b;
  scanColors(&r, &g, &b);
  BT.printf("{\"r\": %d, \"g\": %d, \"b\": %d}\n", r, g, b);
}



void loop() {
  if (BT.available()) {
    String req = BT.readStringUntil(EOL);

    if (req == "calibrate" || req == "c") {
      calibration();
    } else if (req == "manual" || req == "m") {
      manualCalibration();
    } else if (req == "scan" || req == "s") {
      scanAndMap();
    } else if (req == "scan real" || req == "r") {
      scan();
    } else {
      BT.println(
        "'calibrate'/'c' - calibration mode\n"
        "'manual'/'m' - manual calibration mode\n"
        "'scan'/'s' - scan a color and map it to 0..255\n"
        "'scan real'/'r' - scan a color without mapping\n"
      );
    }
  }

  if (BT.connected() && digitalRead(BUTTON_PIN)) {
    scanAndMap();
  }
}