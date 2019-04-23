//¹öÀü ³·Ãã 450->440
#version 440

in vec3 a_Position;
in vec2 a_Uv;

//Varying -> fragment shader input
out vec4 v_Temp;

uniform float u_Time;	// 1.f

const vec3 c_Gravity = vec3(-0.1, -0.1, 0);

void main()
{
	gl_Position = vec4(a_Position.xyz, 1);

	v_Temp = vec4(a_Uv, 0, 1);
}
