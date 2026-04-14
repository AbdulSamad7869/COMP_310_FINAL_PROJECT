#include "graphics.h"
#include "kernel_main.h"
#include <stdint.h>

// TEMP input handling (until input.h is ready)
extern int key_state[256];

// You may need to adjust these based on your interrupt.c
#define KEY_LEFT  0x4B   // common scancode for left arrow
#define KEY_RIGHT 0x4D   // common scancode for right arrow

typedef struct {
	int x;
    	int y;
    	int width;
   	int height;
} Player;

// simple delay to slow down the game
void delay() {
	for (volatile int i = 0; i < 10000000; i++);
}

void game_loop() {

	// initialize player (matches your rectangle)
    	Player player;
    	player.x = 462;
    	player.y = 680;
    	player.width = 100;
    	player.height = 80;

    	while (1) {

        	// INPUT (movement)
        	if (key_state[KEY_LEFT]) {
            		player.x -= 5;
        	}

        	if (key_state[KEY_RIGHT]) {
            		player.x += 5;
        	}

        	// BOUNDARY CHECK
        	if (player.x < 0) {
            		player.x = 0;
        	}

        	if (player.x + player.width > getFramebufferWidth()) {
            		player.x = getFramebufferWidth() - player.width;
        	}

	        // RENDER
        	clearScreen(0xff0000); // red background
        	drawRect(player.x, player.y, player.width, player.height, 0xffffff);

	        // DELAY
        	delay();
    	}
}
