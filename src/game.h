#ifndef GAME_H
#define GAME_H
#include <stdint.h>

// screen dimensions (must match the framebuffer requested in kernel_main.c)
#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

// --- Enemy grid constants ---
#define ENEMY_ROWS       3
#define ENEMY_COLS       8
#define ENEMY_WIDTH      40
#define ENEMY_HEIGHT     20
#define ENEMY_PADDING_X  20
#define ENEMY_PADDING_Y  20
#define ENEMY_START_X    100
#define ENEMY_START_Y    80

// --- Bullet constants ---
#define MAX_BULLETS      8
#define BULLET_WIDTH     4
#define BULLET_HEIGHT    10
#define BULLET_SPEED     8

/*
 * Enemy structure:
 * Stores position, size, and alive state of each enemy
 */
typedef struct {
    int x;
    int y;
    int width;
    int height;
    int alive;   // 1 = alive, 0 = dead (no malloc/free, just flag)
} Enemy;

/*
 * Bullet structure:
 * Stores position, size, direction, and active state
 */
typedef struct {
    int x;
    int y;
    int width;
    int height;
    int active;  // 1 = in flight, 0 = inactive
    int dy;      // direction: negative = moving up (player bullet)
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
