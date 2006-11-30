uniform sampler2D blurTex;
uniform float delta;
uniform float level;

void main(void)
{
	// initial value
	vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);

	// the blur displacement
	//float ddx = 0.01;
	vec2 texCoord = gl_TexCoord[0].xy;

	//outp += 0.015625 * texture2D(blurTex, texCoord + vec2(ddx*-3.0, 0.0) );
	//outp += 0.09375 * texture2D(blurTex, texCoord + vec2(ddx*-2.0, 0.0) );
	//outp += 0.234375 * texture2D(blurTex, texCoord + vec2(ddx*-1.0, 0.0) );

	//// center value
	//outp += 0.3125 * texture2D(blurTex, texCoord);

	//// ddx * 1.0
	//outp += 0.234375 * texture2D(blurTex, texCoord + vec2(ddx, 0.0) );
	//outp += 0.09375 * texture2D(blurTex, texCoord + vec2(ddx*2.0, 0.0) );
	//outp += 0.015625 * texture2D(blurTex, texCoord + vec2(ddx*3.0, 0.0) );

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