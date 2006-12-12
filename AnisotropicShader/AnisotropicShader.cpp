#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "ShaderAttributeValue.h"
#include "ShaderObject.h"
#include "ShaderProgram.h"

#include "CMesh.h"
#include "3ds.h"

/************************************************************************/
/* PROGRAM GLOBAL VARIABLES                                             */
/************************************************************************/
double t0 = 0.0;
int frames = 0;
char titlestring[200];

GLint lightDirectionLoc;

// the last parameter, 0,  makes it become a directional light
GLfloat lightDirection[] = {0.0f, 0.5f, 1.0f, 0.0f};

// rotation angle
float rotationAngle;

ShaderProgram* toonShaderProgram;
ShaderObject* toonVertexShader;
ShaderObject* toonFragmentShader;

/* #### MATERIALS #### */
GLfloat torusDiffuse[3][4] = {{0.0f, 0, 0.4f, 1.0}, {0.3f, 0.4f, 0.3f, 1.0}, {0.6f, 0, 0, 1.0}};
GLfloat torusSpecular[3][4] = {{1, 1, 1, 1.0}, {1, 1, 1, 1.0}, {1, 1, 1, 1.0}};
GLfloat torusShininess[3] = {32, 128, 64};

/* #### CONTROL VARIABLES #### */
bool play = true;

// ############# TRACKBALL ATTRIBUTES ##################
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat xRotOld = 0;
GLfloat yRotOld = 0;
int mouseState = 0;
int xCenter = 0;
int yCenter = 0;

#define M_ROTATE_XY     1

CMesh* model = NULL;

void showFPS();

/*
* Initializes the program shaders	
*/
void initShaders()
{
	toonVertexShader = new ShaderObject(GL_VERTEX_SHADER, "./shaders/phong.vert");
	toonFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "./shaders/phong.frag");

	toonShaderProgram = new ShaderProgram();

	toonShaderProgram->attachShader( *toonVertexShader );
	toonShaderProgram->attachShader( *toonFragmentShader );

	toonShaderProgram->buildProgram();
}

/*
*	
*/
void init()
{
	glEnable(GL_CULL_FACE); // Cull away all back facing polygons
	glEnable(GL_DEPTH_TEST); // Use the Z buffer
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightDirection); // Set light position

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// init shading part: after the window has been created
	// to load correctly the extensions
	initShaders();

	Load3ds loader;
	model = loader.Create("./data/torus.3ds");

	rotationAngle = 0;
}

/*
*	Reshape function
*/
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0f, (GLfloat)w/(GLfloat)h, 1.0f, 100.0f );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( 0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
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

/*
* drawScene(float t) - the actual drawing commands to render our scene.
*/
void drawScene() {
	glPushMatrix();
	{
		/* "World" rotation, controlled by mouse */
		glRotatef(xRot, 1, 0, 0);
		glRotatef(yRot, 0, 1, 0);

		/* ######## Torus 1 ######### */
		glMaterialfv(GL_FRONT, GL_DIFFUSE, torusDiffuse[0]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, torusSpecular[0]);
		glMaterialf(GL_FRONT, GL_SHININESS, torusShininess[0]);

		glPushMatrix();
		{
			glRotatef(45, 0, 1, 0);
			glRotatef(rotationAngle, 1, 0, 0);
			glutSolidTorus(0.25f, 2.0f, 32, 32);
		}
		glPopMatrix();

		/* ######## Torus 2 ######### */
		glMaterialfv(GL_FRONT, GL_DIFFUSE, torusDiffuse[1]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, torusSpecular[1]);
		glMaterialf(GL_FRONT, GL_SHININESS, torusShininess[1]);

		glPushMatrix();
		{
			glRotatef(-45, 0, 1, 0);
			glRotatef(rotationAngle, 1, 1, 0);
			glutSolidTorus(0.2f, 1.6f, 32, 32);
		}
		glPopMatrix();

		/* ######## Torus 3 ######### */
		glMaterialfv(GL_FRONT, GL_DIFFUSE, torusDiffuse[2]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, torusSpecular[2]);
		glMaterialf(GL_FRONT, GL_SHININESS, torusShininess[2]);

		glPushMatrix();
		{
			glRotatef(35, 0, 1, 0);
			model->draw();
		}
		glPopMatrix();
	}
	glPopMatrix();
	
	showFPS();
}

/*
* showFPS() - Calculate and report frames per second
* (updated once per second) in the window title bar
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
		sprintf_s(titlestring, "Anisotropic Shader (%.1f FPS)", fps);
		glutSetWindowTitle(titlestring);
	}
	frames ++;
}

/*
*	Display Function
*/
void display()
{
	// Clear the color buffer and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	toonShaderProgram->useProgram();

	// Render with the shaders active.
	drawScene();

	// Deactivate the shaders.
	toonShaderProgram->disableProgram();

	glutSwapBuffers();
}


/*
*	Idle function
*/
void anim(int millis)
{
	
	// update the rotation angle
	if ( play )
	{
		rotationAngle += 1.5f;
	}

	if (rotationAngle > 360.0f)
	{
		rotationAngle -= 360.0f;
	}
	glutPostRedisplay();
	glutTimerFunc(millis, anim, millis);
}

void keyboard(unsigned char key, int x, int y)
{
	if ( (key == 'p') || (key == 'P') )
	{
		play = ! play;
	}
}

/*
*	
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (640, 640);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(20, anim, 20);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0; 
}


