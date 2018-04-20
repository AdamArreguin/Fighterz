//Eduardo's stuff
#include <time.h>
#include <stdio.h>
#include "header.h"

Image img[1] = {"seamless_back.jpg"};
Image img2[1] = {"opt2_back.jpg"};

using namespace std;

class Texture {
	public:
		Image *backImage;
		GLuint backTexture;
		float xc[2];
		float yc[2];
};

class background {
	public:
		//int xres, yres;
		Texture tex;
		background() {
			//	xres= s->width, yres= s->height;
		}
} b;

bool checkCollision(double xPos, double yPos, double xPos2, double yPos2, sprite pSprite1, sprite pSprite2)
{
	
	float cx = pSprite1.xres/8.0;
	//float cy = pSprite1.yres;

	double pRight = xPos + cx/3.5;
	double pLeft = xPos - cx/3.5;

	double pRight2 = xPos2 + cx/3.5;
	double pLeft2 = xPos2 - cx/3.5;

	if((pRight > pLeft2 && pLeft < pLeft2) || (pLeft < pRight2 && pRight > pRight2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void backGl()
{
	//load the images file into a ppm structure.
	//
	b.tex.backImage = &img[0];
	//create opengl texture elements
	glGenTextures(1, &b.tex.backTexture);
	int w = b.tex.backImage->width;
	int h = b.tex.backImage->height;
	glBindTexture(GL_TEXTURE_2D, b.tex.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, b.tex.backImage->data);
	b.tex.xc[0] = 0.0;
	b.tex.xc[1] = 0.25;
	b.tex.yc[0] = 0.0;
	b.tex.yc[1] = 1.0;

}

void displayTime(int xres, int yres, const char *s)
{
	Rect r;
	r.left = xres/4;
	r.bot = yres/4;
	ggprint16(&r, 32, 0x00ffffff, s);
}

void backgroundRender(int xres, int yres)
{
    glColor3f(1.0f,1.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, b.tex.backTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(b.tex.xc[0], b.tex.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(b.tex.xc[0], b.tex.yc[0]); glVertex2i(0, yres);
        glTexCoord2f(b.tex.xc[1], b.tex.yc[0]); glVertex2i(xres, yres);
        glTexCoord2f(b.tex.xc[1], b.tex.yc[1]); glVertex2i(xres, 0);
    glEnd();
}

void backgroundRenderTimer(int xres, int yres)
{

	clock_t timer;
	char s[100];
	char name[20];
	float diff;

	static double elapsedTime;

	timer = clock();

	///////////////////////////////////////////////////////////
	glColor3f(1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, b.tex.backTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(b.tex.xc[0], b.tex.yc[1]); glVertex2i(0, 0);
	glTexCoord2f(b.tex.xc[0], b.tex.yc[0]); glVertex2i(0, yres);
	glTexCoord2f(b.tex.xc[1], b.tex.yc[0]); glVertex2i(xres, yres);
	glTexCoord2f(b.tex.xc[1], b.tex.yc[1]); glVertex2i(xres, 0);
	glEnd();
	///////////////////////////////////////////////////////////

	//load the images file into a ppm structure.
	//
	b.tex.backImage = &img2[0];
	//create opengl texture elements
	glGenTextures(1, &b.tex.backTexture);
	int w = b.tex.backImage->width;
	int h = b.tex.backImage->height;
	glBindTexture(GL_TEXTURE_2D, b.tex.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, b.tex.backImage->data);
	b.tex.xc[0] = 0.0;
	b.tex.xc[1] = 0.25;
	b.tex.yc[0] = 0.0;
	b.tex.yc[1] = 1.0;

	//////////////////////////////////////////////////////////

    //clock_t timer2;
    //char s2[100];
    //float diff2;
	if(PROFILING_ON != 0)
	{
		clock_t timer2;
		char s2[100];
		float diff2;

		static double elapsedTime2;

		timer2 = clock();

		glColor3f(1.0f,1.0f,1.0f);
		glBindTexture(GL_TEXTURE_2D, b.tex.backTexture);
		glBegin(GL_QUADS);
		glTexCoord2f(b.tex.xc[0], b.tex.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(b.tex.xc[0], b.tex.yc[0]); glVertex2i(0, yres);
		glTexCoord2f(b.tex.xc[1], b.tex.yc[0]); glVertex2i(xres, yres);
		glTexCoord2f(b.tex.xc[1], b.tex.yc[1]); glVertex2i(xres, 0);
		glEnd();

		//////////////////////////////////////////////////////////


		sprintf(name, "Eduardo Barajas");
		displayTime(xres + 280, yres + 120,name);

		diff = clock() - timer;
		elapsedTime += diff;

		sprintf(s, "Elapsed_Time: %f", (elapsedTime/CLOCKS_PER_SEC));
		displayTime(xres,yres,s);


		diff2 = clock() - timer2;
		elapsedTime2 += diff2;

    sprintf(s2, "optimized Elapsed_Time: %f", (elapsedTime2/CLOCKS_PER_SEC));
    displayTime(xres,yres - 120,s2);
		sprintf(s2, "optimized Elapsed_Time: %f", (elapsedTime2/CLOCKS_PER_SEC));
		displayTime(xres,yres - 120,s2);
	}
}


