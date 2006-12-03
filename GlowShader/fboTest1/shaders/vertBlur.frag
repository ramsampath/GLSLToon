uniform sampler2D blurTex;

// the blur displacement
uniform float delta;

void main(void)
{
	// initial value
	vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);
	vec2 texCoord = gl_TexCoord[0].xy;

	outp += texture2D(blurTex, texCoord + vec2(0.0, delta*-3.0) );
	outp += texture2D(blurTex, texCoord + vec2(0.0, delta*-2.0) );
	outp += texture2D(blurTex, texCoord + vec2(0.0, delta*-1.0) );

	// center value
	outp += texture2D(blurTex, texCoord);

	// delta * 1.0
	outp += texture2D(blurTex, texCoord + vec2(0.0, delta) );
	outp += texture2D(blurTex, texCoord + vec2(0.0, delta*2.0) );
	outp += texture2D(blurTex, texCoord + vec2(0.0, delta*3.0) );

	outp /= 7.0;

	gl_FragColor =  outp;
}