//Author:Michael Guzman
//Modified on : 05/09/2018
#include "header.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
//#include <GLUT/glut.h>
//#include <OpenAL/alut.h>

//  function prototypes ---------------------------------------------
void init();
void display();
void reshape(int w, int h); 
void keyboard(unsigned char key, int x, int y); 
void specialKeys(int key, int x, int y); 

void displayScore(const char* s, int xres, int i)
{
	clock_t start = clock();
	Rect r;
	r.bot = xres -50;

	switch(i){
		case 1:
			r.left =400;
			break;
		case 2:
			r.left =800;
			break;
	}

	ggprint16(&r,32, 0x00ffffff, s);

	static float end = 0;
	end += clock() - start;
	r.bot = 400;
	r.left = xres/2;
	r.center = 1;
	char c[50];
	sprintf(c, "Timer:  %f", end/CLOCKS_PER_SEC);
	ggprint16(&r, 16, 0x00ffffff, c);
	glPopMatrix();

	r.bot = 450;
	r.left = xres/2;
	r.center = 1;
	ggprint16(&r, 16, 0x00ffffff, "Michael Guzman");
	glPopMatrix();


}

void displayScoreOpt(const char* s, int xres, int i)
{
	clock_t start = clock();
	Rect r;
	r.bot = xres -50;

	switch(i){
		case 1:
			r.left =400;
			break;									
		case 2:
			r.left =800;
			break;
	}


	//ggprint16(&r,32, 0x00ffffff, s);

	static float end = 0;
	end += clock() - start;
	r.bot = 350;
	r.left = xres*0.5;
	r.center = 1;
	char c[50];
	sprintf(c, "TimerOpt:  %f", end/CLOCKS_PER_SEC);
	ggprint16(&r, 16, 0x00ffffff, c);
	glPopMatrix();


}


/*
// highly evil global variables ---------------------------------------------
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1

ALfloat listenerPos[]={0.0,0.0,4.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat	listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};

ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};


ALfloat source1Pos[]={ 2.0, 0.0, 0.0};
ALfloat source1Vel[]={ 0.0, 0.0, 0.0};

ALfloat source2Pos[]={ 0.0, 0.0, -4.0};
ALfloat source2Vel[]={ 0.0, 0.0, 0.0};

ALuint	buffer[NUM_BUFFERS];
ALuint	source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];
int 	GLwin;

ALsizei size,freq;
ALenum 	format;
ALvoid 	*data;
int 	ch;

// ===================================================================
// void main(int argc, char** argv)
// ===================================================================
int main(int argc, char** argv) //finaly the main function
{
	//initialise glut
	glutInit(&argc, argv) ;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH) ;   
	glutInitWindowSize(400,400) ;

	//initialise openAL
	alutInit(&argc, argv) ; 

	GLwin = glutCreateWindow("PIGE - OpenAL Example") ;
	init() ;
	glutDisplayFunc(display) ;
	glutKeyboardFunc(keyboard) ;
	glutSpecialFunc(specialKeys);
	glutReshapeFunc(reshape) ;

	glutMainLoop() ;

	return 0;
}

// ===================================================================
// void init()
// ===================================================================
void init(void)
{
	alutInit(0, NULL);

	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);

	alGetError(); // clear any error messages

	if(alGetError() != AL_NO_ERROR) 
	{
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else
	{
		printf("init() - No errors yet.");

	}

	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);

	alutLoadWAVFile("c.wav",&format,&data,&size,&freq);
	alBufferData(buffer[0],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile("b.wav",&format,&data,&size,&freq);
	alBufferData(buffer[1],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile("a.wav",&format,&data,&size,&freq);
	alBufferData(buffer[2],format,data,size,freq);
	alutUnloadWAV(format,data,size,freq);
*/
//	alGetError(); /** clear error **/
/*	alGenSources(NUM_SOURCES, source);

	if(alGetError() != AL_NO_ERROR) 
	{
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else
	{
		printf("init - no errors after alGenSources\n");
	}

	alSourcef(source[0],AL_PITCH,1.0f);
	alSourcef(source[0],AL_GAIN,1.0f);
	alSourcefv(source[0],AL_POSITION,source0Pos);
	alSourcefv(source[0],AL_VELOCITY,source0Vel);
	alSourcei(source[0],AL_BUFFER,buffer[0]);
	alSourcei(source[0],AL_LOOPING,AL_TRUE);

	alSourcef(source[1],AL_PITCH,1.0f);
	alSourcef(source[1],AL_GAIN,1.0f);
	alSourcefv(source[1],AL_POSITION,source1Pos);
	alSourcefv(source[1],AL_VELOCITY,source1Vel);
	alSourcei(source[1],AL_BUFFER,buffer[1]);
	alSourcei(source[1],AL_LOOPING,AL_TRUE);

	alSourcef(source[2],AL_PITCH,1.0f);
	alSourcef(source[2],AL_GAIN,1.0f);
	alSourcefv(source[2],AL_POSITION,source2Pos);
	alSourcefv(source[2],AL_VELOCITY,source2Vel);
	alSourcei(source[2],AL_BUFFER,buffer[2]);
	alSourcei(source[2],AL_LOOPING,AL_TRUE);
}


// ===================================================================
// void display()
// ===================================================================
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
	glPushMatrix() ;
	glRotatef(20.0,1.0,1.0,0.0) ;

	glPushMatrix() ;
	glTranslatef(source0Pos[0],source0Pos[1],source0Pos[2]) ;
	glColor3f(1.0,0.0,0.0) ;
	glutWireCube(0.5) ;
	glPopMatrix() ;

	glPushMatrix() ;
	glTranslatef(source2Pos[0],source2Pos[1],source2Pos[2]) ;
	glColor3f(0.0,0.0,1.0) ;
	glutWireCube(0.5) ;
	glPopMatrix() ;

	glPushMatrix() ;
	glTranslatef(source1Pos[0],source0Pos[1],source0Pos[2]) ;
	glColor3f(0.0,1.0,0.0) ;
	glutWireCube(0.5) ;
	glPopMatrix() ;

	//the listener
	glPushMatrix() ;
	glTranslatef(listenerPos[0],listenerPos[1],listenerPos[2]) ;
	glColor3f(1.0,1.0,1.0) ;
	glutWireCube(0.5) ;
	glPopMatrix() ;

	glPopMatrix() ;
	glutSwapBuffers() ;
}

// ===================================================================
// void reshape(int w, int h)
// ===================================================================
void reshape(int w, int h) // the reshape function
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h) ;
	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	gluPerspective(60.0,(GLfloat)w/(GLfloat)h,1.0,30.0) ;
	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;
	glTranslatef(0.0,0.0,-6.6) ;
}


// ===================================================================
// void keyboard(int key, int x, int y)
// ===================================================================
void keyboard(unsigned char key, int x, int y) 
{
	switch(key)
	{
		case '1':
			alSourcePlay(source[0]);
			printf("1\n");
			break;
		case '2':
			alSourcePlay(source[1]);
			printf("2\n");
			break;
		case '3':
			alSourcePlay(source[2]);
			printf("3\n");
			break;
		case '4':
			alSourceStop(source[0]);
			printf("4\n");
			break;
		case '5':
			alSourceStop(source[1]);
			printf("5\n");
			break;
		case '6':
			alSourceStop(source[2]);
			printf("6\n");
			break;
		case 'a':
		case 'A':
			listenerPos[0] -= 0.1 ;
			alListenerfv(AL_POSITION,listenerPos);

			break ;
		case 's':
		case 'S':
			listenerPos[0] += 0.1 ;
			alListenerfv(AL_POSITION,listenerPos);

			break ;
		case 'q':
		case 'Q':
			listenerPos[2] -= 0.1 ;
			alListenerfv(AL_POSITION,listenerPos);

			break ;
		case 'z':
		case 'Z':
			listenerPos[2] += 0.1 ;
			alListenerfv(AL_POSITION,listenerPos);

			break ;
		case 27:
			alSourceStop(source[2]);
			alSourceStop(source[1]);
			alSourceStop(source[0]);

			alutExit();
			glutDestroyWindow(GLwin) ;
			exit(0) ;
			break ;
		default: break;
	}
	glutPostRedisplay() ;
}

// ===================================================================
// void specialKeys(int key, int x, int y)
// =================================================================== 
void specialKeys(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_RIGHT:	listenerPos[0] += 0.1 ;
					alListenerfv(AL_POSITION,listenerPos);
					glutPostRedisplay() ;
					break;
		case GLUT_KEY_LEFT: 	listenerPos[0] -= 0.1 ;
					alListenerfv(AL_POSITION,listenerPos);
					glutPostRedisplay() ;
					break;
		case GLUT_KEY_UP: 	listenerPos[2] -= 0.1 ;
					alListenerfv(AL_POSITION,listenerPos);
					glutPostRedisplay() ;
					break;
		case GLUT_KEY_DOWN: 	listenerPos[2] += 0.1 ;
					alListenerfv(AL_POSITION,listenerPos);
					glutPostRedisplay() ;
					break;
	}
}*/
