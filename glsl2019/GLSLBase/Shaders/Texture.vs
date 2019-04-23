//¹öÀü ³·Ãã 450->440
#version 440

in vec3 a_Position;
in vec2 a_Texture;

out vec2 v_Texture;
out vec2 v_Position;

void main()
{
	gl_Position = vec4(a_Position.xyz, 1);
	v_Texture = a_Texture;
	v_Position = a_Position.xy;
}
