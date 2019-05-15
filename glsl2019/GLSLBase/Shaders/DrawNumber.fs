//버전 낮춤 450->440
#version 440

layout(location=0) out vec4 FragColor;

in vec2 v_Texture;
in vec2 v_Position;

uniform sampler2D u_Texture;
uniform float u_Time;
uniform int u_Number[3];

void main()
{
	//vec4 newcolor = vec4(0);
	//vec2 newTex = vec2(v_Texture.x, 1 - v_Texture.y);
	//
	//if(newTex.x < 0.5 && newTex.y < 0.5) newcolor = texture(u_Texture, fract(v_Texture * 2));
	//else if(newTex.x > 0.5 && newTex.y < 0.5) newcolor = texture(u_Texture_1, fract(v_Texture * 2));
	//else if(newTex.x < 0.5 && newTex.y > 0.5) newcolor = texture(u_Texture_2, fract(newTex * 2));
	//else newcolor = texture(u_Texture_3, fract(v_Texture * 2));
	//FragColor = newcolor;

	vec4 newcolor = vec4(0);
	vec2 newTex = v_Texture;
	
	newTex.x = fract(v_Texture.x * 3);

	int newIndex = int(floor(v_Texture.x * 3));

	float newy = 9 - float(u_Number[newIndex]);
	newTex.y = v_Texture.y / 10.0 + newy / 10.0;
	
	newcolor = texture(u_Texture, newTex);
	FragColor = newcolor;

	//homework : 16x16짜리 스프라이트를 읽게 0~64이렇게 받아서 읽게 ㄱㄱ
}