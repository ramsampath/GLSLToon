uniform sampler2D blurTex;
uniform sampler2D originalTex;

void main(void)
{
	vec4 originalColor = texture2D(originalTex, gl_TexCoord[0].xy);
	originalColor *= originalColor.a;
	
	vec4 blur1 = texture2D(blurTex, gl_TexCoord[0].xy, 0.0);
	blur1 *= blur1.a;
	vec4 blur2 = texture2D(blurTex, gl_TexCoord[0].xy, 1.0);
	blur2 *= blur2.a;
	vec4 blur3 = texture2D(blurTex, gl_TexCoord[0].xy, 2.0);
	blur3 *= blur3.a;
	vec4 blur4 = texture2D(blurTex, gl_TexCoord[0].xy, 3.0);
	blur4 *= blur4.a;
	
	gl_FragColor = blur1 + blur2 + blur3 + blur4 + originalColor;
}