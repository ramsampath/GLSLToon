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

	void finishOriginal();
	void beginOriginal();

	void beginGlowSource();
	void endGlowSource();

private:
	void renderSceneOnQuad(GLuint textureId, GLenum textureUnit);
	void initShaders();

	void blendTextures(GLuint originalTexId, GLuint finalPassTexId);

	void blurMipmaps(GLenum horizontalBuffer, GLenum verticalBuffer, float mipmapLevel, float delta);

private:
	unsigned int imageWinWidth;
	unsigned int imageWinHeight;
	GLuint textures[4];

	// ########### SHADERS DECLARATIONS ###########
	ShaderProgram* horizontalShaderProgram;
	ShaderProgram* verticalShaderProgram;
	ShaderProgram* blenderShaderProgram;
	
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
	GLint currenDrawBuffer;
	FrameBufferObject* fbo;
	// ########### END FBO DECLARATIONS ###########
};
