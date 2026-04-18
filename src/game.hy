#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "game.h" 
/*
 * Player structure:
 * Stores position and size of the player rectangle
 */

/*
 * Starts the player with starting values
 */
void initPlayer(struct player *p, int x, int y, int width, int height);

/*
 * Updates the  player position based on keyboard input
 */
void updatePlayer(struct player *p);

/*
 * Draws the player on screen
 */
void drawPlayer(struct player *p);

/*
 * Delay to slow down the game loop
 */
void gameDelay(void);

void game_loop();



#endif

