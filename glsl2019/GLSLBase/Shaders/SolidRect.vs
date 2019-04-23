//¹öÀü ³·Ãã 450->440
#version 440

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;

uniform float u_Time;	// 1.f

void main()
{
	gl_Position = vec4(a_Position.xyz, 1);
}
