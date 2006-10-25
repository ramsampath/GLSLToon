#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
//#include <IL/il.h>
//#include <IL/ilu.h>
//#include <IL/ilut.h>

#include "ShaderAttributeValue.h"
#include "ShaderObject.h"
#include "ShaderProgram.h"

/************************************************************************/
/* PROGRAM GLOBAL VARIABLES                                             */
/************************************************************************/
double t0 = 0.0;
int frames = 0;
char titlestring[200];

GLint lightDirectionLoc;

// the last parameter, 0,  makes it become a directional light
GLfloat lightDirection[] = {0.0f, 0.0f, 1.0f, 0.0f};

// rotation angle
float angle;

ShaderProgram* shaderProgram;
ShaderObject* vertexShader;
ShaderObject* fragmentShader;

//GLuint textureID;
//ShaderAttributeValue<int> texAttrib;

void showFPS();

/*
* Initializes the program shaders	
*/
void initShaders()
{
	vertexShader = new ShaderObject(GL_VERTEX_SHADER, "./shaders/phong.vert");
	fragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "./shaders/phong.frag");

	shaderProgram = new ShaderProgram();

	shaderProgram->attachShader( *vertexShader );
	shaderProgram->attachShader( *fragmentShader );

	shaderProgram->buildProgram();
}

/*
*	
*/
void init()
{
	glEnable(GL_CULL_FACE); // Cull away all back facing polygons
	glEnable(GL_DEPTH_TEST); // Use the Z buffer

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// init shading part: after the window has been created
	// to load correctly the extensions
	initShaders();

	angle = 0;
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
	gluLookAt( 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
}

/*
* drawScene(float t) - the actual drawing commands to render our scene.
*/
void drawScene() {
	glLightfv(GL_LIGHT0, GL_POSITION, lightDirection); // Set light position
	GLfloat diffuse[4] = {0.8f, 0, 0, 1.0};
	GLfloat specular[4] = {1, 1, 1, 1.0};
	GLfloat light_pos[4] = {0, 0, 1, 0};
	GLfloat shininess = 128;

	// since we are scaling (in our particular case, uniform scaling) 
	// the object, the normals need to be rescaled
	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);


	// Enable lighting and the LIGHT0 we placed before
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPushMatrix(); // Transforms to animate the object:

	//glutSolidSphere(0.3f, 32, 32);
	glRotatef(angle, -1, 0, 0);
	glutSolidTorus(0.1f, 0.3f, 32, 32);

	glPopMatrix(); // Revert to initial transform

	// Disable lighting again, to prepare for next frame.
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

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
	static float t;
	t = (float)glutGet(GLUT_ELAPSED_TIME); // Get elapsed time

	// Clear the color buffer and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram->useProgram();

	// Render with the shaders active.
	drawScene();

	// Deactivate the shaders.
	shaderProgram->disableProgram();

	glutSwapBuffers();
}

/*
*	Idle function
*/
void anim()
{
	// update the rotation angle
	angle += 0.05f;

	if (angle > 360.0f)
	{
		angle -= 360.0f;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	if ( (key == 'r') || (key == 'R') )
	{

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
	glutIdleFunc(anim); 
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0; 
}


