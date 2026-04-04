// this file is needed so the other files can access the methods defined in kernel_main.c


#ifndef KERNEL_MAIN_H
#define KERNEL_MAIN_H

#include <stdint.h>

void drawPixel(int x, int y, int color);
uint32_t getFramebufferWidth();
uint32_t getFramebufferHeight();

#endif
