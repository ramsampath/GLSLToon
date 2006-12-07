// the normal: passed to the fragment shader

varying vec3 normal;
//varying vec4 color;


void main()
{	
	// the vertex normal -> passed to the fragment shader AS an interpolated normal
	// from space-coords to eye-coords
	normal = gl_NormalMatrix * gl_Normal;
	
	gl_Position = ftransform();
}