//¹öÀü ³·Ãã 450->440
#version 440

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Texture;

in vec4 v_Color;
in vec2 v_OriXY;
in float v_Radius;

void main()
{
	/*
	vec4 newColor;
	float dis = sqrt(v_OriXY.x * v_OriXY.x + v_OriXY.y * v_OriXY.y);
	if(dis < v_Radius)
	{
		newColor = v_Color;
		newColor.a = 1 - dis / v_Radius;
	}
	else{
		newColor = vec4(0, 0, 0, 0);
	}
	FragColor = newColor;
	*/

	FragColor = texture(u_Texture, (v_OriXY/v_Radius));

}
