#ifndef GAME_H
#define GAME_H
#include <stdint.h>

// screen dimensions (must match the framebuffer requested in kernel_main.c)
#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

// --- Enemy grid constants ---
#define ENEMY_ROWS       3 // how many rows of enemies there are
#define ENEMY_COLS       8 // how many columns of enemies 
#define ENEMY_WIDTH      40 // how wide each enemy rectangle is
#define ENEMY_HEIGHT     20 // how tall each enemy rectangle is
#define ENEMY_PADDING_X  20 // the horizontal space between each enemy
#define ENEMY_PADDING_Y  20 // the vertical space between each enemy
#define ENEMY_START_X    100 // the x postion of where the enemy starts 
#define ENEMY_START_Y    80 // the y position of where the enemy starts

// --- Bullet constants ---
#define MAX_BULLETS      8 // max number of bullets that are active at once
#define BULLET_WIDTH     4 // how wide the bullet rectangle is 
#define BULLET_HEIGHT    10 // how tall the bullet rectangle is
#define BULLET_SPEED     8 // how many pixels the bullet moves upward

/*
 * Enemy structure:
 * Stores position, size, and alive state of each enemy
 */
typedef struct {
    int x; // the x position of a specific enemy
    int y; // y posotion of a specific enemy
    int width; // how wide an enemy is
    int height; // how tall an enemy is
    int alive;   // 1 = alive, 0 = dead
} Enemy;

/*
 * Bullet structure:
 * Stores position, size, direction, and active state
 */
typedef struct {
    int x; // x position of the bullet
    int y; // y position of the bullet
    int width; // width of a bullet
    int height; // height of the bullet
    int active;  // 1 = in flight, 0 = inactive
    int dy;      // direction and speed of movement.Negative means moving upward, represents the direction the player bullet is supposed to go.
} Bullet;

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
