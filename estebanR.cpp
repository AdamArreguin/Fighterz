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
void displayName (const char* s, int xres, int i)
{
    Rect r;
    r.bot = xres - 50;

    switch (i) {
	case 1:
	    r.left = 500;
	    break;
	case 2:
	    r.left = 750;
	    break;
    }
    ggprint16(&r, 32, 0x00ffffff, s);
}

void controls (int x, int y, const char *s)
{
    Rect r;
    r.left = x;
    r.bot = y;
    ggprint16(&r, 32, 0x00ffffff, s);
}
