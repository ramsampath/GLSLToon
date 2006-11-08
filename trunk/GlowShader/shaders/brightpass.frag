uniform sampler2D blurTex;

void convertRGBtoHSL(vec3 rgb, out vec3 hsl);
float brightPassValue(float luminance);

void main(void)
{
	// get the fragment's texture color
	vec4 texColor = texture2D(blurTex, gl_TexCoord[0].xy);
	vec3 colorHSL;
	vec3 color;
	
	color = texColor.rgb;
	
	// convert the color to HSL
	convertRGBtoHSL(color, colorHSL);
	
	// apply the bright filter using the Luminance value
	colorHSL.z = brightPassValue( colorHSL.z );

	color *= colorHSL.z;
	
	gl_FragColor = vec4(color, 1.0);
}

/*
 * @description
 */
float brightPassValue(float luminance)
{
	if( luminance > 0.8)
	{
		return max(luminance * 1.8, 1.0);
	}
	else
	{
		return max(luminance * 0.4, 0.0);
	}
}

/*
 * @description converts from RGB to HSL
 */
void convertRGBtoHSL(vec3 rgb, out vec3 hsl)
{
	hsl = vec3(0.0);
	
	// Min. value of RGB
	float minRGB = min( rgb.r, min(rgb.g, rgb.b) );
	
	// Max. value of RGB
	float maxRGB = max( rgb.r, max(rgb.g, rgb.b) );

	hsl.z = ( maxRGB + minRGB ) / 2.0;
	
	// perhaps set a limit like 0.00001
	if( hsl.z != 0.0 )
	{
		float delta = maxRGB - minRGB;
		
		// compute the S value
		if( hsl.z <= 0.5 )
		{
			hsl.y = delta / ( maxRGB + minRGB );
		}
		else
		{
			hsl.y = delta / ( 2.0 - maxRGB - minRGB );
		}
		
		float deltaR = ( ( ( maxRGB - rgb.r ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
		float deltaG = ( ( ( maxRGB - rgb.g ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
		float deltaB = ( ( ( maxRGB - rgb.b ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
		
		// compute H value
		if( rgb.r == maxRGB )
		{
			hsl.x = deltaB - deltaG;
		}
//		if( (rgb.r == maxRGB) && (rgb.g >= rgb.b) )
//		{
//			hsl.x = deltaB - deltaG;
//		}
//		else if( rgb.r == maxRGB && (rgb.g < rgb.b) )
//		{
//			hsl.x = deltaB - deltaG + 1.0;
//		}
		else if( rgb.g == maxRGB )
		{
			hsl.x = (1.0 / 3.0) + deltaR - deltaB;
		}
		else if( rgb.b == maxRGB )
		{
			hsl.x = (2.0 / 3.0) + deltaG - deltaR;
		}
		
		if( hsl.x < 0.0)
		{
			hsl.x += 1.0;
		}
		else if( hsl.x > 1.0)
		{
			hsl.x -= 1.0;
		}
	}
	
}