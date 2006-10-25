varying vec3 normal;
//varying vec4 color;

void main()
{	
	// the light source position NORMALIZED here
	vec3 light = normalize( vec3(gl_LightSource[0].position) );
	
	// normalize the normal just to be safe
	vec3 n = normalize(normal);
	
	/**
	 * DIFFUSE COMPONENT PART
	 */
	
	vec4 objDiffuse = gl_FrontMaterial.diffuse;
	vec4 lightDiffuse = gl_LightSource[0].diffuse;
	
	// the angle between the light and the normal
	float theta = max( dot(light, n), 0.0);

	// Id = LightDiffuseRGBA * ObjectDiffuseRGBA * cos( theta )
	vec4 diffuseComponent = lightDiffuse * objDiffuse * theta;
	
	/**
	 * SPECULAR COMPONENT PART
	 */
	vec4 objSpecular = gl_FrontMaterial.specular;
	vec4 lightSpecular = gl_LightSource[0].specular;
	vec3 halfwayVec = gl_LightSource[0].halfVector.xyz;
	float shininess = gl_FrontMaterial.shininess;
	
	// the angle between the light and the normal
	float alpha = max( dot(halfwayVec, n), 0.0);
	
	shininess = pow(alpha, shininess);

	// Is = LightSpecularRGBA * ObjectSpecularRGBA * (cos( alpha ))^shininess
	vec4 specularComponent = lightDiffuse * objSpecular * shininess;
	
	gl_FragColor = diffuseComponent + specularComponent;
}