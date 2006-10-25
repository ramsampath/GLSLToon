uniform sampler2D toonTexture;
varying vec3 normal;

void main()
{
	vec4 color;
	float intensity;
	vec3 light = normalize(vec3(gl_LightSource[0].position));
	
	intensity = max( dot(light, normalize(normal)), 0.0);
	
	color = texture2D( toonTexture, vec2(intensity, 0.0) );
	
	gl_FragColor = color;
}