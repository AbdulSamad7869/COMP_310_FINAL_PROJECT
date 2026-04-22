#include "graphics.h"
#include "kernel_main.h"
#include <stdint.h>
#include "input.h"
#include "game.h"

// simple delay to slow down the game
// lower number makes the game faster and vice versa 
void delay() {
	for (volatile int i = 0; i < 300000; i++); // changed from 1000000 to 300000
}

void game_loop() {

	// initialize player (matches your rectangle)
    	Player player;
    	player.x = 462; // horizontal starting position 
    	player.y = 680; // veritcak starting position
    	player.width = 100; // width of the player rectangle
    	player.height = 80; // height of the player rectangle

	// enemy array
	// 2D array of structs
	Enemy enemies[ENEMY_ROWS][ENEMY_COLS];

	// initializing enemies
	// loop through each row and column of the enemy grid
	for (int row = 0; row < ENEMY_ROWS; row++) {
    		for (int col = 0; col < ENEMY_COLS; col++) {
			// calculate the x position of this enemy based on its column
			// each enemy is offset by its width plus padding from the previous one
        		enemies[row][col].x = ENEMY_START_X + col * (ENEMY_WIDTH + ENEMY_PADDING_X);
        		// calculate the y position of the enemy based on its row
			// each row is offset by its height plus padding from the row above
			enemies[row][col].y = ENEMY_START_Y + row * (ENEMY_HEIGHT + ENEMY_PADDING_Y);
			// set the enemy dimensions from the constants in game.h
        		enemies[row][col].width = ENEMY_WIDTH;
        		enemies[row][col].height = ENEMY_HEIGHT;
			// mark this enemy as alive at the start of the game
        		enemies[row][col].alive = 1;
    		}
	}
	// tracks which direction the enemy block is currently moving
	// when it hits the right edge, then it goes to -1 to make it move left
	// when it hits the left edge, then it goes to +1 to make it move right
	int enemyDirection = 1; // 1 = right, -1 = left

	// Add bullets array. limits how many bullets can be "in flight" at the same time
	Bullet bullets[MAX_BULLETS];

	// Initializing bullets
	// loop through every slot in the bullets array and set active to 0
	// 0 means bullet is not in flight
	// 1 means bullet is in flight
	for (int i = 0; i < MAX_BULLETS; i++) {
    		bullets[i].active = 0;
	}

	// stops the player from shooting too many bullets too quickly
	// starts at 0 so the player can fire a bullet immediately at the start of the game
	int shootCoolDown = 0;

    	while (1) {

        	// INPUT (movement)
		// check if the left arrow key is being pressed
		// if it is move the player 30 pixels to the left
		// key_state[KEY_LEFT] is 1 if the key is held, 0 if not
        	if (key_state[KEY_LEFT]) {
            		player.x -= 30; // changed this from 5 to 30 for speed
        	}

		// same logis as above
        	if (key_state[KEY_RIGHT]) {
            		player.x += 30; // changed this from 5 to 30 for speed
        	}

        	// BOUNDARY CHECK
		// if the player's x position gets lower than 0, set it to 0
		// this makes sure the player doesn't go off the screen on the left
        	if (player.x < 0) {
            		player.x = 0;
        	}

		// if the right side of the player goes past the screen width,
		// pull it back so the player's right edge lines up with the screen edge
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
		// only fire a bullet if two conditions are met:
		// 1. the spacebar is being held down
		// 2. the shoot cool down has reached 0
                if (key_state[KEY_SPACE] && shootCoolDown == 0) {
			// go through the bullet array and find the first inactive slot
                        for (int i = 0; i < MAX_BULLETS; i++) {
				// if the bullet at the spot is not active ( meaning 0)
                                if (!bullets[i].active) {
					// make it active (meaning 1)
                                        bullets[i].active = 1;

					// position the bullet at the center top of the player rectangle
					bullets[i].x = player.x + player.width / 2 - BULLET_WIDTH / 2;
					// have the bullet leave from just above the rectangle
                                        bullets[i].y = player.y - BULLET_HEIGHT;

					// set the bullet dimensions from the constants in game.h
                                        bullets[i].width = BULLET_WIDTH;
                                        bullets[i].height = BULLET_HEIGHT;

					// set the bullet's vertical speed and direction
					// negative means upward movement
					// dy means change in y position per frame
                                        bullets[i].dy = -BULLET_SPEED; // upward

                                        shootCoolDown = 10; // delay between shoots
                                        break; // only fire one bullet
				}
			}
		}


		 // MOVE BULLETS
		// go through every bullet slot in the array
                for (int i = 0; i < MAX_BULLETS; i++) {
			// if the bullet is currently fired (aka in motion), 
			// move the bullet by adding dy to its y position
			// if bullet.y is 670, after this line it becomes 662
			// this is because dy is -8 meaning it moves 8 pixes upwards each frame
                        if (bullets[i].active) {
                                bullets[i].y += bullets[i].dy;

                                // deactivate if off screen
				// if the bullet goes off the top of the screen, deactivate it
				// if y is less than 0, that means it's off the screen
                                if (bullets[i].y < 0) {
                                        bullets[i].active = 0;
                                }
                        }
                }


		// BULLET - ENEMY COLLISION DETECTION
		// check every bullet agaisnt every enemy to see if anything was hit
		for (int i = 0; i < MAX_BULLETS; i++) {
			// loop through all the bullet slots
			// if a bullet isn't in flight just skip it
			if (!bullets[i].active) continue;
			// loop through every row and column of the enemy grid
			for (int row = 0; row < ENEMY_ROWS; row++) {
				for (int col = 0; col < ENEMY_COLS; col++) {
					// if this enemy is dead, just skip it
					if (!enemies[row][col].alive) continue;
					// Axis-Aligned Bounding Box (AABB) overlap check
					// 4 conditions need to be met for an enemy to be hit
					// 1. the bullet's left edge is the to left of the enemy's right edge
					// 2. bullet's right edge is to the right of the enemy's left edge
					// 3. bullet's top edge is above the enemy's bottom edge
					// 4. bullet's bottom edge is below the enemy's top edge
					if (bullets[i].x < enemies[row][col].x + enemies[row][col].width &&
					    bullets[i].x + bullets[i].width > enemies[row][col].x &&
					    bullets[i].y < enemies[row][col].y + enemies[row][col].height &&
					    bullets[i].y + bullets[i].height > enemies[row][col].y) {
						// enemy has been hit, make it dead (set to 0)
						enemies[row][col].alive = 0;
						// deactivate bullet (set to 0)
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
		// loop through every enemy grid 
		for (int row = 0; row < ENEMY_ROWS; row++) {
    			for (int col = 0; col < ENEMY_COLS; col++) {
				// if the  enemy is dead, just skip it
        			if (!enemies[row][col].alive) continue;
				// if the enemy's right edge has reached the screen's right edge, move down
        			if (enemyDirection > 0 &&
				    enemies[row][col].x + enemies[row][col].width >= SCREEN_WIDTH) {
            				moveDown = 1;
        			}
				// same concept as above but for left edge
        			if (enemyDirection < 0 &&
				    enemies[row][col].x <= 0) {
            				moveDown = 1;
        			}
    			}
		}

		// Move enemies
		// loop through every enemy in the grid and move them
		for (int row = 0; row < ENEMY_ROWS; row++) {
    			for (int col = 0; col < ENEMY_COLS; col++) {
				// if the enemyis dead, skip it
        			if (!enemies[row][col].alive) continue;
				// if an enemy has reached the edge of the screen, move it down 20 pixels
        			if (moveDown) {
            				enemies[row][col].y += 20;
				// did not hit the edge so move it in the current direction
				// multiplication is for speed
        			} else {
            				enemies[row][col].x += enemyDirection * 10; // smoother. changed this from 1 to 10 to see if it's faster
        			}
    			}
		}
		// if an enemy has hit the edge, go the other direction (just mulitoply by -1)
		if (moveDown) {
    			enemyDirection *= -1;
		}

	        // RENDER
		// this is done to erase all the previous positions and not leave a trail
        	clearScreen(0x000000); // I want to see with black instead of red
		// draw the player at it's current position
        	drawRect(player.x, player.y, player.width, player.height, 0xffffff);

		// Draw enemies
		// loop through all the enenmies in the grid and draw the ones that are alive
		for (int row = 0; row < ENEMY_ROWS; row++) {
    			for (int col = 0; col < ENEMY_COLS; col++) {
        			if (enemies[row][col].alive) {
            				drawRect(
                				enemies[row][col].x, // horizontal position
                				enemies[row][col].y, // veritcal position
                				enemies[row][col].width, // how wide the enemy is
                				enemies[row][col].height, // how tall the enemy is
                				0x00ff00 // green color
            				);
        			}
    			}
		}

		// DRAW BULLETS
		// loop through all the bullet slots and draw the ones that are in flight
		for (int i = 0; i < MAX_BULLETS; i++) {
    			if (bullets[i].active) {
        			drawRect(
            				bullets[i].x, // horizontal position
            				bullets[i].y, // vertical position
            				bullets[i].width, // how wide the bullet is
            				bullets[i].height, // how tall the bullet is 
            				0xffffff // white bullet
        			);
    			}
		}
		// decrease the shoot cooldown by 1 each frame 
		// once it reaches 0, then the player can fire again
		if (shootCoolDown > 0) {
                	shootCoolDown--;
        	}
		// DELAY
		// wait before starting the next frame
		// controls the game speed
                delay();
    	}
}
