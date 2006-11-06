#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <assert.h>
#include "FrameBufferObject.h"

#include "ShaderObject.h"
#include "ShaderProgram.h"
#include "ShaderUniformValue.h"

#include "GlowEffect.h"

GLuint originalTexture, horizBlurredTex, finalBlurredTex;

// ########### SHADERS DECLARATIONS ###########
ShaderProgram* shaderProgram;
ShaderProgram* shaderProgram2;
ShaderObject* horizontalBlurVertexShader;
ShaderObject* horizontalBlurFragmentShader;
ShaderObject* verticalBlurVertexShader;
ShaderObject* verticalBlurFragmentShader;
ShaderUniformValue<int> textureUniform;
// ########### END SHADERS DECLARATIONS ###########

// ########### FBO DECLARATIONS ###########
FrameBufferObject* fbo;
// ########### END FBO DECLARATIONS ###########

int imageWinWidth = 512;
int imageWinHeight = 512;



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
void renderSceneOnQuad(GLuint textureId, GLenum textureUnit)
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );

	glActiveTexture( textureUnit );
	glBindTexture(GL_TEXTURE_2D, textureId);

	//RENDER MULTITEXTURE ON QUAD
	glBegin(GL_QUADS);
	glMultiTexCoord2f( textureUnit, 0.0f, 0.0f );
	glVertex2f( -1.0f, -1.0f );

	glMultiTexCoord2f( textureUnit, 1.0f, 0.0f );
	glVertex2f( 1.0f, -1.0f );

	glMultiTexCoord2f( textureUnit, 1.0f, 1.0f );
	glVertex2f( 1.0f, 1.0f );

	glMultiTexCoord2f( textureUnit, 0.0f, 1.0f );
	glVertex2f( -1.0f, 1.0f );
	glEnd();

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


/**
 */
void render() 
{
	// ******* RENDER TO THE FRAMEBUFFER ********
	//bind the FBO, and the associated texture.
	fbo->bind();

	// #### FIRST STEP: Draw the object into the original texture: 'originalTexture'
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );

	glPushMatrix();
	glColor4f(1, 1, 1, 1);
	glutSolidSphere(0.2, 32, 32);
	glPopMatrix();

	// #### SECOND STEP: blur the texture horizontally 
	// and store it into the GL_COLOR_ATTACHMENT1_EXT or  'horizBlurredTex'
	// Render it to the right texture: 'horizBlurredTex'
	glDrawBuffer( GL_COLOR_ATTACHMENT1_EXT );

	// render using the horizontal blur shader and the original texture
	shaderProgram->useProgram();
	renderSceneOnQuad( originalTexture, GL_TEXTURE0);
	shaderProgram->disableProgram();

	// #### THIRD STEP: blur the texture vertically 
	// and store it into the 'finalBlurredTex'
	// Choose the right attachment
	glDrawBuffer( GL_COLOR_ATTACHMENT2_EXT );
	//glReadBuffer( GL_COLOR_ATTACHMENT1_EXT );

	//shaderProgram->detachShader( *horizontalBlurFragmentShader );
	//shaderProgram->detachShader( *horizontalBlurVertexShader );

	//shaderProgram->attachShader( *verticalBlurFragmentShader );
	//shaderProgram->attachShader( *verticalBlurVertexShader );

	//textureUniform.setValue( 0 );

	//shaderProgram->buildProgram();

	// render using the vertical blur shader and the horizontal blurred texture
	shaderProgram2->useProgram();
	renderSceneOnQuad( horizBlurredTex, GL_TEXTURE0 );
	shaderProgram2->disableProgram();

	// 'unbind' the FBO. things will now be drawn to screen as usual
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// ******* RENDER TO THE WINDOW *************

	glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0); 

	shaderProgram->useProgram();
	renderSceneOnQuad( finalBlurredTex, GL_TEXTURE0 );
	shaderProgram->disableProgram();

	//glPushMatrix();
	//glColor4f(1, 0, 0, 0.2f);
	//glutSolidSphere(0.2, 32, 32);
	//glPopMatrix();

	glutSwapBuffers();
}

/*
* Initializes the program shaders	
*/
void initShaders()
{
	horizontalBlurVertexShader = new ShaderObject(GL_VERTEX_SHADER, "./shaders/horizBlur.vert");
	horizontalBlurFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "./shaders/horizBlur.frag");

	verticalBlurVertexShader = new ShaderObject(GL_VERTEX_SHADER, "./shaders/vertBlur.vert");
	verticalBlurFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "./shaders/vertBlur.frag");

	shaderProgram = new ShaderProgram();

	shaderProgram2 = new ShaderProgram();

	shaderProgram->attachShader( *horizontalBlurVertexShader );
	shaderProgram->attachShader( *horizontalBlurFragmentShader );

	shaderProgram2->attachShader( *verticalBlurFragmentShader );
	shaderProgram2->attachShader( *verticalBlurVertexShader );

	textureUniform.setValue( 0 );
	textureUniform.setName("blurTex");

	shaderProgram->addUniformObject( &textureUniform );
	shaderProgram2->addUniformObject( &textureUniform );

	shaderProgram->buildProgram();
	shaderProgram2->buildProgram();
}

/**
 */
void init()
{
	// ########### init shaders ###############
	initShaders();

	fbo = new FrameBufferObject();

	/************************************************************************/
	/* TEXTURE PART                                                         */
	/************************************************************************/
	// make a texture
	glGenTextures(1, &originalTexture);
	glGenTextures(1, &horizBlurredTex);
	glGenTextures(1, &finalBlurredTex);

	fbo->bind();

	// initialize texture that will store the framebuffer image
	glBindTexture(GL_TEXTURE_2D, originalTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, horizBlurredTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, finalBlurredTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	fbo->attachTexture(originalTexture, GL_COLOR_ATTACHMENT0_EXT);
	fbo->attachTexture(horizBlurredTex, GL_COLOR_ATTACHMENT1_EXT);
	fbo->attachTexture(finalBlurredTex, GL_COLOR_ATTACHMENT2_EXT);

	FrameBufferObject::unbind();
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