/*
 * CMPS 3350 Semester Project
 * Purpose: Write some functions for the main program called Street Fighter Z.
 * 
 *
 * @author Esteban Rodriguez
 *
 */

#include "header.h"

// Display name of characters
void displayName (const char* s, int xres, int yres, int i)
{
    Rect r;
    r.bot = yres - 50;

	//Switch case for name position depending on which player to display
    switch (i) {
	case 1:
	    r.left = (xres/2) - 200;
	    break;
	case 2:
	    r.left = (xres/2) + 200;
	    break;
    }
    ggprint16(&r, 32, 0x00ffffff, s);
}

void controls (int offset, int yres, const char *s)
{
    Rect r;
    r.left = 100;
    r.bot = yres - offset;
    ggprint16(&r, 32, 0x00ffffff, s);
}

void displayAuthors(bool displayNames)
{
	Rect r;
	r.left = 100;
	r.bot = 100;
	const char* ADAM = "adamA";
	const char* EDUARDO = "eduardoB";
	const char* MATTHEW = "matthewG";
	const char* MICHAEL = "michaelG";
	const char* ESTEBAN = "estebanR";
	ggprint16(&r, 32, 0x00ffffff, ADAM);
	ggprint16(&r, 32, 0x00ffffff, EDUARDO);
	ggprint16(&r, 32, 0x00ffffff, MATTHEW);
	ggprint16(&r, 32, 0x00ffffff, MICHAEL);
	ggprint16(&r, 32, 0x00ffffff, ESTEBAN);
}
