uniform sampler2D blurTex;

void convertRGBtoHSL(vec3 rgb, out vec3 hsl);
void convertHSLtoRGB(vec3 hsl, out vec3 rgb);
float hueRGB(float v1, float v2, float hue);
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
	
	// convert it back to RGB
	//convertHSLtoRGB(colorHSL, color);
	
	color *= colorHSL.z;
	
	gl_FragColor = vec4(color.g, color.g, color.g, 1.0);
	
	gl_FragColor = vec4(color, 1.0);
	
	//gl_FragColor = texture2D(blurTex, gl_TexCoord[0].xy);
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
 * @description 
 */
float hueRGB(float v1, float v2, float hue)
{
	if( hue < 0.0 )
	{
		hue += 1.0;
	}
	else if( hue > 1.0 )
	{
		hue -= 1.0;
	}
   
	if( (6.0 * hue) < 1.0 )
	{
		return ( v1 + ( v2 - v1 ) * 6.0 * hue );
	}
	else if( (2.0 * hue) < 1.0 )
	{
		return v2;
	}
	else if( (3.0 * hue) < 2.0 )
	{
		return ( v1 + ( v2 - v1 ) * ( ( 2.0 / 3.0 ) - hue ) * 6.0 );
	}
	
	return v1;
}

/*
 * @description converts from HSL to RGB
 */
void convertHSLtoRGB(vec3 hsl, out vec3 rgb)
{
	//if( hsl.y == 0.0 )
	if( hsl.y < 0.000001 )
	{
		rgb.r = hsl.z;
		rgb.g = hsl.z;
		rgb.b = hsl.z;
	}
	else
	{
		float var_2;
		if( hsl.z < 0.5 )
		{
			var_2 = hsl.z * (1.0 + hsl.y);
		}
		else
		{
			var_2 = hsl.z + hsl.y - (hsl.z * hsl.y);
		}
		
		float var_1 = 2.0 * hsl.z - var_2;
		
		float r = hsl.x + (1.0 / 3.0);
		float b = hsl.x - (1.0 / 3.0);
		
		rgb.r = hueRGB( var_1, var_2, r);
		rgb.g = hueRGB( var_1, var_2, hsl.x );
		rgb.b = hueRGB( var_1, var_2, b );
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