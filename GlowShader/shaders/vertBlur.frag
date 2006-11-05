uniform sampler2D blurTex;

void main(void)
{
  // initial value
  vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);
  
  // the blur displacement
  float ddy = 0.01;
 
  outp += 0.015625 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(0.0, ddy*-3.0) );
  outp += 0.09375 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(0.0, ddy*-2.0) );
  outp += 0.234375 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(0.0, ddy*-1.0) );
  
  // center value
  outp += 0.3125 * texture2D(blurTex, gl_TexCoord[0].xy);
  
  // ddy * 1.0
  outp += 0.234375 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(0.0, ddy) );
  outp += 0.09375 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(0.0, ddy*2.0) );
  outp += 0.015625 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(0.0, ddy*3.0) );

  //gl_FragColor =  outp;
  gl_FragColor =  texture2D(blurTex, gl_TexCoord[0].xy);
 }