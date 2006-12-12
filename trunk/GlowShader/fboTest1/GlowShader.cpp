#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <assert.h>
#include "FrameBufferObject.h"

#include "ShaderObject.h"
#include "ShaderProgram.h"
#include "ShaderUniformValue.h"

#include "GlowEffect.h"
#include "CMesh.h"
#include "3ds.h"

// ################### FPS INFO ######################
double t0 = 0.0;
int frames = 0;
char titlestring[200];

// ###################################################

int imageWinWidth = 512;
int imageWinHeight = 512;

float rotationAngle = 0;

// the last parameter, 0,  makes it become a directional light
GLfloat lightDirection[] = {0.0f, 0.8f, 1.0f, 0.0f};

GLfloat diffuse[4] = {0.5, 0.5, 1.0, 1};

// ############# TRACKBALL ATTRIBUTES ##################
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat xRotOld = 0;
GLfloat yRotOld = 0;
int mouseState = 0;
int xCenter = 0;
int yCenter = 0;

#define M_ROTATE_XY     1

GlowEffect glow;
CMesh* model;

/**
 * @param w
 * @param h
 */
void reshape(int w, int h)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glFrustum(0.0, 1.0, 1.0, 0.0, 1.0, 100.0);
	gluPerspective(45, (double)w/ (double)h, 1, 50);
	gluLookAt(0.0, 0.0, 1.5, 0.0, 0.0, -100.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	xCenter = x;
	yCenter = y;

	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			mouseState = M_ROTATE_XY;
			xRotOld = xRot;
			yRotOld = yRot;
		}
	} else {
		mouseState = 0;
	}
}

void motion(int x, int y) {
	if (mouseState == M_ROTATE_XY) {
		xRot = xRotOld + (float)(y - yCenter) / 4.0;
		yRot = yRotOld + (float)(x - xCenter) / 4.0;
	}
}


/**
 */
void myIdle(){
	glutPostRedisplay();
}

/**
 * @param key
 * @param x
 * @param y
 */
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	  case 27:
		  exit(0);
		  break;
	  default:
		  break;
	}
}


/**
 * @description draws the scenarion on the right place
 */
void drawScenario()
{
	glPushMatrix();
	{
		glRotatef(rotationAngle, 0, 1, 0);
		model->draw();
	}
	glPopMatrix();
}


/**
 * @description draws the model using a silhouette technique; though,
 * it's not so efficient because the model needs to be drawn twice. It just
 * serves to demonstrate the potentiality of the glow.
 */
void drawGlowSources()
{
	// First, draw the model in wireframe
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(10, 1);
	glLineWidth( 6.0 );
	glColor3f(1, 1, 1);

	drawScenario();

	// Second, draw the model in black
	// to cover the back faces
	glDisable(GL_POLYGON_OFFSET_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glLineWidth( 1.0 );

	glColor3f(0, 0, 0);

	drawScenario();

	glEnable(GL_LIGHTING);
}

/**
 * @descrption draws the original model
 */
void drawOriginal()
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	drawScenario();
}

/**
 */
void showFPS() 
{
	double t;

	// Get current time
	t = glutGet(GLUT_ELAPSED_TIME);
	if (t - t0 > 1000) {
		float fps = frames * 1000.0 /(t-t0);
		t0 = t;		
		frames = 0;
		sprintf_s(titlestring, "Glow Shader (%.1f FPS)", fps);
		glutSetWindowTitle(titlestring);
	}
	frames ++;
}



/**
 */
void render() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	{
		/* "World" rotation, controlled by mouse */
		glRotatef(xRot, 1, 0, 0);
		glRotatef(yRot, 0, 1, 0);

		// Glow Source Pass
		glow.beginGlowSource();
		{
			drawGlowSources();
		}
		glow.endGlowSource();

		// Original Sources Pass
		glow.beginOriginal();
		{
			drawOriginal();
		}
		glow.finishOriginal();

	}
	glPopMatrix();

	showFPS();

	// update the angle: TMP
	rotationAngle += 0.2f;

	if ( rotationAngle > 360.0f)
	{
		rotationAngle -= 360.0f;
	}
	
	glutSwapBuffers();
}

/**
 */
void init()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// lights
	glLightfv(GL_LIGHT0, GL_POSITION, lightDirection); // Set light position

	// init glow fx
	glow.init();

	// init 3d models
	Load3ds loader;
	model = loader.Create("../GlowShader/data/Atronach1.3ds");
}

int main(int argc, char *argv[] )  {

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(imageWinWidth, imageWinHeight);
	glutCreateWindow(argv[0]);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	init();

	glutDisplayFunc(render);
	glutIdleFunc(myIdle);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}