//Author: Matthew Gonzales
//
//
#include "header.h"

Image play[1] = {"fightSprite.png"};

class playerSprite {
	public:
		int health;
		int xres, yres;
		int playerFrame;
		GLuint playerTex;
		GLuint playerTexId;;
		playerSprite() {
			playerTex =0;
			playerTexId =0;
			health = 100;
			xres = 512;
			yres = 64;
			playerFrame = 0;

		}

	private:
		float _x;
		float _y;
		float _width;
		float _height;

} pl;

unsigned char *buildAlphaData(Image *img)
{
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

class SpriteTexture {
	public:
		Image *spriteImage;
		GLuint spriteTexture;
		
		float xc[2];
		float yc[2];
};

class sprite {
	public:
		int xres, yres;
		SpriteTexture spTex;
		int spriteFrame;
		sprite() {
			xres= 512, yres= 64;
			spriteFrame=0;
		}
} sp;

void initSprite() {
	//load the images file into a ppm tructure.
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

void spriteRender() {
	float cx = sp.xres/4.0;
	float cy = sp.yres;
	glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, sp.spTex.spriteTexture);
    glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = sp.spriteFrame % 8;
	int iy = 0;
		
	float tx = (float)ix / 8.0;
	float ty = (float)iy;
    glBegin(GL_QUADS);
        glTexCoord2f(tx, ty+1.0); glVertex2i(0, 0);
        glTexCoord2f(tx, ty); glVertex2i(0, cy);
        glTexCoord2f(tx+0.125, ty); glVertex2i(cx, cy);
        glTexCoord2f(tx+0.125, ty+1.0); glVertex2i(cx, 0);
    glEnd();
    glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}

/*void spritePunch() {
	if (gl.keys[XK_r])
	{
		if (sp.spriteFrame >= 1)
			sp.spriteFrame = 0;
		for(int i = 0; i < 4; i++){
			++sp.spriteFrame;
		}
	}
}*/

void playerHealthRender() {
	Rect r;
	r.bot = 800;
	r.left = 425;
	r.center = 0;
	ggprint16(&r, 32, 0x00ffffff, "Player Health: %i", pl.health);


}



void takePunch() {
	pl.health -= 5;
}

void takeKick() {
	pl.health -= 10;
}
