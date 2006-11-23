uniform sampler2D blurTex;

void main(void)
{
  // initial value
  vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);
  
  // the blur displacement
  float ddy = 0.01;
  vec2 texCoord = gl_TexCoord[0].xy;
 
  outp += 0.00038771 * texture2D(blurTex, texCoord + vec2(0.0, ddy*-3.0) );
  outp += 0.11330373 * texture2D(blurTex, texCoord + vec2(0.0, ddy*-2.0) );
  outp += 0.31098164 * texture2D(blurTex, texCoord + vec2(0.0, ddy*-1.0) );
  
  // center value
  outp += 0.52508352 * texture2D(blurTex, texCoord);
  
  // ddy * 1.0
  outp += 0.31098164 * texture2D(blurTex, texCoord + vec2(0.0, ddy) );
  outp += 0.11330373 * texture2D(blurTex, texCoord + vec2(0.0, ddy*2.0) );
  outp += 0.00038771 * texture2D(blurTex, texCoord + vec2(0.0, ddy*3.0) );

  gl_FragColor =  outp;
}