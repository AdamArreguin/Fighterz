//Author:Michael Guzman
//Modified on : 04/20/2018
#include "header.h"
#include "time.h"
#include <stdio.h>
void displayScore(const char* s, int xres, int i)
{
    clock_t start = clock();
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

    static float end = 0;
    end += clock() - start;
    r.bot = 400;
    r.left = xres/2;
    r.center = 1;
    char c[50];
    sprintf(c, "Timer:  %f", end/CLOCKS_PER_SEC);
    ggprint16(&r, 16, 0x00ffffff, c);
    glPopMatrix();

    r.bot = 450;
    r.left = xres/2;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffffff, "Michael Guzman");
    glPopMatrix();


}

void displayScoreOpt(const char* s, int xres, int i)
{
    clock_t start = clock();
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


    //ggprint16(&r,32, 0x00ffffff, s);

    static float end = 0;
    end += clock() - start;
    r.bot = 350;
    r.left = xres*0.5;
    r.center = 1;
    char c[50];
    sprintf(c, "TimerOpt:  %f", end/CLOCKS_PER_SEC);
    ggprint16(&r, 16, 0x00ffffff, c);
    glPopMatrix();


}
