//¹öÀü ³·Ãã 450->440
#version 440

in vec3 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 1);
}
