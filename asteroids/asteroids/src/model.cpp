/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"


enum {
LBOUND = 50,
RBOUND = 500,
UBOUND = -25,
BBOUND = 300
};


//Initial 
const size_t MAXSize = 10;
miss_t mdata[MAXSize];
miss_t *mfreenodes;
void minitialise(void) {
    int c;
    for(c=0 ; c<(MAXSize-1) ; c++){
        mdata[c].next = &mdata[c+1];
    }
    mdata[c].next = NULL;
		mfreenodes = &mdata[0];
} 

const size_t AMAXSize = 10;
ast_t adata[AMAXSize];
ast_t *afreenodes;
void ainitialise(void) {
    int c;
    for(c=0 ; c<(AMAXSize-1) ; c++){
        adata[c].next = &adata[c+1];
    }
    adata[c].next = NULL;
		afreenodes = &adata[0];
} 



//Allocate Node
miss_t *mallocnode(void) {
	miss_t *mnode = NULL;
	if (mfreenodes) {
		mnode = mfreenodes;
		mfreenodes = mfreenodes->next;
	}
	return mnode;
}

ast_t *aallocnode(void) {
	ast_t *anode = NULL;
	if (mfreenodes) {
		anode = afreenodes;
		afreenodes = afreenodes->next;
	}
	return anode;
}

//Free Node
void mfreenode(miss_t *mn) {
	mn->next = mfreenodes;
	mfreenodes = mn;
}

void afreenode(ast_t *an) {
	an->next = afreenodes;
	afreenodes = an;
}


void mcreate(struct missile *mr) {
	if (mr->hit == 0) {
		 mr->p.x = player.p.x;
		 mr->p.y = player.p.y;

		if (player.heading == 0) { 
			mr->v.y = -1; mr->v.x = 0;
		}
		else if (player.heading == 2) { 
			mr->v.y = 0; mr->v.x = -1;
		}
		else if (player.heading == 4) { 
			mr->v.y = 1; mr->v.x = 0;
		}
		else if (player.heading == 6) { 
			mr->v.y = 0; mr->v.x = 1;
		}
		else if (player.heading == 1) { 
			mr->v.y = -1; mr->v.x = -1;
		}
		else if (player.heading == 3) { 
			mr->v.y = 1; mr->v.x = -1;
		}
		else if (player.heading == 5) { 
			mr->v.y = 1; mr->v.x = 1;
		}
		else if (player.heading == 7) { 
			mr->v.y = -1; mr->v.x = 1;
		}
			mr->ttl = 10; 
			mr->hit = 0;
	}
	else if (mr->hit == 1) {
		mr->p.x = 500;
		mr->p.y = 500;
		mr->v.y = 0;
		mr->v.x = 0;
		mr->ttl = 5;
		mr->hit = 0;
	}
}

//Creating Asteroid
void acreate(struct asteroid *ar) {
	  ar->p.x = randrange(RBOUND,LBOUND);
    ar->p.y = randrange(UBOUND,BBOUND);
			while (((ar->p.y > UBOUND) && (ar->p.y < BBOUND)) && ((ar->p.x > LBOUND) && (ar->p.x < RBOUND))) {
					  ar->p.x = randrange(UBOUND,RBOUND);
						ar->p.y = randrange(UBOUND,BBOUND);
		}
		ar->v.x = randrange(-1,2);
		ar->v.y = randrange(-1,2);
		
		ar->ttl = 20;
		ar->hit = 0;
}


const float Df = 0.01;
void mupdate(struct missile *ml)
{
		// Update each missile in the list
    for( ; ml ; ml = ml->next ) {
        ml->p.x += ml->v.x; 
        ml->p.y += ml->v.y;
        
				ml->ttl -= Df;
        // Remove the missile if its ttl is 0
        if( (ml->next->ttl<=0) || (ml->hit == 1) ) {
            struct missile *mexpired = ml->next;
            ml->next = ml->next->next;
            mfreenode(mexpired);
        }
    }
}

void aupdate(struct asteroid *al)
{
	
    for( ; al ; al = al->next ) {
        al->p.x += al->v.x; 
        al->p.y += al->v.y;
			
			
				if (al->p.x < LBOUND) {
					al->p.x = RBOUND;
				}
				if (al->p.x > RBOUND) {
					al->p.x = LBOUND;
				}
				if (al->p.y < UBOUND) {
					al->p.y = BBOUND;
				}
				if (al->p.y > BBOUND) {
					al->p.y = UBOUND;
				}
				al->ttl -= Df;
				
				
				if(   ((al->next->ttl<=0) && ((al->next->p.x > RBOUND-5) || (al->next->p.x < LBOUND+5)
					|| (al->next->p.y > BBOUND-5) || (al->next->p.y < UBOUND+5))) || (al->hit == 1)) {
					struct asteroid *aexpired = al->next;
					al->next = al->next->next;
					afreenode(aexpired);
				}
    }
	
}


struct missile *shots = NULL;
void missile_system(void) {
	struct missile *mspark = mallocnode();
        if(mspark) {
            mspark->next = shots;
            shots = mspark;
            mcreate(mspark);
        }

        mupdate(shots);
}

int ct = 500;
struct asteroid *asteroids = NULL;


void asteroid_system(void) {
	ct--;
	if (ct == 0) {
		struct asteroid *aspark = aallocnode();
					if(aspark) {
							aspark->next = asteroids;
							asteroids = aspark;
							acreate(aspark);
						ct = 500;
        }
			}
}


void reset() {
	asteroids = NULL;
	shots = NULL;
}

//Collision
void missiles_hit_rocks(struct missile *mi, struct asteroid *as) {
	
	for ( ; mi ; mi = mi->next ) {
		for ( ; as ; as = as->next ) {
			if (((mi->p.x > as->p.x-20) && (mi->p.x < as->p.x+20)) && 
				 ((mi->p.y > as->p.y-20) && (mi->p.y < as->p.y+20))) {
					 as->hit = 1;
					 mi->hit = 1;
					 acreate(as);
					 mcreate(mi);
		  }
	  }	
	}
}


bool ship_hits(struct asteroid *as) {
	for( ; as ; as = as->next ) {
		if (((player.p.x > as->p.x-20) && (player.p.x < as->p.x+20)) && 
				((player.p.y > as->p.y-20) && (player.p.y < as->p.y+20))) {
					return true;
				}
		}
	return false;
}

//Moving Ship
ship move_ship(ship player, float Dt) {
		player.p.y += player.v.y;
		player.p.x += player.v.x;
	
	if (player.p.x < 75) {
		player.p.x = 474;
	}
	if (player.p.x > 475) {
		player.p.x = 74;
	}
	if (player.p.y < 0) {
		player.p.y = 269;
	}
	if (player.p.y > 270) {
		player.p.y = 1;
	}
	
	return player;
}


//Physics
int hit = 0;
int hct = 100;
void physics(void)
{
	if (!paused) {
    player = move_ship(player, Dt);
	  asteroid_system();

		if (ship_hits(asteroids)) {
			if (hit == 0) {
				lives--;
				hit = 1;
			}
	}
		if (hit == 1) {
			player.lostlife = 1;
				hct--;
				if (hct == 0) {
					player.lostlife = 0;
					hit = 0;
					hct = 100;
			  }
		}
	  mupdate(shots);
		aupdate(asteroids);
		missiles_hit_rocks(shots, asteroids);
	}
}
