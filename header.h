#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;
#include <unistd.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "fonts.h"
#include "matthewG.h"
//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt     Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%(a))
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)     ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]

#define PROFILING_ON false

//constants
const float TIMESLICE = 1.0f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;
//image
class Image {
public:
    int width, height;
    unsigned char *data;
    ~Image() { delete [] data; }
    Image(const char *fname) {
        if (fname[0] == '\0')
            return;
        //printf("fname **%s**\n", fname);
        char name[40];
        strcpy(name, fname);
        int slen = strlen(name);
        name[slen-4] = '\0';
        //printf("name **%s**\n", name);
        char ppmname[80];
        sprintf(ppmname,"%s.ppm", name);
        //printf("ppmname **%s**\n", ppmname);
        char ts[100];
        //system("convert eball.jpg eball.ppm");
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
        //sprintf(ts, "%s", name);
        FILE *fpi = fopen(ppmname, "r");
        if (fpi) {
            char line[200];
            fgets(line, 200, fpi);
            fgets(line, 200, fpi);
            while (line[0] == '#')
                fgets(line, 200, fpi);
            sscanf(line, "%i %i", &width, &height);
            fgets(line, 200, fpi);
            //get pixel data
            int n = width * height * 3;
            data = new unsigned char[n];
            for (int i=0; i<n; i++)
                data[i] = fgetc(fpi);
            fclose(fpi);
        } else {
            printf("ERROR opening image: %s\n",ppmname);
            exit(0);
        }
        unlink(ppmname);
    }
};


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
        double kickDelay;
        double punchDelay;
        sprite() {
            xres= 1024, yres= 64;
            spriteFrame=0;
            punchDelay = 0.075;
            kickDelay = 0.09;
        }
};


//external functions
extern bool checkCollision(double Xpos1, double yPos1, double xPos2, double yPos2, sprite pSprite1, sprite pSprite2);
extern bool grabResources(double xPos1, double yPos1, double xPos2, double yPos2);
