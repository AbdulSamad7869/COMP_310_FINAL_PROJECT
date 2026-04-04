#include "graphics.h"

// since we don't have a drawing function to clear the screen, we can only blank it out with a color
// go through the frame buffer width and height and "add" that color to every pixel to clear it 
void clearScreen(uint32_t color) {
	for (int x = 0; x < getFramebufferWidth(); x++) {
		for (int y = 0; y < getFramebufferHeight(); y++) {
			drawPixel(x, y, color);
		}
	}
}

// draws a filled rectangle on the screen at a given x,y postion
// x is the horizontal starting point aka how far left from the edge of the screen
// y is the vertical starting point aka how gar from the top of the screen
// width and height refer to how wide and tall the rectangle is
// loops MUST include the starting width and heights (x and y)!!!!!
void drawRect(int x, int y, int width, int height, uint32_t color) {
	for (int i = x; i < x + width; i++) {
		for (int j = y; j < y + height; j++) {
			drawPixel(i, j, color);
		}
	}
}
