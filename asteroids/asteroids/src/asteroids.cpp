/* Asteroids
    Sample solution for assignment
    Semester 2 -- Small Embedded Systems
    Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int 	prevscore;
int   lives;
int 	tickCt;
struct ship player;

float Dt = 0.01f;

Ticker model, view, controller, timer;

bool paused = true;


void timerHandler() {
	if (!paused) {
		tickCt++;
		elapsed_time = tickCt;
		score = tickCt;
	}
}

void initialise_game() {
		// Initialising the game
	  minitialise();
	  ainitialise();
    init_DBuffer();
    
	  // Tickers
    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);
	  timer.attach(&timerHandler, 1);
    
	  
    lives = 5;
	  tickCt = 0;
	  score = 0;
	  elapsed_time = 0;
	
	 
		player.p.x = 260; player.p.y = 160;
		player.v.x = 0;	player.v.y = 0;
		player.lostlife = 0;
    
    paused = true;
}

int main()
{
		initialise_game();
    
    while(true) {
        while( !paused ){
					while(lives == 0) {
						prevscore = score;
						reset();
						initialise_game();
					}
        }
        
    }
}