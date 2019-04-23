//버전 낮춤 450->440
#version 440

// Attribute
in vec3			a_Position;
in vec3			a_Vel;
in vec2			a_StartLife;
in vec2			a_RatioAmp;
in float		a_Value;
in vec4			a_Color;

//Varying -> fragment shader input
out vec4 v_Color;
out vec2 v_OriXY;
out float v_Radius;

uniform float	u_Time;	// 1.f
uniform bool	u_Repeat = true;

const float PI = 3.141592;
const mat3 c_RP = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);	//	세로부터순서로들어간다!!
const vec3 c_Gravity = vec3(0, -10, 0);

void main()
{
	vec3 newPos = a_Position.xyz;	//초기위치!
	v_OriXY = a_Position.xy;
	v_Radius = abs(a_Position.x);	//보간안된 원래값을 넘김 : abs()
	vec3 newVel = a_Vel.xyz;

	float StartTime = a_StartLife.x;
	float LifeTime = a_StartLife.y;
	float ratio = a_RatioAmp.x;
	float amp = a_RatioAmp.y;
	float Alpha_Color = a_Color.w;
	
	float newTime = u_Time - StartTime;
	
	if(newTime > 0){
		//ratio = ratio * newTime * newTime;
		
		newPos.x += sin(PI * 2 * a_Value);
		newPos.y += cos(PI * 2 * a_Value);

		newTime = mod(newTime, LifeTime);
		newVel = newVel + c_Gravity * newTime;	//속도
		newPos = newPos + a_Vel * newTime + 0.5 * c_Gravity * newTime * newTime;	//위치 : 초기속도 + 중력가속도
		
		vec3 vSin = a_Vel * c_RP;

		Alpha_Color = pow(1.f - newTime/LifeTime, 2);

		//newPos = newPos + vSin * sin(newTime * PI * 2 * ratio) * amp;
	}
	else{
		newPos = vec3(10000, 10000, 10000);
	}

	gl_Position = vec4(newPos, 1);
	
	v_Color = vec4(a_Color.xyz, Alpha_Color);
}
