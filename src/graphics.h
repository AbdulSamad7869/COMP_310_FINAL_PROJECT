// declaration file that includes the methods in graphics.c

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include "kernel_main.h"

void clearScreen(uint32_t color);
void drawRect(int x, int y, int width, int height, uint32_t color);

#endif
