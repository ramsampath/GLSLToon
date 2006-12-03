uniform sampler2D blurTex;

// the blur displacement
uniform float delta;

// the mipmap level
uniform float level;

void main(void)
{
	// initial value
	vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);
	vec2 texCoord = gl_TexCoord[0].xy;

	outp += texture2D(blurTex, texCoord + vec2(delta*-3.0, 0.0), level);
	outp += texture2D(blurTex, texCoord + vec2(delta*-2.0, 0.0), level );
	outp += texture2D(blurTex, texCoord + vec2(delta*-1.0, 0.0), level );

	// center value
	outp += texture2D(blurTex, texCoord, level);

	// delta * 1.0
	outp += texture2D(blurTex, texCoord + vec2(delta, 0.0), level );
	outp += texture2D(blurTex, texCoord + vec2(delta*2.0, 0.0), level );
	outp += texture2D(blurTex, texCoord + vec2(delta*3.0, 0.0), level );

	outp /= 7.0;

	gl_FragColor = outp;
 }