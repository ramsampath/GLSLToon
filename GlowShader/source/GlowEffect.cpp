#include <GL/glew.h>
#include "FrameBufferObject.h"

#include "ShaderObject.h"
#include "ShaderProgram.h"
#include "ShaderUniformValue.h"
#include "GlowEffect.h"

GlowEffect::GlowEffect(unsigned int width, unsigned int height)
: imageWinWidth( width )
, imageWinHeight( height )
{
}

GlowEffect::~GlowEffect(void)
{
	delete horizontalShaderProgram;
	delete verticalShaderProgram;
	delete brightPassShaderProgram;
	delete blenderShaderProgram;

	delete blenderVertexShader;
	delete blenderFragmentShader;

	delete brightPassVertexShader;
	delete brightPassFragmentShader;

	delete horizontalBlurVertexShader;
	delete horizontalBlurFragmentShader;

	delete verticalBlurVertexShader;
	delete verticalBlurFragmentShader;
	delete fbo; 
}

void GlowEffect::begin()
{
	// ******* RENDER TO THE FRAMEBUFFER ********
	//bind the FBO, and the associated texture.
	fbo->bind();

	glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );

	// #### FIRST STEP: Draw the object into the original texture: 'originalTexture'
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/************************************************************************/
	/* ######################### DRAW AFTER THIS !!!                        */
	/************************************************************************/
}

void GlowEffect::end()
{
	/************************************************************************/
	/* ######################### DRAW WAS DONE BEFORE THIS !!!              */
	/************************************************************************/

	// #### SECOND STEP: bright-pass the picture
	// and store it into the GL_COLOR_ATTACHMENT1_EXT or 'brightPassTex'
	// Render it to the right texture: 'brightPassTex'
	glDrawBuffer( GL_COLOR_ATTACHMENT1_EXT );

	// Don't forget to clean the COLOR/DEPTH buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render using the horizontal blur shader and the original texture
	brightPassShaderProgram->useProgram();
	renderSceneOnQuad( originalTexture, GL_TEXTURE0);
	brightPassShaderProgram->disableProgram();

	// #### THIRD STEP: blur the texture horizontally 
	// and store it into the GL_COLOR_ATTACHMENT2_EXT or  'horizBlurredTex'
	// Render it to the right texture: 'horizBlurredTex'
	glDrawBuffer( GL_COLOR_ATTACHMENT2_EXT );

	// Don't forget to clean the COLOR/DEPTH buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render using the vertical blur shader and the horizontal blurred texture
	horizontalShaderProgram->useProgram();
	renderSceneOnQuad( brightPassTex, GL_TEXTURE0 );
	horizontalShaderProgram->disableProgram();

	// #### THIRD STEP: blur the texture vertically 
	// and store it into the 'finalBlurredTex'
	// Choose the right attachment
	glDrawBuffer( GL_COLOR_ATTACHMENT3_EXT );

	// Don't forget to clean the COLOR/DEPTH buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render using the vertical blur shader and the horizontal blurred texture
	verticalShaderProgram->useProgram();
	renderSceneOnQuad( horizBlurredTex, GL_TEXTURE0 );
	verticalShaderProgram->disableProgram();

	// 'unbind' the FBO. things will now be drawn to screen as usual
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// ******* RENDER TO THE WINDOW *************

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/************************************************************************/
	/* SHOULD FIND OUT THIS WELL !!!!                                       */
	/************************************************************************/
	glColor3f(1.0, 1.0, 1.0);

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	blenderShaderProgram->useProgram();
	//renderSceneOnQuad( finalBlurredTex, GL_TEXTURE0 );
	blendTextures(originalTexture, finalBlurredTex);
	blenderShaderProgram->disableProgram();
}

void GlowEffect::blendTextures(GLuint originalTexId, GLuint finalPassTexId)
{
	glEnable( GL_BLEND );
	glBlendFunc(GL_ONE, GL_ONE);

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, originalTexId);

	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, finalPassTexId);

	//RENDER MULTITEXTURE ON QUAD
	glBegin(GL_QUADS);
	glMultiTexCoord2f( GL_TEXTURE0, 0.0f, 0.0f );
	glMultiTexCoord2f( GL_TEXTURE1, 0.0f, 0.0f );
	glVertex2f( -1.0f, -1.0f );

	glMultiTexCoord2f( GL_TEXTURE0, 1.0f, 0.0f );
	glMultiTexCoord2f( GL_TEXTURE1, 1.0f, 0.0f );
	glVertex2f( 1.0f, -1.0f );

	glMultiTexCoord2f( GL_TEXTURE0, 1.0f, 1.0f );
	glMultiTexCoord2f( GL_TEXTURE1, 1.0f, 1.0f );
	glVertex2f( 1.0f, 1.0f );

	glMultiTexCoord2f( GL_TEXTURE0, 0.0f, 1.0f );
	glMultiTexCoord2f( GL_TEXTURE1, 0.0f, 1.0f );
	glVertex2f( -1.0f, 1.0f );
	glEnd();

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	/************************************************************************/
	/* WHY DO I NEED THIS CRAP???????                                       */
	/************************************************************************/

	glActiveTexture( GL_TEXTURE1 );
	glDisable(GL_TEXTURE_2D);

	glActiveTexture( GL_TEXTURE0 );
	glDisable(GL_TEXTURE_2D);

	glDisable( GL_BLEND );
}

/**
* @param textureId
* @param textureUnit
*/
void GlowEffect::renderSceneOnQuad(GLuint textureId, GLenum textureUnit)
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

/*
* Initializes the program shaders	
*/
void GlowEffect::initShaders()
{
	horizontalBlurVertexShader = new ShaderObject(GL_VERTEX_SHADER, "./shaders/horizBlur.vert");
	horizontalBlurFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "./shaders/horizBlur.frag");

	verticalBlurVertexShader = new ShaderObject(GL_VERTEX_SHADER, "./shaders/vertBlur.vert");
	verticalBlurFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "./shaders/vertBlur.frag");

	brightPassVertexShader = new ShaderObject(GL_VERTEX_SHADER, "./shaders/brightpass.vert");
	brightPassFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "./shaders/brightpass.frag");

	blenderVertexShader = new ShaderObject(GL_VERTEX_SHADER, "./shaders/glowblender.vert");
	blenderFragmentShader = new ShaderObject(GL_FRAGMENT_SHADER, "./shaders/glowblender.frag");

	horizontalShaderProgram = new ShaderProgram();
	verticalShaderProgram = new ShaderProgram();
	brightPassShaderProgram = new ShaderProgram();
	blenderShaderProgram = new ShaderProgram();

	horizontalShaderProgram->attachShader( *horizontalBlurVertexShader );
	horizontalShaderProgram->attachShader( *horizontalBlurFragmentShader );

	verticalShaderProgram->attachShader( *verticalBlurFragmentShader );
	verticalShaderProgram->attachShader( *verticalBlurVertexShader );

	brightPassShaderProgram->attachShader( *brightPassVertexShader );
	brightPassShaderProgram->attachShader( *brightPassFragmentShader );

	blenderShaderProgram->attachShader( *blenderVertexShader );
	blenderShaderProgram->attachShader( *blenderFragmentShader );

	textureUniform.setValue( 0 );
	textureUniform.setName("blurTex");

	blenderTextureUniform.setValue( 1 );
	blenderTextureUniform.setName("originalTex");

	horizontalShaderProgram->addUniformObject( &textureUniform );
	verticalShaderProgram->addUniformObject( &textureUniform );
	brightPassShaderProgram->addUniformObject( &textureUniform );
	blenderShaderProgram->addUniformObject( &textureUniform );
	blenderShaderProgram->addUniformObject( &blenderTextureUniform );

	horizontalShaderProgram->buildProgram();
	verticalShaderProgram->buildProgram();
	brightPassShaderProgram->buildProgram();
	blenderShaderProgram->buildProgram();
}

/**
*/
void GlowEffect::init()
{
	// ########### init shaders ###############
	initShaders();

	fbo = new FrameBufferObject();

	/************************************************************************/
	/* TEXTURE PART                                                         */
	/************************************************************************/
	// make a texture
	glGenTextures(1, &originalTexture);
	glGenTextures(1, &brightPassTex);
	glGenTextures(1, &horizBlurredTex);
	glGenTextures(1, &finalBlurredTex);

	glGenTextures(1, &depthBufferId);

	// initialize depth buffer
	/*glGenRenderbuffersEXT(1, &depthBufferId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferId);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,
	GL_DEPTH_COMPONENT24, imageWinWidth, imageWinHeight);*/
	glBindTexture(GL_TEXTURE_2D, depthBufferId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, imageWinWidth, imageWinHeight, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	fbo->bind();

	// initialize texture that will store the framebuffer image
	glBindTexture(GL_TEXTURE_2D, originalTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, brightPassTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, horizBlurredTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, finalBlurredTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);

	fbo->attachTexture(originalTexture, GL_COLOR_ATTACHMENT0_EXT, 0);
	fbo->attachTexture(brightPassTex,	GL_COLOR_ATTACHMENT1_EXT, 0);
	fbo->attachTexture(horizBlurredTex, GL_COLOR_ATTACHMENT2_EXT, 0);
	fbo->attachTexture(finalBlurredTex, GL_COLOR_ATTACHMENT3_EXT, 0);

	fbo->attachDepthBuffer( depthBufferId );

	FrameBufferObject::unbind();
}