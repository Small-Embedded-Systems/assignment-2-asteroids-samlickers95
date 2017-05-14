/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

// Joystick 5-way switch
typedef enum {left, down, right, up, centre} btnId_t;
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};
bool jsPrsdAndHld(btnId_t b);


void fire() {
	missile_system();
}

void controls(void)
{
	if ((jsPrsdAndHld(up) && jsPrsdAndHld(right))) {
		player.v.x = -1; player.v.y = -1;
		player.heading = 1;
	}
	else if ((jsPrsdAndHld(right) && jsPrsdAndHld(down))) {
		player.v.x = -1; player.v.y = 1;
		player.heading = 3;
	}
	else if ((jsPrsdAndHld(down) && jsPrsdAndHld(left))) {
		player.v.x = 1; player.v.y = 1;
		player.heading = 5;
	}
	else if ((jsPrsdAndHld(left) && jsPrsdAndHld(up))) {
		player.v.x = 1; player.v.y = -1;
		player.heading = 7;
	}
	else if (jsPrsdAndHld(up)) {
		player.v.y = -1;
		player.heading = 0;
	}
	else if (jsPrsdAndHld(right)) {
		player.v.x = -1;
		player.heading = 2;
	}
	else if (jsPrsdAndHld(down)) {
		player.v.y = 1;
		player.heading = 4;
	}
	else if (jsPrsdAndHld(left)) {
		player.v.x = 1;
		player.heading = 6;
	}
	

	else if (jsPrsdAndHld(centre)) {
		if (paused) {
			paused = false;
		}
		else if (!paused) {
			fire();
		}
	}
	

	else {	
		player.v.x = 0; player.v.y = 0;
	}
}





/* Definition of Joystick press capture function
 * b is one of JLEFT, JRIGHT, JUP, JDOWN - from enum, 0, 1, 2, 3
 * Returns true if this Joystick pressed, false otherwise.
 *
 * If the value of the button's pin is 0 then the button is being pressed,
 * just remember this in savedState.
 * If the value of the button's pin is 1 then the button is released, so
 * if the savedState of the button is 0, then the result is true, otherwise
 * the result is false. */
bool jsPrsdAndHld(btnId_t b) { // changed from pressed and released to held
	bool result = false;
	uint32_t state;
	static uint32_t savedState[5] = {1,1,1,1,1};
        //initially all 1s: nothing pressed
	state = joystick[b].read();
  if ((savedState[b] == 0) && (state == 0)) {
		result = true;
	}
	savedState[b] = state;
	return result;
}

