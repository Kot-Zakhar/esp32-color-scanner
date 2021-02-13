#ifndef _MEMORY_CONTROLLER_H_
#define _MEMORY_CONTROLLER_H_

#include <Preferences.h>
#include "ColorIndexes.h"
#include "ColorBoundaries.h"
#include "ButtonConfig.h"

void initMemoryController();
void saveColorBoundary(color_index_t colorIndex, color_boundary_t colorBoundaryType, int value);
int getColorBoundaryFromMem(color_index_t colorIndex, color_boundary_t colorBoundaryType);
void dropColorBoundary(color_index_t colorIndex, color_boundary_t colorBoundaryType);

bool getButtonAvailability();
void setButtonAvailability(bool available);
bool toggleButtonAvailability();

#endif