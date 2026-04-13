#ifndef GAME_H
#define GAME_H
#include <stdint.h>
/*
* Player structure:
* Stores position and size of the player rectangle
*/
typedef struct {
	int x; // horizontal position
	int y; // vertical position
	int width; // width of player
	int height; // height of player
} Player;

// delay to slow down the game loop
void delay();

// starts the game loop
void game_loop();

#endif
