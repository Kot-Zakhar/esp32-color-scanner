#include "MemoryController.h"

const char *color_table_name = "clrs";

const char *color_boundary_prefix = "bndr";

Preferences memory;
char keyBuffer[15];

void initMemoryController() {
    memory.begin(color_table_name);
}

void saveColorBoundary(color_index_t colorIndex, color_boundary_t colorBoundaryType, int value) {
    sprintf(keyBuffer, "%s%d_%d", color_boundary_prefix, colorIndex, colorBoundaryType);
    memory.putInt(keyBuffer, value);
}

int getColorBoundaryFromMem(color_index_t colorIndex, color_boundary_t colorBoundaryType) {
    sprintf(keyBuffer, "%s%d_%d", color_boundary_prefix, colorIndex, colorBoundaryType);
    int value = memory.getInt(keyBuffer, color_boundary_default_value[colorBoundaryType]);
    Serial.printf("%s: %d\n", keyBuffer, value);
    return value;
}

void dropColorBoundary(color_index_t colorIndex, color_boundary_t colorBoundaryType) {
    sprintf(keyBuffer, "%s%d_%d", color_boundary_prefix, colorIndex, colorBoundaryType);
    Serial.printf("dropping %s", keyBuffer);
    memory.remove(keyBuffer);
}

bool getButtonAvailability() {
    sprintf(keyBuffer, "%s_btn", color_boundary_prefix);
    return memory.getBool(keyBuffer, BUTTON_ENABLE_DEFAULT);
}

void setButtonAvailability(bool available) {
    sprintf(keyBuffer, "%s_btn", color_boundary_prefix);
    memory.putBool(keyBuffer, available);
}