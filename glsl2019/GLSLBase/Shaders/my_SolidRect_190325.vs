//���� ���� 450->440
#version 440

in vec3			a_Position;
in vec3			a_Vel;
in vec2			a_StartLife;	//ù��°���� : ���۽ð�, �ι�°���� : �����ֱ�

uniform float	u_Time;	// 1.f
uniform bool	u_Repeat = true;

const vec3 c_Gravity = vec3(0, -5, 0);

void main()
{
	vec3 newPos = a_Position.xyz;	//�ʱ���ġ!

	float StartTime = a_StartLife.x;
	float LifeTime = a_StartLife.y;
		
	float newTime = u_Time - StartTime;	// fract : 0~1 repeat..

	if(newTime > 0){
		//newPos += a_Vel * newTime + 0.3 * c_Gravity * newTime * newTime;		//��ġ = ����ġ + �ӵ� * �ð� + �߷°��ӵ�(�ð�^2)
		
		float life = newTime;
		float remainingLife = LifeTime - life;

		if(u_Repeat == true)
		{
			remainingLife = 1.f;
			newTime = mod(newTime, LifeTime);	// mod : �ݺ�
		}

		if(remainingLife < 0) newPos = vec3(10000, 10000, 10000);
		else newPos += a_Vel * newTime + 0.3 * c_Gravity * newTime * newTime;

		//if(newTime > LifeTime) newPos = vec3(10000, 10000, 10000);
	}
	else{
		newPos = vec3(10000, 10000, 10000);
	}

	gl_Position = vec4(newPos, 1);
}
