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

	if(PROFILING_ON != 0)
	{
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

int Punch1(double xPos, double yPos, double xPos2, double yPos2, sprite &pSprite1, sprite &pSprite2,
		double extx)
{
	float cx = pSprite1.xres/8.0;
//	float cy = pSprite1.yres/8.0;

	double pTop = yPos + 100;
	double pBot2 = yPos2;
	double pRight = xPos + cx/3.5;
	double pLeft = xPos - cx/3.5;

	double extxPos = 0;
	if(extx > 0)
	{
		extxPos = xPos + extx;
		pRight = extxPos + cx/3.5;
	}
	else if(extx < 0)
	{
		extxPos = xPos + extx;
		pLeft = extxPos - cx/3.5;
	}

	double pRight2 = xPos2 + cx/3.5;
	double pLeft2 = xPos2 - cx/3.5;

	if((pRight > pLeft2 && pLeft < pLeft2 && pTop > pBot2) || (pLeft < pRight2 && pRight > pRight2 && pTop > pBot2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Punch2(double xPos, double yPos, double xPos2, double yPos2, sprite &pSprite1, sprite &pSprite2,
		double extx)
{
	float cx = pSprite1.xres/8.0;
	//float cy = pSprite1.yres;

	double pTop2 = yPos2 +100;
	double pBot = yPos;
	double pRight = xPos + cx/3.5;
	double pLeft = xPos - cx/3.5;
	double pRight2 = xPos2 + cx/3.5;
	double pLeft2 = xPos2 - cx/3.5;


	double extxPos = 0;
	if(extx > 0)
	{
		extxPos = xPos2 + extx;
		pRight2 = extxPos + cx/3.5;
	}
	else if(extx < 0)
	{
		extxPos = xPos2 + extx;
		pLeft2 = extxPos - cx/3.5;
	}

	if((pRight2 > pLeft && pLeft2 < pLeft && pTop2 > pBot) || (pLeft2 < pRight && pRight2 > pRight && pTop2 > pBot))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int Kick1(double xPos, double yPos, double xPos2, double yPos2, sprite pSprite1, sprite pSprite2,
		double extx)
{
	float cx = pSprite1.xres/8.0;
	//float cy = pSprite1.yres;
	double pTop = yPos + 100;
	double pBot2 = yPos2;

	double pRight = xPos + cx/3.5;
	double pLeft = xPos - cx/3.5;

	double extxPos = 0;
	if(extx > 0)
	{
		extxPos = xPos + extx;
		pRight = extxPos + cx/3.5;
	}
	else if(extx < 0)
	{
		extxPos = xPos + extx;
		pLeft = extxPos - cx/3.5;
	}

	double pRight2 = xPos2 + cx/3.5;
	double pLeft2 = xPos2 - cx/3.5;

	if((pRight > pLeft2 && pLeft < pLeft2 && pTop > pBot2) || (pLeft < pRight2 && pRight > pRight2 && pTop > pBot2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Kick2(double xPos, double yPos, double xPos2, double yPos2, sprite pSprite1, sprite pSprite2,
		double extx)
{
	float cx = pSprite1.xres/8.0;
	//float cy = pSprite1.yres;

	double pTop2 = yPos2 +100;
	double pBot = yPos;
	double pRight = xPos + cx/3.5;
	double pLeft = xPos - cx/3.5;
	double pRight2 = xPos2 + cx/3.5;
	double pLeft2 = xPos2 - cx/3.5;


	double extxPos = 0;
	if(extx > 0)
	{
		extxPos = xPos2 + extx;
		pRight2 = extxPos + cx/3.5;
	}
	else if(extx < 0)
	{
		extxPos = xPos2 + extx;
		pLeft2 = extxPos - cx/3.5;
	}

	if((pRight > pLeft2 && pLeft < pLeft2 && pTop2 > pBot) || (pLeft < pRight2 && pRight > pRight2 && pTop2 > pBot))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
