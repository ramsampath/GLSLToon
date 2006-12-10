#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "glext.h"
#include "vgl.h" 

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
GLfloat lightDirection[] = {0.0f, 0.6f, 1.0f, 0.0f};

// rotation angle
float rotationAngle;

// ############# SHADER STUFF ##################
ShaderProgram* toonShaderProgram;
ShaderObject* toonVertexShader;
ShaderObject* toonFragmentShader;

ShaderAttributeValue<int> texAttrib;
// #############################################

// Silhouette width
float silhouetteWidth = 7.0f;

bool pause = false;

const int textureCount = 3;
GLuint currentTexIndex;
GLuint textureIds[textureCount];
char* filenames[textureCount] = {"gradient1.tga", "gradient2.tga", "gradient3.tga"};

// ############# 3D MODELS ##################
CMesh* models[3];
const int modelCount = 3;
GLuint currentModelIndex = 0;

// ############# TRACKBALL ATTRIBUTES ##################
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat xRotOld = 0;
GLfloat yRotOld = 0;
int mouseState = 0;
int xCenter = 0;
int yCenter = 0;

#define M_ROTATE_XY     1

// ############# METHODS HEADERS ##################
void showFPS();

/*
 * Initializes the program shaders	
 */
void initShaders()
{
	toonVertexShader	= new ShaderObject(GL_VERTEX_SHADER, "vertex_shader.vert");
	toonFragmentShader	= new ShaderObject(GL_FRAGMENT_SHADER, "fragment_shader.frag");

	toonShaderProgram		= new ShaderProgram();

	texAttrib.setValue("toonTexture", 0);

	// attach the texture as an uniform variable
	toonShaderProgram->addUniformObject( &texAttrib );

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

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lightDirection); // Set light position

	// initialize Devil library
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	// loads an image file directly into an OpenGL texture
	// NOTE: It also ENABLES GL_TEXTURE_2D; and overrides the TexParameter settings
	for (int i = 0; i < textureCount; i++)
	{
		textureIds[ i ] = ilutGLLoadImage( filenames[ i ] );
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	
	// current texture index
	currentTexIndex = 0;

	// init shading part: after the window has been created
	// to load correctly the extensions
	initShaders();

	rotationAngle = 0;

	Load3ds loader3DS;

	models[0] = loader3DS.Create("./data/Tie-Fighter.3ds");
	models[1] = loader3DS.Create("./data/Atronach.3ds");
	models[2] = loader3DS.Create("./data/Mephisto.3ds");
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
/*
* drawScene(float t) - the actual drawing commands to render our scene.
*/
void drawScene() {

	glPushMatrix(); // Transforms to animate the object:
		
		glRotatef(rotationAngle, 0, 1, 0);
		models[ currentModelIndex ]->draw();

	glPopMatrix(); // Revert to initial transform

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
		sprintf_s(titlestring, "Toon Shader (%.1f FPS)", fps);
		glutSetWindowTitle(titlestring);
	}
	frames ++;
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
 * @description shade the scene using toon shade style
 */
void toonShade()
{
	// Enable lighting and the LIGHT0 we placed before
	glEnable(GL_LIGHTING | GL_LIGHT0 | GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIds[ currentTexIndex ]);

	toonShaderProgram->useProgram();
	{
		// Render with the shaders active.
		drawScene();
	}
	// Deactivate the toon shader.
	toonShaderProgram->disableProgram();

	// Disable lighting again, to prepare for next frame.
	glDisable(GL_LIGHTING | GL_LIGHT0 | GL_TEXTURE_2D);
}

void silhouetteShade()
{
	glColor3f(0, 0, 0);
	glPolygonMode(GL_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(5, 1);
	glCullFace(GL_FRONT);
	glLineWidth( silhouetteWidth );

	// Render with the shaders active.
	drawScene();

	glDisable(GL_POLYGON_OFFSET_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glCullFace(GL_BACK);
	glLineWidth( 1.0 );
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

	glPushMatrix();
	{
		/* "World" rotation, controlled by mouse */
		glRotatef(xRot, 1, 0, 0);
		glRotatef(yRot, 0, 1, 0);

		// #### shade using toon style ####
		toonShade();

		// #### Draw the silhouette ####
		silhouetteShade();
	}
	glPopMatrix();

	glutSwapBuffers();
}

/*
 *	Animation function
 */
void anim()
{
	// update the rotation angle
	if ( !pause )
	{
		rotationAngle += 0.5f;
		
		if (rotationAngle > 360.0f)
		{
			rotationAngle -= 360.0f;
		}
	}
}

void timerFunc(int value)
{
	glutPostRedisplay();
	anim();
	glutTimerFunc(value, timerFunc, value);
}

void keyboard(unsigned char key, int x, int y)
{
	if ( (key == 'm') || (key == 'M') )
	{
		currentModelIndex = (currentModelIndex + 1) % modelCount;
		rotationAngle = 0;
	}

	if ( (key == 't') || (key == 'T') )
	{
		currentTexIndex = (currentTexIndex + 1) % textureCount;
	}

	if ( (key == 'p') || (key == 'P') )
	{
		pause = ! pause;
	}

	// increase silhouette
	if(key == '+')
	{
		silhouetteWidth += 1.0;
	}

	// decrease silhouette
	if(key == '-')
	{
		silhouetteWidth -= 1.0;
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
	glutTimerFunc(20, timerFunc, 20);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	//delete[] models;

	return 0; 
}

