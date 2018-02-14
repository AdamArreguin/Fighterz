// test
//
//
//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//mod spring 2018: X11 wrapper class
//This program is a game starting point for a 3350 project.
//
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
#include "log.h"
#include "fonts.h"

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%(a))
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float TIMESLICE = 1.0f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;

//-----------------------------------------------------------------------------
//Setup timers
const double OOBILLION = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

class Global {
    public:
	int xres, yres;
	float gravity;
	char keys[65536];
	Global() {
	    xres = 1250;
	    yres = 900;
	    gravity = 1.5;
	    memset(keys, 0, 65536);
	}
} gl;

class Ship {
    public:
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
    public:
	Ship() {
	    VecZero(dir);
	    pos[0] = 200; // Starting point for fighter 1
	    pos[1] = 10;
	    pos[2] = 0.0f;
	    VecZero(vel);
	    angle = 0.0;
	    color[0] = color[1] = color[2] = 1.0;
	}
};

class Game {
    public:
	Ship ship;
	int nasteroids;
	int nbullets;
	struct timespec bulletTimer;
	struct timespec mouseThrustTimer;
    public:
	Game(){
	    nasteroids = 0;
	    nbullets = 0;
	    clock_gettime(CLOCK_REALTIME, &bulletTimer);
	}
	~Game() {
	}
} g;

//X Windows variables
class X11_wrapper {
    private:
	Display *dpy;
	Window win;
	GLXContext glc;
    public:
	X11_wrapper() {
	    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	    XSetWindowAttributes swa;
	    setup_screen_res(gl.xres, gl.yres);
	    dpy = XOpenDisplay(NULL);
	    if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server" << std::endl;
		exit(EXIT_FAILURE);
	    }
	    Window root = DefaultRootWindow(dpy);
	    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	    if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	    } 
	    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	    swa.colormap = cmap;
	    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
		StructureNotifyMask | SubstructureNotifyMask;
	    win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
		    vi->depth, InputOutput, vi->visual,
		    CWColormap | CWEventMask, &swa);
	    set_title();
	    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	    glXMakeCurrent(dpy, win, glc);
	    show_mouse_cursor(0);
	}
	~X11_wrapper() {
	    XDestroyWindow(dpy, win);
	    XCloseDisplay(dpy);
	}
	void set_title() {
	    //Set the window title bar.
	    XMapWindow(dpy, win);
	    XStoreName(dpy, win, "Asteroids template");
	}
	void check_resize(XEvent *e) {
	    //The ConfigureNotify is sent by the
	    //server if the window is resized.
	    if (e->type != ConfigureNotify)
		return;
	    XConfigureEvent xce = e->xconfigure;
	    if (xce.width != gl.xres || xce.height != gl.yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
	    }
	}
	void reshape_window(int width, int height) {
	    //window has been resized.
	    setup_screen_res(width, height);
	    glViewport(0, 0, (GLint)width, (GLint)height);
	    glMatrixMode(GL_PROJECTION); glLoadIdentity();
	    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	    set_title();
	}
	void setup_screen_res(const int w, const int h) {
	    gl.xres = w;
	    gl.yres = h;
	}
	void swapBuffers() {
	    glXSwapBuffers(dpy, win);
	}
	bool getXPending() {
	    return XPending(dpy);
	}
	XEvent getXNextEvent() {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    return e;
	}
	void set_mouse_position(int x, int y) {
	    XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	}
	void show_mouse_cursor(const int onoff) {
	    if (onoff) {
		//this removes our own blank cursor.
		XUndefineCursor(dpy, win);
		return;
	    }
	    //vars to make blank cursor
	    Pixmap blank;
	    XColor dummy;
	    char data[1] = {0};
	    Cursor cursor;
	    //make a blank cursor
	    blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
	    if (blank == None)
		std::cout << "error: out of memory." << std::endl;
	    cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
	    XFreePixmap(dpy, blank);
	    //this makes you the cursor. then set it using this function
	    XDefineCursor(dpy, win, cursor);
	    //after you do not need the cursor anymore use this function.
	    //it will undo the last change done by XDefineCursor
	    //(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	}
} x11;

//function prototypes
void init_opengl();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

//==========================================================================
// M A I N
//==========================================================================
int main()
{
    logOpen();
    init_opengl();
    srand(time(NULL));
    x11.set_mouse_position(100, 100);
    int done=0;
    while (!done) {
	while (x11.getXPending()) {
	    XEvent e = x11.getXNextEvent();
	    x11.check_resize(&e);
	    check_mouse(&e);
	    done = check_keys(&e);
	}
	physics();
	render();
	x11.swapBuffers();
    }
    cleanup_fonts();
    logClose();
    return 0;
}

void init_opengl()
{
    //OpenGL initialization
    glViewport(0, 0, gl.xres, gl.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
}

void normalize2d(Vec v)
{
    Flt len = v[0]*v[0] + v[1]*v[1];
    if (len == 0.0f) {
	v[0] = 1.0;
	v[1] = 0.0;
	return;
    }
    len = 1.0f / sqrt(len);
    v[0] *= len;
    v[1] *= len;
}

void check_mouse(XEvent *e)
{
    //Was a mouse button clicked?
    if (e->type != ButtonPress &&
	    e->type != ButtonRelease &&
	    e->type != MotionNotify) {
	return;
    }
    if (e->xbutton.button==3) {
	//Right button is down
    }
}

int check_keys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    if (e->type != KeyPress && e->type != KeyRelease)
	return 0;
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    //Log("key: %i\n", key);
    if (e->type == KeyRelease) {
	gl.keys[key]=0;
	if (key == XK_Shift_L || key == XK_Shift_R)
	    shift=0;
	return 0;
    }
    gl.keys[key]=1;
    if (key == XK_Shift_L || key == XK_Shift_R) {
	shift=1;
	return 0;
    }
    (void)shift;
    switch (key) {
	case XK_Escape:
	    return 1;
	case XK_f:
	    break;
	case XK_s:
	    break;
	case XK_Down:
	    break;
	case XK_equal:
	    break;
	case XK_minus:
	    break;
	case XK_w:
	    break;
    }
    return 0;
}

void physics()
{
    //Update ship position
    g.ship.pos[0] += g.ship.vel[0];
    g.ship.pos[1] += g.ship.vel[1];

    //update ship velocity due to gravity
    if (g.ship.pos[1] > 10)
    {
	g.ship.vel[1] -= gl.gravity;
    }

    //Check for collision with window edges
    if (g.ship.pos[0] < 15) {
	g.ship.pos[0] = 15;
    }
    if (g.ship.pos[0] > 1235) {
	g.ship.pos[0] = 1235;
    }
    if (g.ship.pos[1] < 10) {
	g.ship.pos[1] += (float)gl.yres;
	g.ship.pos[1] = 10;
	g.ship.vel[1] = 0;
    }
    if (g.ship.pos[1] > (float)gl.yres) {
	g.ship.pos[1] -= (float)gl.yres;
    }

    //---------------------------------------------------
    //check keys pressed now

    if (gl.keys[XK_w] && g.ship.pos[1] <= 10)
    {
	g.ship.vel[1] += 30;
    }

    if (gl.keys[XK_d])
    {
	g.ship.pos[0] += 10;
    }
    if (gl.keys[XK_a])
    {
	g.ship.pos[0] -= 10;
    }
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //-------------
    //Draw the ship
    glColor3fv(g.ship.color);
    glPushMatrix();
    glTranslatef(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2]);
    glRotatef(g.ship.angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-12.0f, -10.0f);
    glVertex2f(  0.0f, 20.0f);
    glVertex2f(  0.0f, -6.0f);
    glVertex2f(  0.0f, -6.0f);
    glVertex2f(  0.0f, 20.0f);
    glVertex2f( 12.0f, -10.0f);
    glEnd();
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();

}






