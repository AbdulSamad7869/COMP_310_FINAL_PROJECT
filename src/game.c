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

	// enemy array
	Enemy enemies[ENEMY_ROWS][ENEMY_COLS];

	// initializing enemies
	for (int row = 0; row < ENEMY_ROWS; row++) {
    		for (int col = 0; col < ENEMY_COLS; col++) {
        		enemies[row][col].x = ENEMY_START_X + col * (ENEMY_WIDTH + ENEMY_PADDING_X);
        		enemies[row][col].y = ENEMY_START_Y + row * (ENEMY_HEIGHT + ENEMY_PADDING_Y);

        		enemies[row][col].width = ENEMY_WIDTH;
        		enemies[row][col].height = ENEMY_HEIGHT;

        		enemies[row][col].alive = 1;
    		}
	}

	int enemyDirection = 1; // 1 = right, -1 = left

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

		// Add edge dectection
		int moveDown = 0;

		for (int row = 0; row < ENEMY_ROWS; row++) {
    			for (int col = 0; col < ENEMY_COLS; col++) {

        			if (!enemies[row][col].alive) continue;

        			if (enemies[row][col].x <= 0 ||
    				   enemies[row][col].x + enemies[row][col].width >= SCREEN_WIDTH) {

            				moveDown = 1;
        			}
    			}
		}

		// Move enemies
		for (int row = 0; row < ENEMY_ROWS; row++) {
    			for (int col = 0; col < ENEMY_COLS; col++) {

        			if (!enemies[row][col].alive) continue;

        			if (moveDown) {
            				enemies[row][col].y += 20;
        			} else {
            				enemies[row][col].x += enemyDirection * 1; // smoother
        			}
    			}
		}

		if (moveDown) {
    			enemyDirection *= -1;
		}

	        // RENDER
        	clearScreen(0xff0000); // red background
        	drawRect(player.x, player.y, player.width, player.height, 0xffffff);

		// Draw enemies
		for (int row = 0; row < ENEMY_ROWS; row++) {
    			for (int col = 0; col < ENEMY_COLS; col++) {

        			if (enemies[row][col].alive) {
            				drawRect(
                				enemies[row][col].x,
                				enemies[row][col].y,
                				enemies[row][col].width,
                				enemies[row][col].height,
                				0x00ff00
            				);
        			}
    			}
		}

		// DELAY
                delay();
    	}
}
