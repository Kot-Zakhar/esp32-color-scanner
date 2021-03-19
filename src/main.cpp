#include <BluetoothSerial.h>

#include "ColorIndexes.h"
#include "ColorBoundaries.h"

#include "infrastructure.h"
#include "functionality.h"

#define EOL '\0'
#define BT_BUFFER_LENGTH 100

BluetoothSerial BT;

char btBuffer[BT_BUFFER_LENGTH];

void setup() {
  bindPins();

  // Setting frequency-scaling to 100%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  // Turning sensor's leds off
  digitalWrite(LEDS, LOW);

  Serial.begin(115200);

  BT.begin("Color sensor esp32");
  BT.setTimeout(1000);

  initDeviceFunctionality();
  restoreBoundaries();
}


void calibrationProcess() {
  BT.println("Write 'white'/'black'/'red'/'green'/'blue'/'clear' to scan a calibration color.\nWrite 'end'/'e' to stop calibration.");
  String input;
  do {
    input = BT.readStringUntil(EOL);
    if (input == "white" || input == "w") {
      int r, g, b, c;
      scanColors(&r, &g, &b, &c);
      BT.printf("{\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d}", r, g, b, c);
      setCurrentBoundary(RED_COLOR_INDEX, HIGH_COLOR_BOUNDARY, r);
      setCurrentBoundary(GREEN_COLOR_INDEX, HIGH_COLOR_BOUNDARY, g);
      setCurrentBoundary(BLUE_COLOR_INDEX, HIGH_COLOR_BOUNDARY, b);
      setCurrentBoundary(CLEAR_COLOR_INDEX, HIGH_COLOR_BOUNDARY, c);
    }
    if (input == "black" || input == "b") {
      int r, g, b, c;
      scanColors(&r, &g, &b, &c);
      BT.printf("{\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d}", r, g, b, c);
      setCurrentBoundary(RED_COLOR_INDEX, LOW_COLOR_BOUNDARY, r);
      setCurrentBoundary(GREEN_COLOR_INDEX, LOW_COLOR_BOUNDARY, g);
      setCurrentBoundary(BLUE_COLOR_INDEX, LOW_COLOR_BOUNDARY, b);
      setCurrentBoundary(CLEAR_COLOR_INDEX, LOW_COLOR_BOUNDARY, c);
    }
    if (input == "red") {
      int value = scanColor(RED_COLOR_INDEX);
      setCurrentBoundary(RED_COLOR_INDEX, HIGH_COLOR_BOUNDARY, value);
      BT.println(value);
    }
    if (input == "green") {
      int value = scanColor(GREEN_COLOR_INDEX);
      setCurrentBoundary(GREEN_COLOR_INDEX, HIGH_COLOR_BOUNDARY, value);
      BT.println(value);
    }
    if (input == "blue") {
      int value = scanColor(BLUE_COLOR_INDEX);
      setCurrentBoundary(BLUE_COLOR_INDEX, HIGH_COLOR_BOUNDARY, value);
      BT.println(value);
    }
    if (input == "clear") {
      int value = scanColor(CLEAR_COLOR_INDEX);
      setCurrentBoundary(CLEAR_COLOR_INDEX, HIGH_COLOR_BOUNDARY, value);
      BT.println(value);
    }
    if (input == "end" || input == "e")
      input = "";
  } while (input != "");
  BT.println("Exiting calibration.");
}

void manualCalibration() {
  BT.println("Manual calibration.");
  
  BT.println("red low value:");
  setCurrentBoundary(RED_COLOR_INDEX, LOW_COLOR_BOUNDARY, BT.readStringUntil(EOL).toInt());
  BT.println("red high value:");
  setCurrentBoundary(RED_COLOR_INDEX, HIGH_COLOR_BOUNDARY, BT.readStringUntil(EOL).toInt());

  BT.println("green low value:");
  setCurrentBoundary(GREEN_COLOR_INDEX, LOW_COLOR_BOUNDARY, BT.readStringUntil(EOL).toInt());
  BT.println("green high value:");
  setCurrentBoundary(GREEN_COLOR_INDEX, HIGH_COLOR_BOUNDARY, BT.readStringUntil(EOL).toInt());

  BT.println("blue low value:");
  setCurrentBoundary(BLUE_COLOR_INDEX, LOW_COLOR_BOUNDARY, BT.readStringUntil(EOL).toInt());
  BT.println("blue high value:");
  setCurrentBoundary(BLUE_COLOR_INDEX, HIGH_COLOR_BOUNDARY, BT.readStringUntil(EOL).toInt());

  BT.println("clear low value:");
  setCurrentBoundary(CLEAR_COLOR_INDEX, LOW_COLOR_BOUNDARY, BT.readStringUntil(EOL).toInt());
  BT.println("clear high value:");
  setCurrentBoundary(CLEAR_COLOR_INDEX, HIGH_COLOR_BOUNDARY, BT.readStringUntil(EOL).toInt());

  BT.println("Manual calibration done.");
}

void scanMapAndSend() {
  int r, g, b, clr;
  scanColorsAndMap(&r, &g, &b, &clr);
  BT.printf("{\"r\": %d, \"g\": %d, \"b\": %d, \"clr\": %d}", r, g, b, clr);
}

void scanAndSend() {
  int r, g, b, clr;
  scanColors(&r, &g, &b, &clr);
  BT.printf("{\"r\": %d, \"g\": %d, \"b\": %d, \"clr\": %d}", r, g, b, clr);
}

void setButtonAndSend(bool evailable) {
  setButton(evailable);
  BT.printf("button is now %s", isButtonEvailable() ? "on" : "bff");
}

void toggleButtonAndSend() {
  toggleButton();
  BT.printf("button was toggled %s", isButtonEvailable() ? "on" : "bff");
}

void sendCurrentBoundaries() {
  BT.printf("{\nlow: {\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d},\nhigh: {\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d}\n}\n",
    getCurrentBoundary(RED_COLOR_INDEX, LOW_COLOR_BOUNDARY),
    getCurrentBoundary(GREEN_COLOR_INDEX, LOW_COLOR_BOUNDARY),
    getCurrentBoundary(BLUE_COLOR_INDEX, LOW_COLOR_BOUNDARY),
    getCurrentBoundary(CLEAR_COLOR_INDEX, LOW_COLOR_BOUNDARY),
    getCurrentBoundary(RED_COLOR_INDEX, HIGH_COLOR_BOUNDARY),
    getCurrentBoundary(GREEN_COLOR_INDEX, HIGH_COLOR_BOUNDARY),
    getCurrentBoundary(BLUE_COLOR_INDEX, HIGH_COLOR_BOUNDARY),
    getCurrentBoundary(CLEAR_COLOR_INDEX, HIGH_COLOR_BOUNDARY)
  );
}

void sendSavedBoundaries() {
  BT.printf("{\nlow: {\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d},\nhigh: {\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d}\n}\n",
    getDefaultBoundary(RED_COLOR_INDEX, LOW_COLOR_BOUNDARY),
    getDefaultBoundary(GREEN_COLOR_INDEX, LOW_COLOR_BOUNDARY),
    getDefaultBoundary(BLUE_COLOR_INDEX, LOW_COLOR_BOUNDARY),
    getDefaultBoundary(CLEAR_COLOR_INDEX, LOW_COLOR_BOUNDARY),
    getDefaultBoundary(RED_COLOR_INDEX, HIGH_COLOR_BOUNDARY),
    getDefaultBoundary(GREEN_COLOR_INDEX, HIGH_COLOR_BOUNDARY),
    getDefaultBoundary(BLUE_COLOR_INDEX, HIGH_COLOR_BOUNDARY),
    getDefaultBoundary(CLEAR_COLOR_INDEX, HIGH_COLOR_BOUNDARY)
  );
}

void saveBoundariesAndSend() {
  saveAllBoundaryColors();
  BT.printf("done");
}

void restoreBoundariesAndSend() {
  restoreBoundaries();
  BT.printf("done");
}

void dropDefaultBoundariesAndSend() {
  dropDefaultBoundaries();
  BT.printf("done");
}

void sendHelp() {
  BT.printf(
    "'calibrate'/'c' - calibration mode\n"
    "'calibrate manual'/'cm' - manual calibration mode\n"
    "'scan'/'s' - scan a color and map it to 0..255\n"
    "'scan real'/'r' - scan a color without mapping\n"
    "'button on'/'button off'/'button'/'b' - toggle button\n"
    "'coef' - list current boundaries\n"
    "'coef save' - saves boundaries in memory (as default)\n"
    "'coef restore' - restores boundaries from memory (from default)\n"
    "'coef default' - list saved boundaries (default boundaries)\n"
    "'coef defalut clear' - drop saved boundaries to %d..%d\n",
    color_boundary_default_value[0],
    color_boundary_default_value[1]
  );
}

void loop() {
  btBuffer[0] = '\0';
  BT.readBytesUntil(EOL, btBuffer, BT_BUFFER_LENGTH);

  String req = String(btBuffer);
  req.toLowerCase();
  req.trim();

  if (req.length() > 0) {
    if (req == "calibrate" || req == "c") {
      calibrationProcess();
    } else if (req == "calibrate manual" || req == "cm") {
      manualCalibration();
    } else if (req == "scan" || req == "s") {
      scanMapAndSend();
    } else if (req == "scan real" || req == "r") {
      scanAndSend();
    } else if (req == "button off") {
      setButtonAndSend(false);
    } else if (req == "button on") {
      setButtonAndSend(true);
    } else if (req == "button" || req == "b") {
      toggleButtonAndSend();
    } else if (req == "coef") {
      sendCurrentBoundaries();
    } else if (req == "coef save") {
      saveBoundariesAndSend();
    } else if (req == "coef restore") {
      restoreBoundariesAndSend();
    } else if (req == "coef default") {
      sendSavedBoundaries();
    } else if (req == "coef default clear") {
      dropDefaultBoundariesAndSend();
    } else {
      sendHelp();
    }
  }

  if (isButtonEvailable() && BT.connected() && digitalRead(BUTTON_PIN)) {
    scanMapAndSend();
  }
}