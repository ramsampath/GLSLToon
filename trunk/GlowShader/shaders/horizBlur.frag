uniform sampler2D blurTex;

void main(void)
{
  // initial value
  vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);
  
  // the blur displacement
  float ddx = 0.01;
  vec2 texCoord = gl_TexCoord[0].xy;
 
  outp += 0.00038771 * texture2D(blurTex, texCoord + vec2(ddx*-3.0, 0.0) );
  outp += 0.11330373 * texture2D(blurTex, texCoord + vec2(ddx*-2.0, 0.0) );
  outp += 0.31098164 * texture2D(blurTex, texCoord + vec2(ddx*-1.0, 0.0) );
  
  // center value
  outp += 0.52508352 * texture2D(blurTex, texCoord);
  
  // ddx * 1.0
  outp += 0.31098164 * texture2D(blurTex, texCoord + vec2(ddx, 0.0) );
  outp += 0.11330373 * texture2D(blurTex, texCoord + vec2(ddx*2.0, 0.0) );
  outp += 0.00038771 * texture2D(blurTex, texCoord + vec2(ddx*3.0, 0.0) );

  gl_FragColor =  outp;
 }