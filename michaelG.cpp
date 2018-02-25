//
#include "header.h"

void displayScore(const char* s, int xres, int i)
{

	Rect r;
	r.bot = xres -50;

switch(i){
	case 1:
		r.left =400;
		break;
	case 2:
		r.left =800;
		break;
	}

	ggprint16(&r,32, 0x00ffffff, s);
}		








