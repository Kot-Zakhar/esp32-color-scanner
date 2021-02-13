#include "functionality.h"

#include "MemoryController.h"
#include "infrastructure.h"

int srcBoundaries[4][2] {
  {color_boundary_default_value[0], color_boundary_default_value[1]},
  {color_boundary_default_value[0], color_boundary_default_value[1]},
  {color_boundary_default_value[0], color_boundary_default_value[1]},
  {color_boundary_default_value[0], color_boundary_default_value[1]}
};

int dstBoundary[2] {
  0, 255
};

void initDeviceFunctionality() {
  initMemoryController();
}

void setButton(bool available) {
  setButtonAvailability(available);
}

void toggleButton() {
  setButtonAvailability(!getButtonAvailability());
}

bool isButtonEvailable() {
  return getButtonAvailability();
}

void setCurrentBoundary(color_index_t index, color_boundary_t boundary, int value) {
  srcBoundaries[index][boundary] = value;
}

int *getCurrentBoundaryPtr(color_index_t index, color_boundary_t boundary) {
  return &srcBoundaries[index][boundary];
}

int getCurrentBoundary(color_index_t index, color_boundary_t boundary) {
  return srcBoundaries[index][boundary];
}

int getDefaultBoundary(color_index_t index, color_boundary_t boundary) {
  return getColorBoundaryFromMem(index, boundary);
}

void restoreBoundaries() {
  Serial.println("Restoring color boundaries from mem.");
  srcBoundaries[RED_COLOR_INDEX][LOW_COLOR_BOUNDARY] = getColorBoundaryFromMem(RED_COLOR_INDEX, LOW_COLOR_BOUNDARY);
  srcBoundaries[GREEN_COLOR_INDEX][LOW_COLOR_BOUNDARY] = getColorBoundaryFromMem(GREEN_COLOR_INDEX, LOW_COLOR_BOUNDARY);
  srcBoundaries[BLUE_COLOR_INDEX][LOW_COLOR_BOUNDARY] = getColorBoundaryFromMem(BLUE_COLOR_INDEX, LOW_COLOR_BOUNDARY);
  srcBoundaries[CLEAR_COLOR_INDEX][LOW_COLOR_BOUNDARY] = getColorBoundaryFromMem(CLEAR_COLOR_INDEX, LOW_COLOR_BOUNDARY);
  srcBoundaries[RED_COLOR_INDEX][HIGH_COLOR_BOUNDARY] = getColorBoundaryFromMem(RED_COLOR_INDEX, HIGH_COLOR_BOUNDARY);
  srcBoundaries[GREEN_COLOR_INDEX][HIGH_COLOR_BOUNDARY] = getColorBoundaryFromMem(GREEN_COLOR_INDEX, HIGH_COLOR_BOUNDARY);
  srcBoundaries[BLUE_COLOR_INDEX][HIGH_COLOR_BOUNDARY] = getColorBoundaryFromMem(BLUE_COLOR_INDEX, HIGH_COLOR_BOUNDARY);
  srcBoundaries[CLEAR_COLOR_INDEX][HIGH_COLOR_BOUNDARY] = getColorBoundaryFromMem(CLEAR_COLOR_INDEX, HIGH_COLOR_BOUNDARY);
}

void dropDefaultBoundaries() {
  Serial.println("Dropping color boundaries from mem.");
  dropColorBoundary(RED_COLOR_INDEX, LOW_COLOR_BOUNDARY);
  dropColorBoundary(GREEN_COLOR_INDEX, LOW_COLOR_BOUNDARY);
  dropColorBoundary(BLUE_COLOR_INDEX, LOW_COLOR_BOUNDARY);
  dropColorBoundary(CLEAR_COLOR_INDEX, LOW_COLOR_BOUNDARY);
  dropColorBoundary(RED_COLOR_INDEX, HIGH_COLOR_BOUNDARY);
  dropColorBoundary(GREEN_COLOR_INDEX, HIGH_COLOR_BOUNDARY);
  dropColorBoundary(BLUE_COLOR_INDEX, HIGH_COLOR_BOUNDARY);
  dropColorBoundary(CLEAR_COLOR_INDEX, HIGH_COLOR_BOUNDARY);
}

int mapColor(color_index_t index, int value) {
  int *srcBoundary = srcBoundaries[index];

  return (int)((float)(value - srcBoundary[LOW_COLOR_BOUNDARY]) /
    (srcBoundary[HIGH_COLOR_BOUNDARY] - srcBoundary[LOW_COLOR_BOUNDARY]) * (dstBoundary[HIGH_COLOR_BOUNDARY] - dstBoundary[LOW_COLOR_BOUNDARY]))
    + dstBoundary[LOW_COLOR_BOUNDARY];
}

void scanColors(int *r, int *g, int *b, int *clr) {
  *r = scanColor(RED_COLOR_INDEX);
  Serial.printf("R= %d ", *r);

  *g = scanColor(GREEN_COLOR_INDEX);
  Serial.printf("G= %d ", *g);

  *b = scanColor(BLUE_COLOR_INDEX);
  Serial.printf("B= %d ", *b);

  *clr = scanColor(CLEAR_COLOR_INDEX);
  Serial.printf("CLR= %d ", *clr);

  Serial.println();
}

void scanColorsAndMap(int *r, int *g, int *b, int *clr) {
  scanColors(r, g, b, clr);
  *r = mapColor(RED_COLOR_INDEX, *r);
  *g = mapColor(GREEN_COLOR_INDEX, *g);
  *b = mapColor(BLUE_COLOR_INDEX, *b);
  *clr = mapColor(CLEAR_COLOR_INDEX, *clr);
}

void saveAllBoundaryColors() {
  saveColorBoundary(RED_COLOR_INDEX, LOW_COLOR_BOUNDARY, srcBoundaries[RED_COLOR_INDEX][LOW_COLOR_BOUNDARY]);
  saveColorBoundary(GREEN_COLOR_INDEX, LOW_COLOR_BOUNDARY, srcBoundaries[GREEN_COLOR_INDEX][LOW_COLOR_BOUNDARY]);
  saveColorBoundary(BLUE_COLOR_INDEX, LOW_COLOR_BOUNDARY, srcBoundaries[BLUE_COLOR_INDEX][LOW_COLOR_BOUNDARY]);
  saveColorBoundary(CLEAR_COLOR_INDEX, LOW_COLOR_BOUNDARY, srcBoundaries[CLEAR_COLOR_INDEX][LOW_COLOR_BOUNDARY]);
  saveColorBoundary(RED_COLOR_INDEX, HIGH_COLOR_BOUNDARY, srcBoundaries[RED_COLOR_INDEX][HIGH_COLOR_BOUNDARY]);
  saveColorBoundary(GREEN_COLOR_INDEX, HIGH_COLOR_BOUNDARY, srcBoundaries[GREEN_COLOR_INDEX][HIGH_COLOR_BOUNDARY]);
  saveColorBoundary(BLUE_COLOR_INDEX, HIGH_COLOR_BOUNDARY, srcBoundaries[BLUE_COLOR_INDEX][HIGH_COLOR_BOUNDARY]);
  saveColorBoundary(CLEAR_COLOR_INDEX, HIGH_COLOR_BOUNDARY, srcBoundaries[CLEAR_COLOR_INDEX][HIGH_COLOR_BOUNDARY]);
}
