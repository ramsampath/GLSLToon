varying vec3 normal;

void main()
{
	vec4 color;
	float intensity;
	float alpha;
	vec3 light = normalize(vec3(gl_LightSource[0].position));
	
	intensity = dot(light, normalize(normal));
	
	if( intensity < 0.0 )
	{
		alpha = 1.0;
	}
	else
	{
		alpha = 0.0;
	}
	
	gl_FragColor = vec4(0.0, 0.0, 0.0, alpha);
}