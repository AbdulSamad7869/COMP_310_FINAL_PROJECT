#ifndef INPUT_H
#define INPUT_H
#include <stdint.h>
/*
* key_state array:
* Each index represents a key (scancode)
* Value = 1 → key is pressed
* Value = 0 → key is not pressed
* This is updated inside interrupt.c
*/
extern uint8_t key_state[256];

/*
* Keyboard scancodes (from the interrupt homework)
* These represent left and right arrow keys
*/
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D
#define KEY_SPACE 0x39

#endif
