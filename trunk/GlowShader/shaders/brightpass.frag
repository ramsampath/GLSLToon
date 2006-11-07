uniform sampler2D blurTex;

void convertRGBtoHSL(vec3 rgb, out vec3 hsl);
void convertHSLtoRGB(vec3 hsl, out vec3 rgb);
float hueRGB(float v1, float v2, inout float hue);
float brightPassValue(float luminance);

void main(void)
{
	vec4 texColor = texture2D(blurTex, gl_TexCoord[0].xy);
	vec3 colorHSL;
	vec3 color;
	
	color = texColor.xyz;
	
	convertRGBtoHSL(color, colorHSL);
	
	//colorHSL.z = brightPassValue( colorHSL.z );
	
	convertHSLtoRGB(colorHSL, color);
	
	gl_FragColor = vec4(color, 1.0);
	//gl_FragColor = texture2D(blurTex, gl_TexCoord[0].xy);
}

/*
 * @description
 */
float brightPassValue(float luminance)
{
	if( luminance > 0.6)
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
float hueRGB(float v1, float v2, inout float hue)
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
	if( hsl.y == 0.0 )
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
			var_2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);
		}
		
		float var_1 = 2.0 * hsl.z - var_2;
		
		float t = hsl.x + (1.0/ 3.0);
		float s = hsl.x - (1.0/ 3.0);
		rgb.r = hueRGB( var_1, var_2, t);
		rgb.g = hueRGB( var_1, var_2, hsl.x );
		rgb.b = hueRGB( var_1, var_2, s );
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