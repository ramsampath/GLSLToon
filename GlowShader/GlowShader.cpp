#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <assert.h>
#include "FrameBufferObject.h"

#include "ShaderObject.h"
#include "ShaderProgram.h"
#include "ShaderUniformValue.h"

#include "GlowEffect.h"

// ################### FPS INFO ######################
double t0 = 0.0;
int frames = 0;
char titlestring[200];

// ###################################################

int imageWinWidth = 512;
int imageWinHeight = 512;

float angle = 0;

// the last parameter, 0,  makes it become a directional light
GLfloat lightDirection[] = {0.0f, 0.2f, 1.0f, 0.0f};

GLfloat diffuse[] = {0.7f, 0.0f, 0.0f, 1.0};
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0};

GlowEffect glow;


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
	gluPerspective(45, (double)w/ (double)h, 1, 100);
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, -100.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glutPostRedisplay();
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

void drawGlowSources()
{
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(1, 1);
	glCullFace(GL_FRONT);
	glLineWidth( 3.0 );
	glColor3f(0.7f, 0.7f, 1.0f);

	glPushMatrix();
		glTranslatef(0, 0, -0.8);
		glRotatef(angle, 1, 1, 0);
		glutSolidTorus(0.12f, 0.22f, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.5, -0.5, -0.8);
		glutSolidSphere(0.22, 32, 32);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glDisable(GL_POLYGON_OFFSET_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glCullFace(GL_BACK);
	glLineWidth( 1.0 );
}

void drawScenario()
{
	//drawGlowSources();

	GLfloat diffuse1[4] = {0.8f, 0, 0, 1.0};
	GLfloat specular1[4] = {1, 1, 1, 1.0};
	GLfloat shininess1 = 128;

	// materials
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess1);

	glPushMatrix();
		glTranslatef(0, 0, -1);
		glRotatef(angle, 1, 1, 0);
		glutSolidTorus(0.1f, 0.2f, 32, 32);
	glPopMatrix();

	GLfloat diffuse2[4] = {0.4f, 0.4f, 1.0, 1.0};
	GLfloat specular2[4] = {1, 1, 1, 1.0};
	GLfloat shininess2 = 128;

	// materials
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular2);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess2);

	glPushMatrix();
	glTranslatef(-0.5, -0.5, -1);
	glutSolidSphere(0.2, 32, 32);
	glPopMatrix();
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

void drawTMP()
{
	glColor3f(1,1,1);

	glPushMatrix();
	glTranslatef(0.5, 0, -1);
	glutSolidSphere(0.14, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1, 0, -1);
	glutSolidSphere(0.1, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 0, -1);
	glutSolidSphere(0.07, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, 0, -1);
	glutSolidSphere(0.03, 32, 32);
	glPopMatrix();

	glPointSize(64.0);
	glBegin(GL_POINTS);
	glVertex3f(0.2, 0.0, -1);
	glEnd();

	glLineWidth(5.0);
	glBegin(GL_LINES);
	glVertex3f(-0.2, 0.5, -1);
	glVertex3f(0.5, 0.5, -1);
	glEnd();

	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex3f(-0.2, 0.4, -1);
	glVertex3f(0.5, 0.4, -1);
	glEnd();
}

/**
 */
void render() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glow.begin();
		glDisable(GL_LIGHTING);
		
		//drawTMP();
		
		//drawScenario();
		drawGlowSources();

	glow.end();

	showFPS();

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	// update the angle: TMP
	angle += 0.2f;

	if ( angle > 360.0f)
	{
		angle -= 360.0f;
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

	glow.init();
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
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}