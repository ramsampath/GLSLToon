#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <assert.h>
#include "FrameBufferObject.h"

#include "ShaderObject.h"
#include "ShaderProgram.h"
#include "ShaderUniformValue.h"

#include "GlowEffect.h"

int imageWinWidth = 512;
int imageWinHeight = 512;

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

/**
 */
void render() 
{

	glow.begin();

		glPushMatrix();
		glColor4f(1, 1, 1, 1);
		glutSolidSphere(0.2, 32, 32);
		glPopMatrix();

	glow.end();

	glutSwapBuffers();
}

/**
 */
void init()
{
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