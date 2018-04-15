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

//void sleep();
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

void displayText(int x, int y, const char* s)
{
    Rect r;
    r.bot = y;
    r.left = x;
    ggprint16(&r, 16, 0x00ffffff, s);
}

void showControls(int x, int y, int i)
{
    switch (i) {
        case 1:
            displayText(x, y-100, "PLAYER 1");
            displayText(x, y-125, "-------------");
            displayText(x, y-150, "JUMP: W");
            displayText(x, y-175, "LEFT: A");
            displayText(x, y-200, "RIGHT: D");
            displayText(x, y-225, "PUNCH: R");
            displayText(x, y-250, "KICK: F");
            break;
        case 2:
            displayText(x, y-100, "PLAYER 2");
            displayText(x, y-125, "-------------");
            displayText(x, y-150, "JUMP: W");
            displayText(x, y-175, "LEFT: A");
            displayText(x, y-200, "RIGHT: D");
            displayText(x, y-225, "PUNCH: R");
            displayText(x, y-250, "KICK: F");
            break;
    }
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

void drawMenu(int x, int y)
{
    glClearColor(.1,.1,.6,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(x*0.5, y*0.5,1.0f);
    //glRectf(-125,50,125,-50);
    glBegin(GL_QUADS);
        glColor3ub(75,75,75);
        glVertex2f(-150,30);
        glColor3ub(100,100,100);
        glVertex2f(150,30);
        glColor3ub(75,75,75);
        glVertex2f(150,-30);
        glColor3ub(100,100,100);
        glVertex2f(-150,-30);
    glEnd();
    glPopMatrix();
    
    Rect r;
    r.bot = y*0.5-13;
    r.left = x*0.5;
    r.center = 1;
    ggprint16(&r, 32, 0x00ffffff, "PLAY");

    glPushMatrix();
    glTranslatef(x*.5, y*.5, 1.0f);
    glLineWidth(20);
    glBegin(GL_LINE_LOOP);
        glColor3ub(185,185,185);
        glVertex2f(-150,30);
        glColor3ub(50,50,50);
        glVertex2f(150,30);
        glColor3ub(185,185,185);
        glVertex2f(150, -30);
        glColor3ub(50,50,50);
        glVertex2f(-150,-30);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x*0.5,y*0.32,1.0f);
    glBegin(GL_QUADS);
        glColor3ub(100,100,100);
        glVertex2f(-150,30);
        glColor3ub(75,75,75);
        glVertex2f(150,30);
        glColor3ub(100,100,100);
        glVertex2f(150,-30);
        glColor3ub(75,75,75);
        glVertex2f(-150,-30);
    glEnd();
    glPopMatrix();

    //Rect r;
    r.bot = y*0.32-13;
    r.left = x*0.5;
    r.center = 1;
    ggprint16(&r, 32, 0x00ffffff, "CONTROLS");

    glPushMatrix();
    glTranslatef(x*.5, y*.32, 1.0f);
    glLineWidth(20);
    glBegin(GL_LINE_LOOP);
        glColor3ub(185,185,185);
        glVertex2f(-150,30);
        glColor3ub(50,50,50);
        glVertex2f(150,30);
        glColor3ub(185,185,185);
        glVertex2f(150, -30);
        glColor3ub(50,50,50);
        glVertex2f(-150,-30);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x*0.5,y*0.14,1.0f);
    glBegin(GL_QUADS);
        glColor3ub(100,100,100);
        glVertex2f(-150,30);
        glColor3ub(75,75,75);
        glVertex2f(150,30);
        glColor3ub(100,100,100);
        glVertex2f(150,-30);
        glColor3ub(75,75,75);
        glVertex2f(-150,-30);
    glEnd();
    glPopMatrix();
    
    r.bot = y*0.14-13;
    r.left = x*0.5;
    r.center = 1;
    ggprint16(&r, 32, 0x00ffffff, "CREDITS");

    glPushMatrix();
    glTranslatef(x*.5, y*.14, 1.0f);
    glLineWidth(20);
    glBegin(GL_LINE_LOOP);
        glColor3ub(185,185,185);
        glVertex2f(-150,30);
        glColor3ub(50,50,50);
        glVertex2f(150,30);
        glColor3ub(185,185,185);
        glVertex2f(150, -30);
        glColor3ub(50,50,50);
        glVertex2f(-150,-30);
    glEnd();
    glPopMatrix();
}


