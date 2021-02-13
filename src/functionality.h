#ifndef _FUNCTIONALITY_H_
#define _FUNCTIONALITY_H_

#include "ColorBoundaries.h"
#include "ColorIndexes.h"

void initDeviceFunctionality();

void setButton(bool evailable);
void toggleButton();
bool isButtonEvailable();

void setCurrentBoundary(color_index_t index, color_boundary_t boundary, int value);
int *getCurrentBoundaryPtr(color_index_t index, color_boundary_t boundary);
int getCurrentBoundary(color_index_t index, color_boundary_t boundary);

int getDefaultBoundary(color_index_t index, color_boundary_t boundary);
void dropDefaultBoundaries();

void restoreBoundaries();
int mapColor(color_index_t index, int value);
void saveAllBoundaryColors();
void scanColors(int *r, int *g, int *b, int *clr);
void scanColorsAndMap(int *r, int *g, int *b, int *clr);
void saveAllBoundaryColors();

#endif