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
		int keyHeldr;
		int keyHeldf;
		char keys[65536];
		Global(){
			xres = 1280;
			yres = 720;
			gravity = 1.5;
			memset(keys, 0, 65536);
			keyHeldr = 0;
			keyHeldf = 0;
		}
} gl;


class Player {
    public:
        sprite sp;
        Vec dir;
        Vec pos;
        Vec vel;
        float color[3];
        int animationState;
        bool collisionState;
    public:
        Player(int x) {
            VecZero(dir);
            pos[0] = x; // Starting point for fighter
            pos[1] = 10;
            pos[2] = 0.0f;
            VecZero(vel);
            color[0] = color[1] = color[2] = 1.0;
            animationState = 0;
            collisionState = false;

        }
};

Player player(200);
Player player2(800);
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

//extern prototypes
extern void backGl();
extern void backgroundRender(int xres, int yres);
extern void displayName(const char*, int, int);
extern void displayScore(const char*, int,int);
extern void displayScoreOpt(const char*, int, int);
extern void controls (int, int, const char*);
extern void initSprite(sprite&);
//extern void initSprite2();
extern void spriteRender(sprite,double, double, double);
//extern void spriteRenderRight(double, double, double);
extern int spritePunch(sprite&,int, int);
extern int spriteKick(sprite&);
extern int Punch1(double, double,double, double, sprite, sprite);
//extern int spritePunchRight(int, int);
//extern int spriteKickRight();

//
extern void showTimer(int xres, int yres);
extern void drawHealthBar1(int, int);
extern void drawHealthBar2(int, int);
extern void healthBarOverlay(int, int);
extern void healthBarOverlay2(int, int);
extern void countdown(int, int);
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
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		physics();
		//t = clock();
		//t0 = t;
		render();
		//spritePunch();
		//t1 = t;
		//frame_time = t1-t0;
		//sleep(1/fps - frame_time);
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}

void init_opengl()
{
	backGl();
	initSprite(player.sp);
	initSprite(player2.sp);
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
	if (gl.keyHeldr == 1 && e->type == KeyRelease){
		gl.keyHeldr =0;
	}
	if (gl.keyHeldf == 1 && e->type == KeyRelease){
		gl.keyHeldf =0;
	}

	(void)shift;
	switch (key) {
		case XK_Escape:
			return 1;
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
	}
	return 0;
}

void physics()
{
	//Update player position
	player.pos[0] += player.vel[0];
	player.pos[1] += player.vel[1];

	//update player velocity due to gravity
	if (player.pos[1] > 10)
	{
		player.vel[1] -= gl.gravity;
	}

	//Check for collision with window edges
	if (player.pos[0] < 15) {
		player.pos[0] = 15;
	}
	if (player.pos[0] > 1235) {
		player.pos[0] = 1235;
	}
	if (player.pos[1] < 10) {
		player.pos[1] += (float)gl.yres;
		player.pos[1] = 10;
		player.vel[1] = 0;
	}
	if (player.pos[1] > (float)gl.yres) {
		player.pos[1] -= (float)gl.yres;
	}

	//---------------------------------------------------
	//check keys pressed now

	if (gl.keys[XK_w] && player.pos[1] <= 10)
	{
		player.vel[1] += 30;
	}

	if (gl.keys[XK_d])
	{
		if(player.collisionState == false || player.pos[1] > player2.pos[1])
		{
			player.pos[0] += 10;
		}
	}
	if (gl.keys[XK_a])
	{
		if(player.collisionState == false || player.pos[1] > player2.pos[1])
		{
			player.pos[0] -= 10;
		}
	}
	if (gl.keys[XK_r] && gl.keyHeldr == 0)
	{
		int Punchval = Punch1(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
							player.sp,player2.sp);
		if (Punchval == 1 && (player.pos[0] < player2.pos[0]))
		{
			player2.pos[0] += 10;
		}
		player.animationState = 1;
		gl.keyHeldr = 1;
	}
	if (!gl.keys[XK_r] && gl.keyHeldr == 1)
	{

		gl.keyHeldr = 0;
	} 
	if (gl.keys[XK_f] && gl.keyHeldf == 0)
	{

		player.animationState = 2;
		gl.keyHeldf = 1;
	}
	if (!gl.keys[XK_f] && gl.keyHeldf == 1)
	{

		gl.keyHeldf = 0;
	} 
	player2.pos[0] += player2.vel[0];
	player2.pos[1] += player2.vel[1];

	//update player2 velocity due to gravity
	if (player2.pos[1] > 10)
	{
		player2.vel[1] -= gl.gravity;
	}

	//Check for collision with window edges
	if (player2.pos[0] < 15) {
		player2.pos[0] = 15;
	}
	if (player2.pos[0] > 1235) {
		player2.pos[0] = 1235;
	}
	if (player2.pos[1] < 10) {
		player2.pos[1] += (float)gl.yres;
		player2.pos[1] = 10;
		player2.vel[1] = 0;
	}
	if (player2.pos[1] > (float)gl.yres) {
		player2.pos[1] -= (float)gl.yres;
	}

	//---------------------------------------------------
	//check keys pressed now

	if (gl.keys[XK_Up] && player2.pos[1] <= 10)
	{
		player2.vel[1] += 30;
	}

	if (gl.keys[XK_Right])
	{
		player2.pos[0] += 10;
	}
	if (gl.keys[XK_Left])
	{
		player2.pos[0] -= 10;
	}
	if (gl.keys[XK_Shift_R] && gl.keyHeldr == 0)
	{

		player2.animationState = 1;
		gl.keyHeldr = 1;
	}
	if (!gl.keys[XK_Shift_R] && gl.keyHeldr == 1)
	{

		gl.keyHeldr = 0;
	} 
	if (gl.keys[XK_Return] && gl.keyHeldf == 0)
	{

		player2.animationState = 2;
		gl.keyHeldf = 1;
	}
	if (!gl.keys[XK_Return] && gl.keyHeldf == 1)
	{

		gl.keyHeldf = 0;
	} 

	//nudge player if colliding
	if(player.collisionState == true && (player.pos[0] < player2.pos[0]))
	{
		player.pos[0] -= 1;
	}
	else if (player.collisionState == true && (player.pos[0] > player2.pos[0]))
	{
		player.pos[0] += 1;
	}
}

void render()
{
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//backgroundRender(gl.xres,gl.yres);

	//Display player names
	const char* P1 = "Player 1";
	const char* P2 = "Player 2";
	displayName(P1, 900, 1);
	displayName(P2, 900, 2);
	const char* SC = "Scores :";
	if(PROFILING_ON !=0){
	displayScore(SC,800,1);
	displayScoreOpt(SC,800,1);
	}
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
	if (player.animationState == 1){
		// return player.animation state back to 0 after spritePunch();
		player.animationState = spritePunch(player.sp,0,3); 
	}
	if (player2.animationState == 1){
		// return player.animation state back to 0 after spritePunch(); 
		player2.animationState = spritePunch(player2.sp,8,11); 

	}

	if (player.animationState == 2){
		// return player.animation state back to 0 after spriteKick();
		player.animationState = spriteKick(player.sp);
	}
	if (player2.animationState == 2){
		// return player.animation state back to 0 after spriteKick();
		player2.animationState = spriteKick(player2.sp);
	}
	spriteRender(player.sp,player.pos[0], player.pos[1], player.pos[2]);
	spriteRender(player2.sp,player2.pos[0], player2.pos[1], player2.pos[2]);
	
	//grabing resources to check collition
	player.collisionState = checkCollision(player.pos[0],player.pos[1],player2.pos[0],player2.pos[1],
							player.sp,player2.sp);

	//Display healthbars
	drawHealthBar1(gl.xres, gl.yres);
	drawHealthBar2(gl.xres, gl.yres);
	healthBarOverlay(gl.xres, gl.yres);
	healthBarOverlay2(gl.xres, gl.yres);
    countdown(gl.xres, gl.yres);
	//render background
	//backgroundRender();
	
}






