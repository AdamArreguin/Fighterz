/*
 * CMPS 3350 Semester Project
 * Purpose: Write some functions for the main program called Street Fighter Z.
 * 
 *
 * @author Esteban Rodriguez
 *
 */

#include "header.h"
//#include <ctime>
//#include <time.h>

void sleep();
// Display name of characters
void displayName (const char* s, int yres, int i)
{
    Rect r;
    r.bot = yres - 50;

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

void healthBarOverlay(int x, int y) 
{
    glPushMatrix();
    glTranslatef(x*.5 - 500, y-40, 1.0f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-50.0f, -50.0f, 0.0f);
    glBegin(GL_QUADS);
        glColor3ub(195, 195, 0);
        //Increment x when decreasing health
        glVertex2f(0,0);
        glColor3ub(255, 255, 0);
        //Increment x when decreasing health
        glVertex2f(0,40);
        glColor3ub(255, 255, 0);
        glVertex2f(500,40);
        glColor3ub(195, 195, 0);
        glVertex2f(450,0); 
    glEnd();
    glPopMatrix();

    //HealthBar border
    glPushMatrix();
    glTranslatef(x*.5 - 500, y-36, 1.0f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-50.0f, -50.0f, 0.0f);
    glLineWidth(30);
    glBegin(GL_LINE_LOOP);
        glColor3ub(50,50,50);
        glVertex2f(0,0);
        glColor3ub(185,185,185);
        glVertex2f(0,32);
        glColor3ub(50,50,50);
        glVertex2f(500,32);
        glColor3ub(102,102,102);
        glVertex2f(458,0);
    glEnd();
    glPopMatrix();
}

void healthBarOverlay2(int x, int y)
{   
    glPushMatrix();
    glTranslatef(x*.5+140, y-40, 1.0f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-50.0f, -50.0f, 0.0f);
    glBegin(GL_QUADS);
        glColor3ub(195, 195, 0);
        glVertex2f(  0,  0);
        glColor3ub(255, 255, 0);
	    glVertex2f(-50, 40);
        glColor3ub(255, 255, 0);
	    glVertex2f(450, 40);
        glColor3ub(195, 195, 0);
	    glVertex2f(450,  0);
    glEnd();
    glPopMatrix();

    //HealthBar2 border
    glPushMatrix();
    glTranslatef(x*.5+130, y-36, 1.0f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-50.0f, -50.0f, 0.0f);
    glLineWidth(30);
    glBegin(GL_LINE_LOOP);
        glColor3ub(102,102,102);
        glVertex2f(0,0);
        glColor3ub(50,50,50);
        glVertex2f(-42,32);
        glColor3ub(185,185,185);
        //Decrement x when decreasing health
        glVertex2f(460, 32);
        glColor3ub(50,50,50);
        //Decrement x when decreasing health
        glVertex2f(460,0);
    glEnd();
    glPopMatrix();
}


void countdown(int x, int y) 
{
    glPushMatrix();
    glTranslatef(x*.5-100, y-90, 1.0f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glColor3ub(185, 185, 185);
        glVertex2f(0,0);
        glColor3ub(50, 50, 50);
        glVertex2f(50,40);
        glColor3ub(185, 185, 185);
        glVertex2f(139,40);
        glColor3ub(50, 50, 50);
        glVertex2f(189,0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x*.5-72, y-82, 1.0f);
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(33,24);
        glVertex2f(104,24);
        glVertex2f(134,0);
    glEnd();
    glPopMatrix();
    static Rect r;
    r.bot = y - 82;
    r.left = x*.5 - 20;
    
    static int min = 5;
    static int sec = 0;
    char c[4];
    for (int i=5; i>=0 ; i--) {
        if (min == 0 && sec == 0) {
            sprintf(c, "%d:0%d", min, sec);
            ggprint16(&r, 32, 0x00ff0000, c);
            break;
        }
        for (int j=59; j>=0; j--) {
            if (sec == 0) {
                sec = 59;
            } else if (sec < 10) {
                sprintf(c, "%d:0%d", min, sec);
                ggprint16(&r, 32, 0x00ff0000, c);
                sec--;
            } else {
                sprintf(c, "%d:%d", min, sec);
                ggprint16(&r, 32, 0x00ff0000, c);
                sec--;
            }
        }
        min--; 
    }  
}

//============GORDON'S EXAMPLE=================
//
//double testFunc2(){
//static double t = 0.0;
//struct timespec ftimeStart, ftimeEnd;
//clock_gettime(CLOCK_REALLTIME, &ftimeStart);
//int a = 200, b = 300, c = 400;
//for (int i=0; i<10; i++) {
//	b = (a&15) - (b&7) - (C&3);
//	c = (b&31) - (a&7) - (c&3);
//}
//clock_gettime(CLOCK_REALTIME, &ftimeEnd);
//t += timeDiff(&ftimeStart, &ftimeEnd);
//return t;
//}
