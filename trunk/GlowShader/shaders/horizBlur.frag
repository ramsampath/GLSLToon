uniform sampler2D blurTex;

void main(void)
{
  // initial value
  vec4 outp = vec4(0.0, 0.0, 0.0, 0.0);
  
  // the blur displacement
  float ddx = 0.01;
 
  outp += 0.015625 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(ddx*-3.0, 0.0) );
  outp += 0.09375 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(ddx*-2.0, 0.0) );
  outp += 0.234375 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(ddx*-1.0, 0.0) );
  
  // center value
  outp += 0.3125 * texture2D(blurTex, gl_TexCoord[0].xy);
  
  // ddx * 1.0
  outp += 0.234375 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(ddx, 0.0) );
  outp += 0.09375 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(ddx*2.0, 0.0) );
  outp += 0.015625 * texture2D(blurTex, gl_TexCoord[0].xy + vec2(ddx*3.0, 0.0) );

  gl_FragColor =  outp;
 }