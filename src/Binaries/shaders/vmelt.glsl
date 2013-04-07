// Test melt vertex shader

varying vec4 vColor;

uniform float vrandomN;
void main()
{
	vColor = gl_Color;

	float PI = 3.14159265358979323846264;
	float angle = 45.0;
	float rad_angle = angle * PI / 180.0;

	vec4 a = gl_Vertex;
	vec4 b = a;

	//b.x = a.x * cos(rad_angle) - a.y * sin(rad_angle);
	//b.y = a.y * cos(rad_angle) + a.x * sin(rad_angle);

	b.y -= vrandomN;

	gl_Position = gl_ModelViewProjectionMatrix * b;
}
