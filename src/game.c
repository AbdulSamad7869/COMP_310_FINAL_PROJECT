#include "graphics.h"
#include "kernel_main.h"
#include <stdint.h>
#include "input.h"
#include "game.h"

// simple delay to slow down the game
void delay() {
	for (volatile int i = 0; i < 1000000; i++);
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

		// DELAY
                delay();

	        // RENDER
        	clearScreen(0xff0000); // red background
        	drawRect(player.x, player.y, player.width, player.height, 0xffffff);
    	}
}
