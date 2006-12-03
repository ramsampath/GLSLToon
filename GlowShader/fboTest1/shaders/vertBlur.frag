uniform sampler2D blurTex;
uniform float delta;

void main(void)
{
	// initial value
	vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);

	// the blur displacement
	//float ddy = 0.01;
	vec2 texCoord = gl_TexCoord[0].xy;

	//outp += 0.015625 * texture2D(blurTex, texCoord + vec2(0.0, ddy*-3.0) );
	//outp += 0.09375 * texture2D(blurTex, texCoord + vec2(0.0, ddy*-2.0) );
	//outp += 0.234375 * texture2D(blurTex, texCoord + vec2(0.0, ddy*-1.0) );

	//// center value
	//outp += 0.3125 * texture2D(blurTex, texCoord);

	//// ddy * 1.0
	//outp += 0.234375 * texture2D(blurTex, texCoord + vec2(0.0, ddy) );
	//outp += 0.09375 * texture2D(blurTex, texCoord + vec2(0.0, ddy*2.0) );
	//outp += 0.015625 * texture2D(blurTex, texCoord + vec2(0.0, ddy*3.0) );

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