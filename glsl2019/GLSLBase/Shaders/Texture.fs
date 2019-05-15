//버전 낮춤 450->440
#version 440

layout(location=0) out vec4 FragColor;

in vec2 v_Texture;
in vec2 v_Position;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture_1;
uniform sampler2D u_Texture_2;
uniform sampler2D u_Texture_3;
uniform float u_Time;

void main()
{
	//default
	//vec2 texpos = v_Texture;
	//vec4 newcolor = vec4(0);
	//
	//newcolor = texture(u_Texture, texpos);
	//
	//FragColor = newcolor;

	//blur
	//vec2 newtex[5];
	//vec4 newcolor[5];
	//
	//float width = 1.0/256.0;
	//float height = 1.0/256.0;
	//
	//newtex[0] = vec2(v_Texture.x, v_Texture.y);
	//newtex[1] = vec2(v_Texture.x-1.0 / width, v_Texture.y);
	//newtex[2] = vec2(v_Texture.x+1.0 / width, v_Texture.y);
	//newtex[3] = vec2(v_Texture.x, v_Texture.y - 1.0/height);
	//newtex[4] = vec2(v_Texture.x, v_Texture.y + 1.0/height);
	//
	//newcolor[0] = texture(u_Texture, newtex[0]);
	//newcolor[1] = texture(u_Texture, newtex[1]);
	//newcolor[2] = texture(u_Texture, newtex[2]);
	//newcolor[3] = texture(u_Texture, newtex[3]);
	//newcolor[4] = texture(u_Texture, newtex[4]);
	//
	//vec4 finalcolor = newcolor[0] + newcolor[1] + newcolor[2] + newcolor[3] + newcolor[4];
	//finalcolor /= 5.0
	//
	//FragColor = finalcolor;

	//vec2 newtex = vec2(fract(v_Texture.x*3), v_Texture.y / 3 + floor(v_Texture.x * 3.0) / 3.0);	//	offset + 0~1/3 - 우좌 거꾸로
	//vec2 newtex = vec2(fract(v_Texture.x*3), v_Texture.y / 3 + floor((1-v_Texture.x) * 3.0) / 3.0);	//	offset + 0~1/3
	//vec2 newtex = vec2(fract(v_Texture.x*3), v_Texture.y / 3 + (2 - floor(v_Texture.x * 3.0) / 3.0));	// 좌우 거꾸로
	//vec2 newtex = vec2(v_Texture.x, (2 - floor(v_Texture.y * 3)) / 3 + fract(v_Texture.y * 3) / 3);	//	floor : 나머지 - 아래위 거꾸로
	
	//vec2 newtex = vec2(fract(v_Texture.x * 2) + floor(v_Texture.y * 2) / 2, fract(v_Texture.y * 2));	// 시험??좀바꿔서...
	//vec4 newcolor = texture(u_Texture, newtex);
	//
	//FragColor = newcolor;

	vec4 newcolor = vec4(0);
	vec2 newTex = vec2(v_Texture.x, 1 - v_Texture.y);
	
	if(newTex.x < 0.5 && newTex.y < 0.5) newcolor = texture(u_Texture, fract(v_Texture * 2));
	else if(newTex.x > 0.5 && newTex.y < 0.5) newcolor = texture(u_Texture_1, fract(v_Texture * 2));
	else if(newTex.x < 0.5 && newTex.y > 0.5) newcolor = texture(u_Texture_2, fract(newTex * 2));
	else newcolor = texture(u_Texture_3, fract(v_Texture * 2));
	FragColor = newcolor;
		
	//vec2 newtex[3];
	//vec4 newcolor[3];
	//
	//float width = 1.0/3.0;
	//
	//newtex[0] = vec2(v_Texture.x, v_Texture.y / 3.0 + 2.0 / 3.0);	// R 
	//newtex[1] = vec2(v_Texture.x, v_Texture.y / 3.0 + 1.0 / 3.0);	// G
	//newtex[2] = vec2(v_Texture.x, v_Texture.y / 3.0);				// B
	//
	//newcolor[0] = texture(u_Texture, vec2(newtex[0].x / 3.0 + 2.0 / 3.0, newtex[0].y));
	//newcolor[1] = texture(u_Texture, vec2(newtex[1].x / 3.0 + 1.0 / 3.0, newtex[1].y));
	//newcolor[2] = texture(u_Texture, vec2(newtex[2].x / 3.0, newtex[2].y));
	//
	//vec4 finalcolor = newcolor[0] + newcolor[1] + newcolor[2];
	//
	//FragColor = finalcolor;
}
