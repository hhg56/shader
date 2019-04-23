//¹öÀü ³·Ãã 450->440
#version 440

layout(location=0) out vec4 FragColor;

in vec2 v_Texture;
in vec2 v_Position;

uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{
	vec4 newcolor = vec4(0);
	float newtime = u_Time;
	newtime = fract(newtime);
	if(v_Texture.y - 0.5 < sin(2 * 3.141592 * (v_Texture.x + newtime)) / 2 && v_Texture.y - 0.5 > sin(2 * 3.141592 * (v_Texture.x + newtime)) / 2 - 0.01) newcolor = vec4(v_Texture, 1, 1);

	//FragColor = vec4(v_Texture, 0, 1);
	//FragColor = texture(u_Texture, v_Texture);
	FragColor = newcolor;
}
