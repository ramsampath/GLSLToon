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

/**
 */
void GlowEffect::finishOriginal()
{
	// 'Unbind' the FBO. things will now be drawn to screen as usual
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer( currenDrawBuffer );

	// ******* RENDER TO THE WINDOW *************

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	blenderShaderProgram->useProgram();
		//renderSceneOnQuad( textures[3], GL_TEXTURE0 );
		blendTextures(textures[3], textures[2]);
	blenderShaderProgram->disableProgram();
}

/**
 * @description should be called BEFORE the original scene
 * is being drawn. This scene will be additively blended to
 * the glow source drawn between beginGlowSource() and endGlowSource()
 */
void GlowEffect::beginOriginal()
{
	glDrawBuffer( GL_COLOR_ATTACHMENT3_EXT );

	// #### FIRST STEP: Draw the object into the original texture: 'originalTexture'
	glClearColor(0.0f, 0.0f, 0.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 */
void GlowEffect::beginGlowSource()
{
	// saves the current draw buffer
	glGetIntegerv(GL_DRAW_BUFFER, &currenDrawBuffer);

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

/**
 * @description should be called AFTER the original scene
 * is being drawn. This scene will be additively blended to
 * the glow source drawn between beginGlowSource() and endGlowSource()
 */
void GlowEffect::endGlowSource()
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

	/************************************************************************/
	/* PROVAVELMENTE POSSO REMOVER O BRIGHTPASS!!! A FONTE VEM JÁ SELECCIONADA ***/
	/************************************************************************/

	// do the bright pass
	brightPassShaderProgram->useProgram();
		renderSceneOnQuad( textures[0], GL_TEXTURE0);
	brightPassShaderProgram->disableProgram();

	// generate mipmaps for the brightpass
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	// ******* BLUR MIPMAPS AND BLEND ************
	blurMipmaps(GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, 6.0f, 0.03f);
	blurMipmaps(GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT1_EXT, 3.0f, 0.005f);

	// Blend both blurred sources
	glDrawBuffer( GL_COLOR_ATTACHMENT2_EXT );

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// texture[2] will store the blended and final blur
	blenderShaderProgram->useProgram();
		blendTextures(textures[1], textures[3]);
	blenderShaderProgram->disableProgram();
}

/**
 * @param horizontalBuffer the GLenum buffer where the horizontal blur should be stored
 * @param verticalBuffer the GLenum buffer where the vertical blur should be stored
 * @param mipmapLevel the mipmap level for what the blur is being done
 * @param delta the texcoord displacement used to compute the blur kernel
 */
void GlowEffect::blurMipmaps( GLenum horizontalBuffer, GLenum verticalBuffer, 
							 float mipmapLevel, float delta )
{
	/************************************************************************/
	/* HORIZONTAL BLUR                                                      */
	/************************************************************************/
	glDrawBuffer( horizontalBuffer );

	// Don't forget to clean the COLOR/DEPTH buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mipmapLevelUniform.setValue( mipmapLevel );
	blurDeltaUniform.setValue( delta );
	
	// render using the horizontal blur shader and the brightpass texture
	horizontalShaderProgram->useProgram();
		renderSceneOnQuad( textures[1], GL_TEXTURE0 );
	horizontalShaderProgram->disableProgram();
	
	blurDeltaUniform.setValue( delta );

	/************************************************************************/
	/* VERTICAL BLUR                                                        */
	/************************************************************************/
	glDrawBuffer( verticalBuffer );

	// Don't forget to clean the COLOR/DEPTH buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render using the vertical blur shader and the horizontal blurred texture
	verticalShaderProgram->useProgram();
		renderSceneOnQuad( textures[2], GL_TEXTURE0 );
	verticalShaderProgram->disableProgram();
}

void GlowEffect::blendTextures(GLuint originalTexId, GLuint finalPassTexId)
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, originalTexId);

	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, finalPassTexId);

	/************************************************************************/
	/* PODE SER OPTIMIZADO GUARDANDO AS COORDENADAS EM VECTOR!!!!           */
	/************************************************************************/
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

	glMatrixMode( GL_MODELVIEW );

	// disable texture units
	glActiveTexture( GL_TEXTURE1 );
	glDisable(GL_TEXTURE_2D);

	glActiveTexture( GL_TEXTURE0 );
	glDisable(GL_TEXTURE_2D);
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

	glMatrixMode( GL_MODELVIEW );

	glActiveTexture( textureUnit );
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

	mipmapLevelUniform.setName("level");
	mipmapLevelUniform.setValue( 0.0f );
	
	blurDeltaUniform.setName("delta");
	blurDeltaUniform.setValue( 0.01f );

	horizontalShaderProgram->addUniformObject( &textureUniform );
	horizontalShaderProgram->addUniformObject( &mipmapLevelUniform );
	horizontalShaderProgram->addUniformObject( &blurDeltaUniform );
	
	verticalShaderProgram->addUniformObject( &textureUniform );
	verticalShaderProgram->addUniformObject( &blurDeltaUniform );
	
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
	// ########### Init Shaders ###############
	initShaders();

	fbo = new FrameBufferObject();

	// ########### Texture Creation ###############
	glGenTextures(4, textures);

	fbo->bind();

	// create and initialize depth buffer
	glGenRenderbuffersEXT(1, &depthBufferId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferId); 
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, imageWinWidth, imageWinHeight);

	// initialize texture that will store the framebuffer image
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);

	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);

	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWinWidth, imageWinHeight, 0,
		GL_RGBA, GL_FLOAT, NULL);

	glGenerateMipmapEXT(GL_TEXTURE_2D);

	fbo->attachTexture(textures[0], GL_COLOR_ATTACHMENT0_EXT, 0);
	fbo->attachTexture(textures[1],	GL_COLOR_ATTACHMENT1_EXT, 0);
	fbo->attachTexture(textures[2], GL_COLOR_ATTACHMENT2_EXT, 0);
	fbo->attachTexture(textures[3], GL_COLOR_ATTACHMENT3_EXT, 0);

	fbo->attachDepthBuffer( depthBufferId );

	FrameBufferObject::unbind();
}