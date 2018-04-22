//Author: Matthew Gonzales
//
//
#include "header.h"
double elapsedTime;
char t[200];
Image play[1] = {"fighterSprite.png"};


unsigned char *buildAlphaData(Image *img){
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

//TODO remove duplicate functions and call sprite class from main.
//TODO change player direction based on other players position 

void initSprite(sprite &sp) {
	//load the images file into a ppm structure.
	//
	sp.spTex.spriteImage = &play[0];
	//create opengl texture elements
	glGenTextures(1, &sp.spTex.spriteTexture);
	int w = sp.spTex.spriteImage->width;
	int h = sp.spTex.spriteImage->height;
	glBindTexture(GL_TEXTURE_2D, sp.spTex.spriteTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	unsigned char *spriteData = buildAlphaData(&play[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
	sp.spTex.xc[0] = 0.0;
	sp.spTex.xc[1] = 0.125;
	sp.spTex.yc[0] = 0.0;
	sp.spTex.yc[1] = 1.0;

}

void funcTimer(const char *t) {

	Rect r;
	r.bot = 550;
	r.left = 50;
	r.center = 0;

	ggprint16(&r, 8, 0x00ffff00, "matthew G.");
	r.bot = 500;
	ggprint16(&r, 8, 0x00ffff00, t);
}


void spriteRender(sprite sp,double xPos, double yPos, double zPos) {
	clock_t timer;
	timer = clock();
	float cx = sp.xres/8.0;
	float cy = sp.yres;
	glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, sp.spTex.spriteTexture);
    glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = sp.spriteFrame % 16;
	int iy = 0;
		
	float tx = (float)ix / 16.0;
	float ty = (float)iy;
	glTranslatef(xPos, yPos, zPos);
    glBegin(GL_QUADS);
        glTexCoord2f(tx, ty+1.0); glVertex2i(0, 0);
        glTexCoord2f(tx, ty); glVertex2i(0, cy+96);
        glTexCoord2f(tx+0.0625, ty); glVertex2i(cx, cy+96);
        glTexCoord2f(tx+0.0625, ty+1.0); glVertex2i(cx, 0);
    glEnd();
    glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	if(PROFILING_ON != 0)
    {
	elapsedTime += clock() - timer;
	sprintf(t,"Elapsed Time: %f", (elapsedTime/CLOCKS_PER_SEC));
	funcTimer(t);
	}
}
/*
bool grabResources(double xPos1, double yPos1, double xPos2, double yPos2)
{ 
	float cx = sp.xres/8.0;
	float cy = sp.yres;
	float cx2 = sp2.xres/8.0;
	float cy2 = sp2.yres;

	return checkCollision(xPos1,yPos1,cx,cy,xPos2,yPos2,cx2,cy2);
	
}
*/
class Timers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec animationTime;
	Timers() {
		physicsRate = 1.0 / 30.0;
		oobillion = 1.0 / 1e9;
	}
	double timeDiff(struct timespec *start, struct timespec *end) {
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}
	void timeCopy(struct timespec *dest, struct timespec *source) {
		memcpy(dest, source, sizeof(struct timespec));
	}
	void recordTime(struct timespec *t) {
		clock_gettime(CLOCK_REALTIME, t);
	}
} timers;

int spritePunch(sprite &sp, int start, int end) {
	cout << "start: "<< start <<endl;
	cout << "punch: "<<sp.spriteFrame << endl;
	cout << "end: " << end << endl;
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.animationTime, &timers.timeCurrent);
	if(timeSpan > sp.punchDelay)
	{
		if(sp.spriteFrame < start)
			sp.spriteFrame = start;
		else if (sp.spriteFrame >= end){
			sp.spriteFrame = start;
			return 0;
		}
		++sp.spriteFrame;
		timers.recordTime(&timers.animationTime);
		return 1;
	}
	else 
		return 1;
		
}

int spriteKick(sprite &sp, int start, int end) {
	cout << "start: "<< start <<endl;
	cout << "kick: "<<sp.spriteFrame << endl;
	cout << "end: " << end << endl;
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&timers.animationTime, &timers.timeCurrent);
	if(timeSpan > sp.kickDelay)
	{
		if (sp.spriteFrame < start)
			sp.spriteFrame = start;
		else if (sp.spriteFrame >= end){
			sp.spriteFrame = start;
			return 0;
		}
		++sp.spriteFrame;
		timers.recordTime(&timers.animationTime);
		return 2;
		}
	else 
		return 2;

}

int checkPosition(sprite &p1, sprite &p2,double xPos1, double xPos2, int &posFlag, int &pos1, int &pos2) {
	double centerP1 = xPos1;

	double centerP2 = xPos2;

	if(centerP1 > centerP2 && posFlag == 1) {
		p1.spriteFrame = 8;
		p2.spriteFrame = 0;
		posFlag = 2;
		pos1 = 2;
		pos2 = 1;
		cout << "posFlag: " << posFlag << " pos1: " << pos1 << endl;
		return 2;
	}
	else if (centerP2 > centerP1 && posFlag == 2) {
		p1.spriteFrame = 0;
		p2.spriteFrame = 8;
		posFlag = 1;
		pos1 = 1;
		pos2 = 2;
		return 1;
	} 
	return 0;
}

