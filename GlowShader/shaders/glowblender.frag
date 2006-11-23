uniform sampler2D blurTex;
uniform sampler2D originalTex;

void main(void)
{
	vec4 originalColor = texture2D(originalTex, gl_TexCoord[0].xy);
	vec4 blur = texture2D(blurTex, gl_TexCoord[0].xy);
	gl_FragColor = originalColor + blur;
}