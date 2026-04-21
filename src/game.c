#include "graphics.h"
#include "kernel_main.h"
#include <stdint.h>
#include "input.h"
#include "game.h"

// simple delay to slow down the game
void delay() {
	for (volatile int i = 0; i < 300000; i++); // changed from 1000000 to 300000
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

	// Add bullets array
	Bullet bullets[MAX_BULLETS];

	// Initializing bullets
	for (int i = 0; i < MAX_BULLETS; i++) {
    		bullets[i].active = 0;
	}

	int shootCoolDown = 0;

    	while (1) {

        	// INPUT (movement)
        	if (key_state[KEY_LEFT]) {
            		player.x -= 30; // changed this from 5 to 30 for speed
        	}

        	if (key_state[KEY_RIGHT]) {
            		player.x += 30; // changed this from 5 to 30 for speed
        	}

        	// BOUNDARY CHECK
        	if (player.x < 0) {
            		player.x = 0;
        	}

        	if (player.x + player.width > getFramebufferWidth()) {
            		player.x = getFramebufferWidth() - player.width;
        	}

		// once the letter q, is pressed, the game would end. (clear screen)
		if (key_state[KEY_Q]) {
			clearScreen(0x000000);
			while(1) {
				asm volatile("hlt");
			}
		}

		// Bullet Firing logic
                if (key_state[KEY_SPACE] && shootCoolDown == 0) {

                        for (int i = 0; i < MAX_BULLETS; i++) {

                                if (!bullets[i].active) {

                                        bullets[i].active = 1;

                                        bullets[i].x = player.x + player.width / 2 - BULLET_WIDTH / 2;
                                        bullets[i].y = player.y - BULLET_HEIGHT;

                                        bullets[i].width = BULLET_WIDTH;
                                        bullets[i].height = BULLET_HEIGHT;

                                        bullets[i].dy = -BULLET_SPEED; // upward

                                        shootCoolDown = 10; // delay between shoots
                                        break; // only fire one bullet
				}
			}
		}


		 // MOVE BULLETS
                for (int i = 0; i < MAX_BULLETS; i++) {

                        if (bullets[i].active) {
                                bullets[i].y += bullets[i].dy;

                                // deactivate if off screen
                                if (bullets[i].y < 0) {
                                        bullets[i].active = 0;
                                }
                        }
                }


		// BULLET - ENEMY COLLISION DETECTION
		for (int i = 0; i < MAX_BULLETS; i++) {

			if (!bullets[i].active) continue;

			for (int row = 0; row < ENEMY_ROWS; row++) {
				for (int col = 0; col < ENEMY_COLS; col++) {

					if (!enemies[row][col].alive) continue;

					// AABB overlap check
					if (bullets[i].x < enemies[row][col].x + enemies[row][col].width &&
					    bullets[i].x + bullets[i].width > enemies[row][col].x &&
					    bullets[i].y < enemies[row][col].y + enemies[row][col].height &&
					    bullets[i].y + bullets[i].height > enemies[row][col].y) {

						enemies[row][col].alive = 0;
						bullets[i].active = 0;
						break; // bullet is gone, stop checking enemies
					}
				}
			}
		}

		// Add edge detection
		// Only flag moveDown if an alive enemy is being pushed INTO the edge.
		// This prevents the enemies from marching straight down after reversing.
		int moveDown = 0;

		for (int row = 0; row < ENEMY_ROWS; row++) {
    			for (int col = 0; col < ENEMY_COLS; col++) {

        			if (!enemies[row][col].alive) continue;

        			if (enemyDirection > 0 &&
				    enemies[row][col].x + enemies[row][col].width >= SCREEN_WIDTH) {
            				moveDown = 1;
        			}

        			if (enemyDirection < 0 &&
				    enemies[row][col].x <= 0) {
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
            				enemies[row][col].x += enemyDirection * 10; // smoother. changed this from 1 to 10 to see if it's faster
        			}
    			}
		}

		if (moveDown) {
    			enemyDirection *= -1;
		}

	        // RENDER
        	clearScreen(0x000000); // I want to see with black instead of red
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

		// DRAW BULLETS
		for (int i = 0; i < MAX_BULLETS; i++) {

    			if (bullets[i].active) {
        			drawRect(
            				bullets[i].x,
            				bullets[i].y,
            				bullets[i].width,
            				bullets[i].height,
            				0xffffff // white bullet
        			);
    			}
		}

		if (shootCoolDown > 0) {
                	shootCoolDown--;
        	}


		// DELAY
                delay();
    	}

}
