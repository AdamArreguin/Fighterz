using namespace std;

#include "header.h"


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
	int keyHeldr1;
	int keyHeldf1;
	int keyHeldr2;
	int keyHeldf2;
	char keys[65536];
	int mx, my;
	int menu[2];
	int menu_1[2];
	int menu_2[2];
	int menu_3[2];
	int gameMenu[];
	int STATE;
	int posFlag;
	int secpas;
	Global(){
	    xres = 1280;
	    yres = 720;
	    gravity = 1.5;
	    memset(keys, 0, 65536);
	    keyHeldr1 = 0;
	    keyHeldf1 = 0;
	    keyHeldr2 = 0;
	    keyHeldf2 = 0;
	    posFlag = 1;
	    menu[0] = 790;
	    menu[1] = 490;
	    menu_1[0] = 394;
	    menu_1[1] = 328;
	    menu_2[0] = 520;
	    menu_2[1] = 450;
	    secpas = 0;
	}
} gl;


class Player {
    public:
	sprite sp;
	Timers timers;
	Timers timers2;
	Vec dir;
	Vec pos;
	Vec vel;
	float color[3];
	int animationState;
	bool collisionState;
	int positionState;
	int punchedState;
	int kickedState;
	int checkPunchState;
	int checkKickState;
	int punchCounter;
	int kickCounter;
	int playerHealth;
	int health1;
	int health2;
	int TimerState;
	bool isDead;
    public:
	Player(int x, int posState, int startFrame) {
	    VecZero(dir);
	    pos[0] = x; // Starting point for fighter
	    pos[1] = 10;
	    pos[2] = 0.0f;
	    VecZero(vel);
	    color[0] = color[1] = color[2] = 1.0;
	    animationState = 0;
	    collisionState = false;
	    positionState = posState;
	    sp.spriteFrame = startFrame;
	    playerHealth = 100;
	    punchedState = 0;
	    kickedState = 0;
	    punchCounter = 0;
	    kickCounter = 0;
	    checkPunchState = 0;
	    checkKickState = 0;
	    health1 = 0;
	    health2 = 450;
	    TimerState = 1;
	    isDead = false;

	}
};

Player player(200, 1, 0);
Player player2(800, 2, 8);
//X Windows variables
class X11_wrapper {
    private:
	Display *dpy;
	Window win;
	GLXContext glc;
    public:
	X11_wrapper() {
	    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None 	};
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
	    XStoreName(dpy, win, "FighterZ");
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
	    /*
	    //vars to make blank cursor
	    Pixmap blank;
	    XColor dummy;
	    char data[1] = {0};
	    Cursor cursor;
	    //make a blank cursor
	    blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
	    if (blank == None)
	    std::cout << "error: out of memory." << std::endl;
	    cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0		);
	    XFreePixmap(dpy, blank);
	    //this makes you the cursor. then set it using this function
	    XDefineCursor(dpy, win, cursor);
	    //after you do not need the cursor anymore use this function.
	    //it will undo the last change done by XDefineCursor
	    //(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	    */
	}
} x11;

//function prototypes
void init_opengl();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

//extern prototypes
extern void backGl();
extern void backgroundRender(int xres, int yres);
extern void backgroundRenderTimer(int xres, int yres);
extern void displayName(const char*, int, int);
extern void displayScore(const char*, int,int);
extern void displayScoreOpt(const char*, int, int);
extern void controls (int, int, const char*);
extern void initSprite(sprite&, sprite&);
//extern void spriteRender(sprite,double, double, double, float);
extern int spritePunch(sprite&, int, int, Timers&);
extern int spriteKick(sprite&, int, int, Timers&);
extern void checkPosition(sprite&, sprite&, double, double, int&, int&, int&);
extern int Punch1(double, double,double, double, sprite&, sprite&, double);
extern int Kick1(double, double,double, double, sprite, sprite, double);
extern int Punch2(double, double,double, double, sprite&, sprite&, double);
extern int Kick2(double, double,double, double, sprite, sprite, double);


extern int Punch1(double, double,double, double, sprite, sprite);
extern void drawMenu(int, int);
extern void showControls(int, int, int);
extern int timer(int, int);
//
extern void showTimer(int xres, int yres);
extern void drawHealthBar1(int, int);
extern void drawHealthBar2(int, int);
extern void healthBarOverlay(int, int, int);
extern void healthBarOverlay2(int, int, int);
extern void countdown(int, int);

extern void displayText(int, int, const char* ); 

//
extern int checkGameOver(int&,int&);
extern int checkPlayerStatus(int, int, Vec, Vec, sprite, sprite);
//==========================================================================
// M A I N
//==========================================================================
int main()
{ 

    //int fps = 30;
    //int t0, t1, frame_time;
    //clock_t t;

    logOpen();
    init_opengl();
    srand(time(NULL));
    x11.set_mouse_position(100, 100);
    int done=0;
    //time_t start, end;
    //time(&start);
    //double hold = 0;
    //float elapsed = 0;
    while (!done) {
	while (x11.getXPending()) {
	    XEvent e = x11.getXNextEvent();
	    x11.check_resize(&e);
	    check_mouse(&e);
	    done = check_keys(&e);
	}
	physics();
	//timer for one second
	/*time(&end);
	  elapsed = difftime(end,start);
	  if(gl.secpas == 0)
	  {

	  if(elapsed != hold)
	  {
	  gl.secpas = 1;
	  }
	  }
	  hold = elapsed;*/
	render();
	x11.swapBuffers();
    }
    cleanup_fonts();
    logClose();
    return 0;
}

void init_opengl()
{
    backGl();
    initSprite(player.sp, player2.sp);
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

    // Change state of game when clicking on menu items
    if (gl.STATE == 0) {
	gl.mx = e->xbutton.x;
	gl.my = e->xbutton.y;
	//		cout << gl.mx << " " << gl.my << endl;
	if (e->xbutton.button==1) {
	    // Check which menu item was clicked
	    if (gl.mx < gl.menu[0]  && gl.mx > gl.menu[1] && 
		    gl.my < gl.menu_1[0] && gl.my > gl.menu_1[1]) {
		gl.STATE = 2;
	    } else if (gl.mx < gl.menu[0]  && gl.mx > gl.menu[1] 
		    && gl.my < gl.menu_2[0] && gl.my > gl.menu_2[1])
		gl.STATE = 1;
	}
    } else if (gl.STATE == 1) {
	if(e->xbutton.button == 1) {
	    gl.STATE = 0;
	}
    }

    if (gl.STATE == 3) {
	gl.mx = e->xbutton.x;
	gl.my = e->xbutton.y;

	if (e->xbutton.button == 1) {

	}
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
    //Log("key: %i\n", key)
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
	    exit(0);
	    if (gl.STATE == 2) {
		gl.STATE  = 3;
	    } else if (gl.STATE == 3) {
		gl.STATE = 2;
	    } else {
		return 1;
	    }
	    break;
	case XK_f:
	    break;
	case XK_r:
	    break;
	case XK_Down:
	    break;
	case XK_equal:
	    break;
	case XK_minus:
	    break;
	case XK_w:
	    break;
	case XK_m:
	    gl.STATE = 0;
	    break;
    }
    return 0;
}

void physics()
{
    //Update player position
    player.pos[0] += player.vel[0];
    player.pos[1] += player.vel[1];
    player2.pos[0] += player2.vel[0];
    player2.pos[1] += player2.vel[1];
    //update player velocity due to gravity
    if (player.pos[1] > 80)
    {
	player.vel[1] -= gl.gravity;
    }

    //update player2 velocity when puched
    if(player2.vel[0] > 0)
    {
	player2.vel[0] -= 1.5;

	if(player2.vel[0] < 0)
	{
	    player2.vel[0] = 0;
	}
    }

    if(player2.vel[0] < 0)
    {
	player2.vel[0] += 1.5;

	if(player2.vel[0] > 0)
	{
	    player2.vel[0] = 0;
	}
    }

    if(player.vel[0] > 0)
    {
	player.vel[0] -= 1.5;

	if(player.vel[0] < 0)
	{
	    player.vel[0] = 0;
	}
    }

    if(player.vel[0] < 0)
    {
	player.vel[0] += 01.5;

	if(player.vel[0] > 0)
	{
	    player.vel[0] = 0;
	}
    }




    //Check for collision with window edges
    if (player.pos[0] < -50) {
	player.pos[0] = -50;
	player.vel[0] = 0;
    }
    if (player.pos[0] > 1200) {
	player.pos[0] = 1200;
	player.vel[0] = 0;
    }
    if (player.pos[1] < 70) {
	player.pos[1] += (float)gl.yres;
	player.pos[1] = 70;
	player.vel[1] = 0;
    }
    if (player.pos[1] > (float)gl.yres) {
	player.pos[1] -= (float)gl.yres;
    }

    //---------------------------------------------------
    //check keys pressed now


    //press m for menu
    if (gl.keys[XK_m])
    {
	//if(player.health1 >= 490)
	player.health1 = 0;
	//if(player.health2 <= 0)
	player2.health2 = 450;
    }
    //--------------------------------------------------
if(player.isDead == false) {
    if (gl.keys[XK_w] && player.pos[1] <= 70)
    {
	player.vel[1] += 30;
    }

    if (gl.keys[XK_d])
    {
	if(player.collisionState == false || player.pos[1] > player2.pos[1]+50)
	{
	    player.pos[0] += 7;

	}
    }
    if (gl.keys[XK_a])
    {
	if(player.collisionState == false || player.pos[1] > player2.pos[1]+50)
	{
	    player.pos[0] -= 7;
	}
    }
    if (gl.keys[XK_r] && gl.keyHeldr1 == 0 && player.checkPunchState == 0)
    {
	int Punchval = 0;
	if(player.pos[0] < player2.pos[0])
	{
	    Punchval = Punch1(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
		    player.sp,player2.sp, 5);
	    if (Punchval == 1 && player2.pos[1] < 700 && (player.pos[1] >= player2.pos[1] - 20 && player.pos[1] <= player2.pos[1] + 20))
	    {
		if(player.punchCounter == 2)
		{
		    player2.vel[0] = 20;
		    player2.vel[1] = 15;
		    player2.punchedState = 2;
		    player.punchCounter++;
		}
		else
		{
		    player2.vel[0] = 10;
		    player2.vel[1] = 10;
		    player2.punchedState = 1;
		    player.punchCounter++;

		}
	    }
	}
	else if (player.pos[0] > player2.pos[0])
	{
	    Punchval = Punch1(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
		    player.sp,player2.sp, -5);
	    if(Punchval == 1 && player2.pos[1] < 700 && (player.pos[1] == player2.pos[1]))
	    {
		if(player.punchCounter == 2)
		{
		    player2.vel[0] = -20;
		    player2.vel[1] = 15;
		    player2.punchedState = 2;
		    player.punchCounter++;
		}
		else
		{
		    player2.vel[0] = -10;
		    player2.vel[1] = 10;
		    player2.punchedState = 1;
		    player.punchCounter++;

		}
	    }
	}
	player2.checkPunchState = 1;
	player2.checkKickState = 1;
	player2.punchCounter = 0;
	player2.kickCounter = 0;
	player.animationState = 1;

	if(player.punchCounter == 3)
	{
	    player.checkPunchState = 1;
	    player.punchCounter = 0;
	}

	gl.keyHeldr1 = 1;

    }

    if (!gl.keys[XK_r] && gl.keyHeldr1 == 1)
    {
	//player2.kickedState = 0;
	//player2.punchedState = 0;
	gl.keyHeldr1 = 0;
    } 
    if (gl.keys[XK_f] && gl.keyHeldf1 == 0 && player.checkKickState == 0)
    {
	int Kickval = 0;
	if(player.pos[0] < player2.pos[0])
	{
	    Kickval = Kick1(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
		    player.sp,player2.sp, 5);
	    if (Kickval == 1 && (player.pos[1] >= player2.pos[1] - 10 && player.pos[1] <= player2.pos[1] + 20))
	    {
		if(player.kickCounter == 1)
		{
		    player2.vel[0] = 25;
		    player2.vel[1] = 15;
		    player2.kickedState = 2;
		    player.kickCounter++;
		}
		else
		{
		    player2.vel[0] = 10;
		    player2.vel[1] = 10;
		    player2.kickedState = 1;
		    player.kickCounter++;

		}
	    }
	}
	else if (player.pos[0] > player2.pos[0] && (player.pos[1] == player2.pos[1]))
	{
	    Kickval = Kick1(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
		    player.sp,player2.sp, -5);
	    if(Kickval == 1 && (player.pos[1] <= player2.pos[1] + 10 && player.pos[1] >= player2.pos[1] - 20))
	    {
		if(player.kickCounter == 1)
		{
		    player2.vel[0] = -25;
		    player2.vel[1] = 15;
		    player2.kickedState = 2;
		    player.kickCounter++;
		}
		else
		{
		    player2.vel[0] = -10;
		    player2.vel[1] = 10;
		    player2.kickedState = 1;
		    player.kickCounter++;

		}
	    }
	}
	player2.checkPunchState = 1;
	player2.checkKickState = 1;
	player2.punchCounter = 0;
	player2.kickCounter = 0;
	player.animationState = 2;

	if(player.kickCounter == 2)
	{
	    player.checkKickState = 1;
	    player.kickCounter = 0;
	}

	gl.keyHeldf1 = 1;
    }
    if (!gl.keys[XK_f] && gl.keyHeldf1 == 1)
    {
	//player2.punchedState = 0;
	//player2.kickedState = 0;
	gl.keyHeldf1 = 0;
    }

}

    //update player2 velocity due to gravity
    if (player2.pos[1] > 70)
    {
	player2.vel[1] -= gl.gravity;
    }

    //Check for collision with window edges
    if (player2.pos[0] < -50) {
	player2.pos[0] = -50;
	player2.vel[0] = 0;
    }
    if (player2.pos[0] > 1200) {
	player2.pos[0] = 1200;
	player2.vel[0] = 0;
    }
    if (player2.pos[1] < 70) {
	player2.pos[1] += (float)gl.yres;
	player2.pos[1] = 70;
	player2.vel[1] = 0;
    }
    if (player2.pos[1] > (float)gl.yres) {
	player2.pos[1] -= (float)gl.yres;
    }
    //---------------------------------------------------
    //check keys pressed now
    if(player2.isDead == false)
    {
	if (gl.keys[XK_Up] && player2.pos[1] <= 70)
	{
	    player2.vel[1] += 30;
	}
    if (gl.keys[XK_Right])
    {
	if(player2.collisionState == false || (player2.pos[1] > player.pos[1]+50))
	{
		player2.pos[0] += 7;
	}

    }
    if (gl.keys[XK_Left])
    {
	if(player2.collisionState == false || (player2.pos[1] > player.pos[1]+50))
	{
	    
		player2.pos[0] -= 7;
	    
	}
    }
	if (gl.keys[XK_Shift_R] && gl.keyHeldr2 == 0 && player2.checkPunchState == 0)
	{
	    int Kickval = 0;
	    if(player.pos[0] > player2.pos[0])
	    {
		Kickval = Kick2(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
			player.sp,player2.sp, 5);
		if (Kickval == 1 && (player2.pos[1] >= player.pos[1] - 20 && player2.pos[1] <= player.pos[1] +20))
		{
		    if(player2.kickCounter == 1)
		    {
			player.vel[0] = 25;
			player.vel[1] = 15;
			player.kickedState = 2;
			player2.kickCounter++;
		    }
		    else
		    {
			player.vel[0] = 10;
			player.vel[1] = 10;
			player.kickedState = 1;
			player2.kickCounter++;

		    }
		}
	    }
	
	    else if (player.pos[0] < player2.pos[0])
	    {
		Kickval = Kick2(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
			player.sp,player2.sp, -5);
		if(Kickval == 1 && (player2.pos[1] >= player.pos[1] - 20 && player2.pos[1] <= player.pos[1] + 20))
		{
		    if(player2.kickCounter == 1)
		    {
			player.vel[0] = -25;
			player.vel[1] = 15;
			player.kickedState = 2;
			player2.kickCounter++;
		    }
		    else
		    {
			player.vel[0] = -10;
			player.vel[1] = 10;
			player.kickedState = 1;
			player2.kickCounter++;

		    }	
		}
	    }
	    player.checkPunchState = 1;
	    player.checkKickState = 1;
	    player.punchCounter = 0;
	    player.kickCounter = 0;
	    player2.animationState = 2;

	    if(player2.kickCounter == 2)
	    {
		player2.checkPunchState = 1;
		player2.kickCounter = 0;
	    }
	    cout << "kicked state: " << player.kickedState << endl;				

	    gl.keyHeldr2 = 1;
	}


	if (!gl.keys[XK_Shift_R])
	{
	   // player.punchedState = 0;
	   // player.kickedState = 0;
	    gl.keyHeldr2 = 0;
	} 
	if (gl.keys[XK_Return] && gl.keyHeldf2 == 0 && player2.checkKickState == 0)
	{
	    int Punchval = 0;
	    if(player.pos[0] > player2.pos[0])
	    {
		Punchval = Punch2(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
			player.sp,player2.sp, 5);
		if (Punchval == 1 && (player2.pos[1] >= player.pos[1] - 20 && player2.pos[1] <= player.pos[1]))
		{
		    if(player2.punchCounter == 2)
		    {
			player.vel[0] = 20;
			player.vel[1] = 15;
			player.punchedState = 2;
			player2.punchCounter++;
		    }
		    else
		    {
			player.vel[0] = 10;
			player.vel[1] = 10;
			player.punchedState = 1;
			player2.punchCounter++;

		    }
		}
	    }
	    else if (player.pos[0] < player2.pos[0])
	    {
		Punchval = Punch2(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
			player.sp,player2.sp, -5);
		if(Punchval == 1 && (player2.pos[1] >= player.pos[1] - 20 && player2.pos[1] <= player.pos[1]))
		{
		    if(player2.punchCounter == 2)
		    {
			player.vel[0] = -20;
			player.vel[1] = 15;
			player.punchedState = 2;
			player2.punchCounter++;
		    }
		    else
		    {
			player.vel[0] = -10;
			player.vel[1] = 10;
			player.punchedState = 1;
			player2.punchCounter++;

		    }
		}
	    }
	    player.checkPunchState = 1;
	    player.checkKickState = 1;
	    player.punchCounter = 0;
	    player.kickCounter = 0;
	    player2.animationState = 1; 

	    if(player2.punchCounter == 3)
	    {
		player2.checkKickState = 1;
		player2.punchCounter = 0;
	    }


	    gl.keyHeldf2 = 1;

	}
	if (!gl.keys[XK_Return])
	{
	    //player.punchedState = 0;
	    //player.kickedState = 0;
	    gl.keyHeldf2 = 0;
	} 
    }
	//nudge player if colliding
	if(player.collisionState == true && (player.pos[0] < player2.pos[0]) && (player.pos[1] == player2.pos[1]))
	{
	    player.pos[0] -= 1;
	}
	else if (player.collisionState == true && (player.pos[0] > player2.pos[0]) && (player.pos[1] == player2.pos[1]))
	{
	    player.pos[0] += 1;
	}

	if(player2.collisionState == true && (player2.pos[0] < player.pos[0]) && (player.pos[1] == player2.pos[1]))
	{
	    player2.pos[0] -= 1;
	}
	else if (player2.collisionState == true && (player2.pos[0] > player.pos[0]) && (player.pos[1] == player2.pos[1]))
	{
	    player2.pos[0] += 1;
	}	
    }



void render() 
{
    if (gl.STATE == 0 || gl.STATE == 3) {
	drawMenu(gl.xres, gl.yres);
    } else if (gl.STATE == 1) {
	glClearColor(.1,.1,.6,1);
	glClear(GL_COLOR_BUFFER_BIT);
	int x = gl.xres * .25;
	int y = gl.yres;
	showControls(x, y, 1);
	x = gl.xres * .75;
	showControls(x,y,2);
    } else if (gl.STATE == 2) {
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	if(PROFILING_ON != 0)
	    backgroundRenderTimer(gl.xres,gl.yres);
	else
	    backgroundRender(gl.xres,gl.yres);

	//Display player names
	const char* P1 = "Player 1";
	const char* P2 = "Player 2";
	displayName(P1, 900, 1);
	displayName(P2, 900, 2);
	const char* SC = "Scores :";
	if(PROFILING_ON !=0)
	{
	    displayScore(SC,800,1);
	    displayScoreOpt(SC,800,1);
	}

	//Display keys to press for menu options

	
	displayText(gl.xres/2,gl.yres/1.2,"Press 'm' for menu");
	displayText(gl.xres/2,gl.yres/1.2 - 30, "Press 'esc' to quit game");

	/*
	 * Commenting out for clean look and used when we create
	 * a main menu
	//Display controls
	const char* CONTROLS = "CONTROLS";
	const char* LINE = "-------------------";
	const char* JUMP = "Jump: W";
	const char* LEFT = "Move Left: A";
	const char* RIGHT = "Move Right: D";
	controls(75, 850, CONTROLS);
	controls(87, 840, LINE);
	controls(80, 795, LEFT);
	controls(83, 770, RIGHT);
	controls(60, 820, JUMP);
	*/


	checkPosition(player.sp, player2.sp, player.pos[0], player2.pos[0], gl.posFlag, player.positionState, player2.positionState);
	if (player.animationState == 1) 
	{
	    if(player.positionState == 1) 
	    {
		// return player.animation state back to 0 after spritePunch();
		player.animationState = spritePunch(player.sp,0,3, player.timers);
	    }
	    else if( player.positionState == 2) 
	    {
		player.animationState = spritePunch(player.sp, 8,11, player.timers);
	    } 
	    if(player.animationState == 0) 
	    {
		player2.checkPunchState = 0;
		player2.checkKickState = 0;
	    }
	}

	if (player2.animationState == 1) 
	{
	    if (player2.positionState == 2) 
	    {
		// return player.animation state back to 0 after spritePunch(); 
		player2.animationState = spritePunch(player2.sp,8,11,player2.timers2);
	    }
	    else if (player2.positionState == 1) 
	    {
		player2.animationState = spritePunch(player2.sp,0,3, player2.timers2);
	    } 
	    if(player2.animationState == 0) 
	    {
		player.checkPunchState = 0;
		player.checkKickState = 0;
	    }
	}

	if (player.animationState == 2) 
	{
	    // return player.animation state back to 0 after spriteKick();
	    if(player.positionState == 1)
		player.animationState = spriteKick(player.sp, 4, 7,player.timers);
	    else if( player.positionState == 2) 
	    {
		player.animationState = spriteKick(player.sp, 12,15,player.timers);
	    }
	    if(player.animationState == 0) 
	    {
		player2.checkPunchState = 0;
		player2.checkKickState = 0;
	    }		
	}

	if (player2.animationState == 2) 
	{
	    // return player.animation state back to 0 after spriteKick();
	    if (player2.positionState == 2)
		player2.animationState = spriteKick(player2.sp,12,15,player2.timers2);
	    else if (player2.positionState == 1) 
	    {
		player2.animationState = spriteKick(player2.sp,4,7,player2.timers2);
	    } 
	    if(player2.animationState == 0) 
	    {
		player.checkPunchState = 0;
		player.checkKickState = 0;
	    }		
	}
	int gameStatus = checkPlayerStatus(player.health1,player2.health2,player.pos,player2.pos,player.sp,player2.sp);
	Rect r;
	r.bot = gl.yres/2;
	r.left = gl.xres/2;
	if(gameStatus == 1)
	{
	    player.isDead = true;
	    ggprint16(&r, 32, 0x00ffffff, "Player 2 Wins!");
	}
	if(gameStatus == 2)
	{
	    player2.isDead = true;
	    ggprint16(&r, 32, 0x00ffffff, "Player 1 Wins!");
	}
	/*
	   int gameOverStatus = checkGameOver(player.health1,player2.health2);
	   if(gameOverStatus == 1)
	   {
	   if(player2.pos[0] > player.pos[0])
	   spriteRender(player.sp,player.pos[0], player.pos[1], player.pos[2],90);
	   else
	   spriteRender(player.sp,player.pos[0], player.pos[1], player.pos[2],-90);

	   spriteRender(player2.sp,player2.pos[0], player2.pos[1], player2.pos[2],0);
	   }
	   else if(gameOverStatus == 2)
	   {
	   spriteRender(player.sp,player.pos[0], player.pos[1], player.pos[2],0);
	   if(player.pos[0] > player2.pos[0])
	   spriteRender(player2.sp,player2.pos[0], player2.pos[1], player2.pos[2],90);
	   else
	   spriteRender(player2.sp,player2.pos[0], player2.pos[1], player2.pos[2],-90);
	   }
	   else
	   {
	   spriteRender(player.sp,player.pos[0], player.pos[1], player.pos[2],0);
	   spriteRender(player2.sp,player2.pos[0], player2.pos[1], player2.pos[2],0);
	   }
	   */
	//check collision of each sprite
	player.collisionState = checkCollision(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
		player.sp,player2.sp);
	player2.collisionState = checkCollision(player2.pos[0],player2.pos[1],player.pos[0],player.pos[1],
		player2.sp,player.sp);


	//Display healthbars
	drawHealthBar1(gl.xres, gl.yres);
	drawHealthBar2(gl.xres, gl.yres);

	if(player2.punchedState == 1)
	{
	    player2.health2 -= 15;
	    player2.punchedState = 0;
	} else if(player2.kickedState == 1)
	{
	    player2.health2 -= 20;
	    player2.kickedState = 0;
	} 

	if(player2.punchedState == 2)
	{
	    player2.health2 -= 25;
	    player2.punchedState = 0;
	} else if(player2.kickedState == 2)
	{
	    player2.health2 -= 35;
	    player2.kickedState = 0;
	}

	if(player.punchedState == 2)
	{
	    player.health1 += 25;
	    player.punchedState = 0;
	} else if(player.kickedState == 2)
	{
	    player.health1 += 35;
	    player.kickedState = 0;
	}

	if(player.punchedState == 1)
	{
	    player.health1 += 15;
	    player.punchedState = 0;
	} else if(player.kickedState == 1)
	{
	    player.health1 += 20;
	    player.kickedState = 0;
	}


	//gl.STATE = checkGameOver(player.health1, player2.health2);
	//gl.STATE = checkGameOver(player2.health2);

	healthBarOverlay(gl.xres, gl.yres, player.health1);
	healthBarOverlay2(gl.xres, gl.yres, player2.health2);
	countdown(gl.xres, gl.yres);
	//render background
	//backgroundRender();
    }
}







