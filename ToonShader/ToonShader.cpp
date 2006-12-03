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
float rotationAngle;

// ############# SHADER STUFF ##################
ShaderProgram* toonShaderProgram;
ShaderObject* toonVertexShader;
ShaderObject* toonFragmentShader;

ShaderProgram* silhouetteShaderProgram;
ShaderObject* silhouetteVertexShader;
ShaderObject* silhouetteFragmentShader;
// #############################################

// Silhouette width
float silhouetteWidth = 3.0f;

bool pause = false;

GLuint currentTexIndex;
GLuint textureIds[2];
ShaderAttributeValue<int> texAttrib;

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef struct tagVECTOR										// A Structure To Hold A Single Vector ( NEW )
{
	float X, Y, Z;												// The Components Of The Vector ( NEW )
}
VECTOR;

typedef struct tagVERTEX										// A Structure To Hold A Single Vertex ( NEW )
{
	VECTOR Nor;													// Vertex Normal ( NEW )
	VECTOR Pos;													// Vertex Position ( NEW )
}
VERTEX;

typedef struct tagPOLYGON										// A Structure To Hold A Single Polygon ( NEW )
{
	VERTEX Verts[3];											// Array Of 3 VERTEX Structures ( NEW )
}
POLYGON;

POLYGON		*polyData		= NULL;
int			polyNum			= 0;

BOOL ReadMesh();
void showFPS();
/************************************************************************/
/*                                                                      */
/************************************************************************/

/*
 * Initializes the program shaders	
 */
void initShaders()
{
	toonVertexShader	= new ShaderObject(GL_VERTEX_SHADER, "vertex_shader.vert");
	toonFragmentShader	= new ShaderObject(GL_FRAGMENT_SHADER, "fragment_shader.frag");

	silhouetteVertexShader		= new ShaderObject(GL_VERTEX_SHADER, "silhouette.vert");
	silhouetteFragmentShader	= new ShaderObject(GL_FRAGMENT_SHADER, "silhouette.frag");

	toonShaderProgram		= new ShaderProgram();
	silhouetteShaderProgram = new ShaderProgram();

	texAttrib.setValue("toonTexture", 0);

	// attach the texture as an uniform variable
	toonShaderProgram->addUniformObject( &texAttrib );

	toonShaderProgram->attachShader( *toonVertexShader );
	toonShaderProgram->attachShader( *toonFragmentShader );

	silhouetteShaderProgram->attachShader( *silhouetteFragmentShader );
	silhouetteShaderProgram->attachShader( *silhouetteVertexShader );

	toonShaderProgram->buildProgram();
	silhouetteShaderProgram->buildProgram();
}

/*
 *	
 */
void init()
{
	glEnable(GL_CULL_FACE); // Cull away all back facing polygons
	glEnable(GL_DEPTH_TEST); // Use the Z buffer

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	// initialize Devil library
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	// loads an image file directly into an OpenGL texture
	// NOTE: It also ENABLES GL_TEXTURE_2D; and overrides the TexParameter settings
	textureIds[0] = ilutGLLoadImage("gradient1.tga");
	textureIds[1] = ilutGLLoadImage("gradient2.tga");

	currentTexIndex = 0;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// init shading part: after the window has been created
	// to load correctly the extensions
	initShaders();

	rotationAngle = 0;

	ReadMesh();
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
/* READS A 3D MODEL: from NeHe Lesson 37                               */
/************************************************************************/
void drawModel()
{
	float normal[3];
	glBegin (GL_TRIANGLES);										// Tell OpenGL That We're Drawing Triangles

	for (int i = 0; i < polyNum; i++)							// Loop Through Each Polygon ( NEW )
	{
		for (int j = 0; j < 3; j++)								// Loop Through Each Vertex ( NEW )
		{
			normal[0] = polyData[i].Verts[j].Nor.X;		// Fill Up The TmpNormal Structure With
			normal[1] = polyData[i].Verts[j].Nor.Y;		// The Current Vertices' Normal Values ( NEW )
			normal[2] = polyData[i].Verts[j].Nor.Z;

			glNormal3fv(normal);

			//glTexCoord1f (TmpShade);						// Set The Texture Co-ordinate As The Shade Value ( NEW )
			glVertex3fv (&polyData[i].Verts[j].Pos.X);		// Send The Vertex Position ( NEW )
		}
	}

	glEnd ();	
}

BOOL ReadMesh ()												// Reads The Contents Of The "model.txt" File ( NEW )
{
	FILE *In = fopen ("model.txt", "rb");					// Open The File ( NEW )

	if (!In)
		return FALSE;											// Return FALSE If File Not Opened ( NEW )

	fread (&polyNum, sizeof (int), 1, In);						// Read The Header (i.e. Number Of Polygons) ( NEW )

	polyData = new POLYGON [polyNum];							// Allocate The Memory ( NEW )

	fread (&polyData[0], sizeof (POLYGON) * polyNum, 1, In);	// Read In All Polygon Data ( NEW )

	fclose (In);												// Close The File ( NEW )

	return TRUE;												// It Worked ( NEW )
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
/*
* drawScene(float t) - the actual drawing commands to render our scene.
*/
void drawScene() {
	glLightfv(GL_LIGHT0, GL_POSITION, lightDirection); // Set light position

	// Enable lighting and the LIGHT0 we placed before
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindTexture(GL_TEXTURE_2D, textureIds[ currentTexIndex ]);

	glPushMatrix(); // Transforms to animate the object:
		
		glRotatef(rotationAngle, 0, 1, 0);
		drawModel();

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
		sprintf_s(titlestring, "Toon Shader (%.1f FPS)", fps);
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

	toonShaderProgram->useProgram();
	{
		// Render with the shaders active.
		drawScene();
	}
	// Deactivate the toon shader.
	toonShaderProgram->disableProgram();

	// #### Draw the silhouette ####
	silhouetteShaderProgram->useProgram();
	{
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_BACK, GL_LINE);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(5, 1);
		glCullFace(GL_FRONT);
		glLineWidth( silhouetteWidth );

		// Render with the shaders active.
		drawScene();

		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		glCullFace(GL_BACK);
		glLineWidth( 1.0 );
	}
	// Deactivate the shaders.
	silhouetteShaderProgram->disableProgram();

	glutSwapBuffers();
}

/*
 *	Idle function
 */
void anim()
{
	// update the rotation angle
	if ( !pause )
	{
		rotationAngle += 1.0f;
		
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
	if ( (key == 't') || (key == 'T') )
	{
		currentTexIndex = (currentTexIndex + 1) % 2;
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
	//glutIdleFunc(anim); 
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	free( polyData );

	return 0; 
}

