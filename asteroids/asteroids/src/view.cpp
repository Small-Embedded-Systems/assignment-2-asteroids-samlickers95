/* Asteroids view
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

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

const colour_t background = LIGHTBLUE;
const colour_t sidebar = BLACK;


const coordinate_t shape[] = {
    {10,0}, {-5,5}, {-5,-5}
};


void init_DBuffer(void)
{   
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{  
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

//Sidebar
void draw_sidebar(int e_t, int sc, int li) {
	graphics->fillRect(0, 0, 80, 275, sidebar);
	  graphics->setTextColor(WHITE, sidebar);
		graphics->setCursor(5, 10);
			graphics->printf("Assignment:");
		graphics->setCursor(5, 30);
			graphics->printf("Asteroids");
		graphics->setCursor(27, 80);
			graphics->printf("Time");
		graphics->setCursor(35, 90);
	    graphics->printf("%d", e_t);
		graphics->setCursor(25, 120);
	    graphics->printf("Score:");
		graphics->setCursor(35, 130);
	    graphics->printf("%d", sc);
		graphics->setCursor(25, 160);
		  graphics->printf("Lives:");
		graphics->setCursor(35, 170);
	    graphics->printf("%d", li);
		graphics->setCursor(17, 200);
		  graphics->printf("Previous");
		graphics->setCursor(25, 210);
			graphics->printf("Score:");
		graphics->setCursor(35, 220);
	    graphics->printf("%d", prevscore);
			
			if (paused) {
				graphics->setCursor(180, 90);
					graphics->setTextColor(WHITE, background);
					graphics->printf("Press joystick IN to begin");
			}
}

//Draw Ship
void draw_ship(ship player) {
	if (player.heading == 0) {
		graphics->fillTriangle(player.p.x, player.p.y-4,
		player.p.x-2, player.p.y+2,
		player.p.x+2, player.p.y+2,
		GREEN);
				if (player.lostlife == 1) {
			graphics->fillTriangle(player.p.x, player.p.y-4,
			player.p.x-2, player.p.y+2,
			player.p.x+2, player.p.y+2,
			RED);
			}
	}
	
	if (player.heading == 1) { 
		graphics->fillTriangle(player.p.x-3, player.p.y-3,
		player.p.x, player.p.y+3,
		player.p.x+3, player.p.y,
		GREEN);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x-3, player.p.y-3,
				player.p.x, player.p.y+3,
				player.p.x+3, player.p.y,
				RED);
			}
	}

	if (player.heading == 2) { 
		graphics->fillTriangle(player.p.x-4, player.p.y,
		player.p.x+2, player.p.y+2,
		player.p.x+2, player.p.y-2,
		GREEN);			  
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x-4, player.p.y,
				player.p.x+2, player.p.y+2,
				player.p.x+2, player.p.y-2,
				RED);
			}
	}

	if (player.heading == 3) { 
		graphics->fillTriangle(player.p.x-3, player.p.y+3,
		player.p.x+3, player.p.y,
		player.p.x, player.p.y-3,
		GREEN);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x-3, player.p.y+3,
				player.p.x+3, player.p.y,
				player.p.x, player.p.y-3,
				RED);
			}
	}

	if (player.heading == 4) { 
		graphics->fillTriangle(player.p.x, player.p.y+4,
		player.p.x+2, player.p.y-2,
		player.p.x-2, player.p.y-2,
		GREEN);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x, player.p.y+4,
				player.p.x+2, player.p.y-2,
				player.p.x-2, player.p.y-2,
				RED);
			}
	}

	if (player.heading == 5) {
		graphics->fillTriangle(player.p.x+3, player.p.y+3,
		player.p.x, player.p.y-3,
		player.p.x-3, player.p.y,
		GREEN);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x+3, player.p.y+3,
				player.p.x, player.p.y-3,
				player.p.x-3, player.p.y,
				RED);
			}
	}

	if (player.heading == 6) { 
		graphics->fillTriangle(player.p.x+4, player.p.y,
		player.p.x-2, player.p.y-2,
		player.p.x-2, player.p.y+2,
		GREEN);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x+4, player.p.y,
				player.p.x-2, player.p.y-2,
				player.p.x-2, player.p.y+2,
				RED);
			}
	}

	if (player.heading == 7) { 
		graphics->fillTriangle(player.p.x+3, player.p.y-3,
		player.p.x-3, player.p.y,
		player.p.x, player.p.y+3,
		GREEN);
			if (player.lostlife == 1) {
				graphics->fillTriangle(player.p.x+3, player.p.y-3,
				player.p.x-3, player.p.y,
				player.p.x, player.p.y+3,
				RED);
			}
	}
}

void drawmissiles(struct missile *lst)
{
   
    while(lst) {
        graphics->fillCircle(lst->p.x,lst->p.y, 1, RED);
        lst = lst->next; 
    }
}

void drawasteroids(struct asteroid *lst)
{
    
    while(lst) {
      graphics->fillCircle(lst->p.x,lst->p.y, 20, WHITE);
			lst = lst->next; 
    }
}

void draw(void)
{
    graphics->fillScreen(background);
	
		draw_ship(player);
		drawmissiles(shots);
		drawasteroids(asteroids);
		draw_sidebar(elapsed_time, score, lives);
	
    swap_DBuffer();
}


