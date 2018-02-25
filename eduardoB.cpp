//Eduardo's stuff
#include "header.h"

Image img[1] = {"seamless_back.jpg"};

class Texture {
public:
    Image *backImage;
    GLuint backTexture;
    float xc[2];
    float yc[2];
};

class background {
    public:
	int xres, yres;
	Texture tex;
	background() {
	   xres= 1250, yres= 900;
	}
} b;

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

void backgroundRender()
{
    glColor3f(1.0f,1.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, b.tex.backTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(b.tex.xc[0], b.tex.yc[1]); glVertex2i(0, 0);
        glTexCoord2f(b.tex.xc[0], b.tex.yc[0]); glVertex2i(0, b.yres);
        glTexCoord2f(b.tex.xc[1], b.tex.yc[0]); glVertex2i(b.xres, b.yres);
        glTexCoord2f(b.tex.xc[1], b.tex.yc[1]); glVertex2i(b.xres, 0);
    glEnd();

}
