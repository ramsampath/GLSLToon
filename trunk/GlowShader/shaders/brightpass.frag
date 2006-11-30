uniform sampler2D blurTex;

float getLuminanceFromRGB(vec3 rgb);
float brightPassValue(float luminance, out float alpha);

void main(void)
{
	// get the fragment's texture color
	vec3 color = texture2D(blurTex, gl_TexCoord[0].xy).xyz;

	// convert the color to HSL
	float luminance = getLuminanceFromRGB(color);
	
	//
	//
	// STORE THE LUMINANCE IN THE ALPHA VALUE AS WELL???
	//
	//
	
	float alpha = 0.0;

	// apply the bright filter using the Luminance value
	luminance = brightPassValue(luminance, alpha);

	color *= luminance;
	
	gl_FragColor = vec4(color, alpha);
}

/*
 * @description
 */
float brightPassValue(float luminance, out float alpha)
{
	if( luminance > 0.7)
	{
		return max(luminance * 1.2, 1.0);
	}
	else
	{
		return max(luminance * 0.3, 0.0);
	}
	alpha = luminance;
}

/*
 * @description returns the luminance inherent to the RGB values
 */
float getLuminanceFromRGB(vec3 rgb)
{	
	// Min. value of RGB
	float minRGB = min( rgb.r, min(rgb.g, rgb.b) );
	
	// Max. value of RGB
	float maxRGB = max( rgb.r, max(rgb.g, rgb.b) );

	return ( maxRGB + minRGB ) / 2.0;
}