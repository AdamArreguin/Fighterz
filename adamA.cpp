#include "header.h"
#include <stdio.h>


void drawHealthBar1(int x, int y)
{  
    clock_t start = clock();
    static float angle = 0.0;
    glColor3ub(250,0,0);
    glPushMatrix();
    glTranslatef(x-1200, y-40, 0.0);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-50.0,-50.,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(0, 50);
    glVertex2f(500, 50);
    glVertex2f(450, 0);
    glEnd();
    static Rect r;
    r.bot = 50;
    r.left = x-1065;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffffff, "Fighter 1");
    glPopMatrix();

    static float end = 0;
    end += clock() - start;

    r.bot = 100;
    r.left = 100;
    r.center = 1;
    char c[50];
    sprintf(c, "Timer: %f", end/CLOCKS_PER_SEC);
    ggprint16(&r, 16, 0x00ffffff, c);
    glPopMatrix();

    r.bot = 120;
    r.left = 100;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffffff, "Adam Arreguin");
    glPopMatrix();

}

void drawHealthBar2(int x, int y)
{
    static float angle = 0.0;
    glColor3ub(250,0,0);
    glPushMatrix();
    glTranslatef(x-435, y-40, 0.0);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-50.0,-50.,0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(-50, 50);
    glVertex2f(450, 50);
    glVertex2f(450, 0);
    glEnd();
    static Rect r;
    r.bot = 50;
    r.left = x-1065;
    r.center = 1;
    ggprint16(&r, 16, 0x00ffffff, "Fighter 2");
    glPopMatrix();
}

