//¹öÀü ³·Ãã 450->440
#version 440

layout(location=0) out vec4 FragColor;

in vec4 v_Temp;

uniform float u_Time;
uniform vec2 u_Points[5];

const vec2 c_Points[2] = {vec2(0), vec2(0.1)};

void main()
{
	vec2 newuv = v_Temp.xy - vec2(0.5);	// -0.5 ~ 0.5

	float pointGrey = 0;
	
	float distance = length(newuv);
	float newTime = fract(u_Time);
	float ringwidth = 0.1;
	
	if(distance < newTime + ringwidth && distance > newTime - ringwidth)
	{
		float temp = (distance - newTime) / ringwidth;
		pointGrey = temp;

		for(int i = 0; i < 2; i++)
		{
			vec2 newPoints = u_Points[i];
			vec2 newvec = newPoints - newuv;
			float distance = length(newvec);
			if(distance < 0.1)
			{
				pointGrey += 0.5 * pow((1 - (distance / 0.1)), 5);	// °ãÄ¡¸é »öÀÌ ´õ ÁøÇØÁü
			}
		}
	}

	// float distance = sqrt(newuv.x * newuv.x + newuv.y * newuv.y);
	// float grey = sin(distance * 3.141592 * 4 * 100);
	
	/*if(distance < 0.5 && distance > 0.48)
	{
		FragColor = vec4(1);
	}
	else
	{
		FragColor = vec4(0);
	}*/

	FragColor = vec4(pointGrey);
}
