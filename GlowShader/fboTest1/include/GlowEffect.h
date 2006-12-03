/**
 * TODO: Brief Description
 * 
 * @file GlowEffect.h
 * @author João Pedro Jorge
 */


class GlowEffect
{
public:
	GlowEffect(unsigned int width = 512, unsigned int height = 512);
	~GlowEffect(void);

	void init();

	void begin();
	void end();

private:
	void renderSceneOnQuad(GLuint textureId, GLenum textureUnit);
	void initShaders();

	void blendTextures(GLuint originalTexId, GLuint finalPassTexId);

	void blurToMipmaps(GLenum horizontalBuffer, GLenum verticalBuffer, float mipmapLevel, float delta);

private:
	unsigned int imageWinWidth;
	unsigned int imageWinHeight;
	GLuint originalTexture, brightPassTex, horizBlurredTex, finalBlurredTex;

	// ########### SHADERS DECLARATIONS ###########
	ShaderProgram* horizontalShaderProgram;
	ShaderProgram* verticalShaderProgram;
	ShaderProgram* brightPassShaderProgram;
	ShaderProgram* blenderShaderProgram;
	
	ShaderObject* brightPassVertexShader;
	ShaderObject* brightPassFragmentShader;

	ShaderObject* blenderVertexShader;
	ShaderObject* blenderFragmentShader;

	ShaderObject* horizontalBlurVertexShader;
	ShaderObject* horizontalBlurFragmentShader;
	
	ShaderObject* verticalBlurVertexShader;
	ShaderObject* verticalBlurFragmentShader;
	ShaderUniformValue<int> textureUniform;
	ShaderUniformValue<int> blenderTextureUniform;
	ShaderUniformValue<float> blurDeltaUniform;
	ShaderUniformValue<float> mipmapLevelUniform;
	// ########### END SHADERS DECLARATIONS ###########

	// ########### FBO DECLARATIONS ###########
	GLuint depthBufferId;
	FrameBufferObject* fbo;
	// ########### END FBO DECLARATIONS ###########
};
