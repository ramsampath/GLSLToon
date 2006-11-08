uniform sampler2D blurTex;

void main(void)
{
  gl_FragColor =  texture2D(blurTex, gl_TexCoord[0].xy);
}