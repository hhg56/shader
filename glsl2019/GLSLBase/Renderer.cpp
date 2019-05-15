#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_solidSampleShader = CompileShaders("./Shaders/my_SolidRect_190325.vs", "./Shaders/my_SolidRect_190325.fs");
	m_solidSinShader = CompileShaders("./Shaders/my_sin_trail.vs", "./Shaders/my_sin_trail.fs");
	m_solidCircleShader = CompileShaders("./Shaders/SolidRect_190410.vs", "./Shaders/SolidRect_190410.fs");
	m_solidFillAllShader = CompileShaders("./Shaders/Fill_All.vs", "./Shaders/Fill_All.fs");
	m_solidTextureShader = CompileShaders("./Shaders/Texture.vs", "./Shaders/Texture.fs");
	m_solidDrawNumberShader = CompileShaders("./Shaders/Drawnumber.vs", "./Shaders/Drawnumber.fs");
	
	m_Texture_1 = CreatePngTexture("./Texture/Explosion.png");
	m_Texture_2 = CreatePngTexture("./Texture/pororo.png");
	m_Texture_3 = CreatePngTexture("./Texture/rgb.png");
	m_Texture_test[0] = CreatePngTexture("./Texture/numbers.png");	// number

	//Create VBOs
	CreateVertexBufferObjects();
	CreateProxyGeometry();			// grid
}

void Renderer::CreateVertexBufferObjects()
{
	float size = 0.02f;
	float rect[]
		=
	{
		-size, -size, 0.f, 0.5,
		-size, size, 0.f, 0.5,
		size, size, 0.f, 0.5, //Triangle1
		-size, -size, 0.f, 0.5,
		size, size, 0.f, 0.5,
		size, -size, 0.f, 0.5, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float color[]
		=
	{
		1 , 0 , 1, 0,	//rgba
		1 , 0 , 1, 0,
		1 , 0 , 1, 0,
		1 , 0 , 1, 0,
		1 , 0 , 1, 0,
		1 , 0 , 1, 0,
	};

	glGenBuffers(1, &m_VBORectColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	
	//Lecture2
	float tri[] =
	{
		-0.01, -0.01, 0.f, -0.01, 0.01, 0.f, 0.01, 0.01 , 0.f, //Triangle1
	};
	glGenBuffers(1, &m_VBOTri);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
	
	//Lecture10
	float Csize = 1.0f;
	float Circle[] =
	{
		-Csize, -Csize, 0.f,	0.f, 0.f,	// x, y, z, uv.x, uv.y
		-Csize, Csize, 0.f,		0.f, 1.f,
		Csize, Csize, 0.f,		1.f, 1.f,	// Triangle1
		Csize, Csize, 0.f,		1.f, 1.f,
		-Csize, -Csize, 0.f,	0.f, 0.f,
		Csize, -Csize, 0.f,		1.f, 0.f,	// Triangle2
	};
	glGenBuffers(1, &m_VBOCircle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOCircle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Circle), Circle, GL_STATIC_DRAW);

	static const GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};
	glGenTextures(1, &gTextureID);
	glBindTexture(GL_TEXTURE_2D, gTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	GenQuadsVBO(10000, false, &m_VBOQuads, 0);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp);	// GL_RGBA -> GL_RGB	: BMP이미지는 A값이 없엉

	return temp;
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	
	if (test_count >= 1.f) test_count = 0.f;
	else test_count += 0.01f;

	glUniform1f(uTime, test_count);

	GLint id0 = glGetAttribLocation(m_SolidRectShader, "a_Position");
	GLint id1 = glGetAttribLocation(m_SolidRectShader, "a_Color");

	//int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(id0);	// 동일한 셰이더에서 쓸때도있고 안쓸때도있으므로
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(id0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);		// DirectX에서 루트시그니쳐와 같음
	// 3개씩 끊어서 하나의 버텍스를 구성하라.						다음것을 읽으려면 각 float포인터 형태로 * 3개 이후에 것을 읽어라

	glEnableVertexAttribArray(id1);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(id1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 8);

	glDisableVertexAttribArray(id0);
	glDisableVertexAttribArray(id1);
}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ProxyGeo); 	// m_VBOTri
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_LINE_STRIP, 0, m_Count_ProxyGeo);	//GL_TRIANGLES

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture04()
{
	glUseProgram(m_solidSampleShader);
	
	GLuint uTime = glGetUniformLocation(m_solidSampleShader, "u_Time");
	test_count += 0.001f;
	//if (test_count >= 1.f) test_count = 0.f;
	//else test_count += 0.001f;
	glUniform1f(uTime, test_count);
	
	GLuint aPos = glGetAttribLocation(m_solidSampleShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_solidSampleShader, "a_Vel");
	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aVel);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	// 원래는 float으로 3개씩 읽었지만 xyz읽고 vx,vy,vz가 추가되어 6개를 뛰어 다음 xyz를 읽어야한다.
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float)*3));
	// 최초시작지점이 aPos다음 3자리가시작이므로 바꿔야하므로 float*3만큼 밀어줘야한다.

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);
	//glDrawArrays(GL_LINE_STRIP, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
}

void Renderer::Lecture05()
{
	glUseProgram(m_solidSampleShader);

	GLuint uTime = glGetUniformLocation(m_solidSampleShader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	test_count += 0.001f;
	glUniform1f(uTime, test_count);

	GLuint aPos = glGetAttribLocation(m_solidSampleShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_solidSampleShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_solidSampleShader, "a_StartLife");

	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	// 원래는 float으로 3개씩 읽었지만 xyz읽고 vx,vy,vz가 추가되어 6개를 뛰어 다음 xyz를 읽어야한다.
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	// 최초시작지점이 aPos다음 3자리가시작이므로 바꿔야하므로 float*3만큼 밀어줘야한다.
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);
	//glDrawArrays(GL_LINE_STRIP, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);
}

void Renderer::Lecture06()
{
	glUseProgram(m_solidSampleShader);

	GLuint uTime = glGetUniformLocation(m_solidSampleShader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	test_count += 0.001f;
	glUniform1f(uTime, test_count);

	GLuint aPos = glGetAttribLocation(m_solidSampleShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_solidSampleShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_solidSampleShader, "a_StartLife");
	
	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);
}

void Renderer::Lecture07()
{
	glUseProgram(m_solidSinShader);

	GLuint uTime = glGetUniformLocation(m_solidSinShader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	test_count += 0.001f;
	glUniform1f(uTime, test_count);

	GLuint aPos = glGetAttribLocation(m_solidSinShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_solidSinShader, "a_Vel");
	GLuint aTmp = glGetAttribLocation(m_solidSinShader, "a_StartLife");

	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aTmp);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aTmp, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aTmp);
}

void Renderer::Lecture08()
{
	glUseProgram(m_solidSinShader);

	GLuint uTime = glGetUniformLocation(m_solidSinShader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	test_count += 0.0005f;
	glUniform1f(uTime, test_count);

	GLuint aPos = glGetAttribLocation(m_solidSinShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_solidSinShader, "a_Vel");
	GLuint aTmp = glGetAttribLocation(m_solidSinShader, "a_StartLife");
	GLuint aRat = glGetAttribLocation(m_solidSinShader, "a_RatioAmp");
	GLuint aVal = glGetAttribLocation(m_solidSinShader, "a_Value");

	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aTmp);
	glEnableVertexAttribArray(aRat);
	glEnableVertexAttribArray(aVal);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aTmp, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aRat, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 8));
	glVertexAttribPointer(aVal, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 10));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aTmp);
	glDisableVertexAttribArray(aRat);
	glDisableVertexAttribArray(aVal);
}

void Renderer::Lecture09()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_solidSinShader);
	
	GLuint uTime = glGetUniformLocation(m_solidSinShader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	test_count += 0.0005f;
	glUniform1f(uTime, test_count);

	int uTex = glGetUniformLocation(m_solidSinShader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture_1);

	GLuint aPos = glGetAttribLocation(m_solidSinShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_solidSinShader, "a_Vel");
	GLuint aTmp = glGetAttribLocation(m_solidSinShader, "a_StartLife");
	GLuint aRat = glGetAttribLocation(m_solidSinShader, "a_RatioAmp");
	GLuint aVal = glGetAttribLocation(m_solidSinShader, "a_Value");
	GLuint aCol = glGetAttribLocation(m_solidSinShader, "a_Color");

	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aTmp);
	glEnableVertexAttribArray(aRat);
	glEnableVertexAttribArray(aVal);
	glEnableVertexAttribArray(aCol);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aTmp, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aRat, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 8));
	glVertexAttribPointer(aVal, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(aCol, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aTmp);
	glDisableVertexAttribArray(aRat);
	glDisableVertexAttribArray(aVal);
	glDisableVertexAttribArray(aCol);
}

void Renderer::Lecture10()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glUseProgram(m_solidCircleShader);

	float points[] = { 0.f, 0.f, 0.2f, 0.2f, -0.5f, -0.5f, 1.f, 1.f, -1.f, -1.f, };

	GLuint uPoints = glGetUniformLocation(m_solidCircleShader, "u_Points");
	glUniform2fv(uPoints, 5, points);

	GLuint uTime = glGetUniformLocation(m_solidCircleShader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	test_count += 0.0005f;
	glUniform1f(uTime, test_count);

	GLuint aPos = glGetAttribLocation(m_solidCircleShader, "a_Position");
	GLuint aUv = glGetAttribLocation(m_solidCircleShader, "a_Uv");
	
	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aUv);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOCircle);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUv, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aUv);
}

void Renderer::DrawTextureRect()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint shader = m_solidTextureShader;

	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	test_count += 0.0005f;
	glUniform1f(uTime, test_count);

	int uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	int uTex1 = glGetUniformLocation(shader, "u_Texture_1");
	glUniform1i(uTex1, 1);
	int uTex2 = glGetUniformLocation(shader, "u_Texture_2");
	glUniform1i(uTex2, 2);
	int uTex3 = glGetUniformLocation(shader, "u_Texture_3");
	glUniform1i(uTex3, 3);
	int uTex4 = glGetUniformLocation(shader, "u_Texture_test");
	glUniform1i(uTex4, 4);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTextureID);	// gTextureID, m_Texture_1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Texture_1);	// gTextureID, m_Texture_1
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_Texture_2);	// gTextureID, m_Texture_1
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_Texture_3);	// gTextureID, m_Texture_1
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_Texture_test[0]);	// gTextureID, m_Texture_1

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Texture");

	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOCircle);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
}

void Renderer::DrawNumber(int *number)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint shader = m_solidDrawNumberShader;

	glUseProgram(shader);

	// Vertex Setting
	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_Texture");
	glEnableVertexAttribArray(aPos);	//  기말에도 낸다.
	glEnableVertexAttribArray(aTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOCircle);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	// texture Setting
	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture_test[0]);

	// Time Setting
	GLuint uTime = glGetUniformLocation(shader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	if (test_count >= 9) test_count = 0;
	else test_count += 0.01f;
	glUniform1f(uTime, test_count);
		
	GLuint u_Number = glGetUniformLocation(shader, "u_Number");	//Uniform과Attirute값 구분 필수!!
	glUniform1iv(u_Number, 3, number);
	
	// Draw Here
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Restore
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
}

void Renderer::FillAll()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_solidFillAllShader);

	float points[] = { 0.f, 0.f, 0.5f, 0.5f, 0.3f, 0.3f, 0.7f, 0.7f, 1.f, 1.f, };

	GLuint uPoints = glGetUniformLocation(m_solidFillAllShader, "u_Points");
	glUniform2fv(uPoints, 5, points);

	GLuint uTime = glGetUniformLocation(m_solidFillAllShader, "u_Time");	//Uniform과Attirute값 구분 필수!!
	test_count += 0.0005f;
	glUniform1f(uTime, test_count);

	GLuint aPos = glGetAttribLocation(m_solidFillAllShader, "a_Position");
	GLuint aUv = glGetAttribLocation(m_solidFillAllShader, "a_Uv");

	glEnableVertexAttribArray(aPos);	// 시험 get해서 넘어오는 ip
	glEnableVertexAttribArray(aUv);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOCircle);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aUv, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aUv);

	glDisable(GL_BLEND);
}

void Renderer::GenQuads()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_VertexCount);

	glDisableVertexAttribArray(0);
}

void Renderer::GenQuadsVBO(int count, bool what, GLuint* id, GLuint y)
{
	float randX, randY;
	float size = 0.04f;
	int CountQuad = count;
	int verticesPerQuad = 6;
	int floatsPerVertex = 3 + 3 + 2 + 2 + 1 + 4;	// Pos + Vel + Time + ratio/amplitude + value
	float arraySize = CountQuad * floatsPerVertex * verticesPerQuad;
	float *vertices = new float[arraySize];
	for (int i = 0; i < count; i++)
	{
		float rnadColR, rnadColG, rnadColB, rnadColA;
		float randVelX, randVelY, randVelZ;
		float StartTime, LifeTime;
		float ratio, amplitude;
		float Value;
		float StartTimeMax = 6.f;
		float LifeTimeMax = 2.f;
		float VelXMax = 1.f;
		float VelYMax = 1.f;
		float ratioMax = 4.f;	// 진폭
		float ampMax = -0.1f;	// 주기
		float ValueMax = 1.f;

		int index = i * floatsPerVertex * verticesPerQuad;

		if (what == true) {
			randX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
			randY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		}
		else {
			randY = 0.f;		// 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
			randX = 0.f;		// 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		}

		randVelX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f) * VelXMax;
		randVelY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f) * VelYMax;
		randVelZ = 0.f;

		StartTime = ((float)rand() / (float)RAND_MAX) * StartTimeMax;
		LifeTime = ((float)rand() / (float)RAND_MAX) * LifeTimeMax;

		ratio = ((float)rand() / (float)RAND_MAX) * ratioMax;
		amplitude = ((float)rand() / (float)RAND_MAX) * ampMax;

		Value = ((float)rand() / (float)RAND_MAX) * ValueMax;

		rnadColR = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		rnadColG = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		rnadColB = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		rnadColA = 1.f;

		vertices[index] = randX - size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = LifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amplitude; ++index;
		vertices[index] = Value; ++index;
		vertices[index] = rnadColR; ++index;
		vertices[index] = rnadColG; ++index;
		vertices[index] = rnadColB; ++index;
		vertices[index] = rnadColA; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = LifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amplitude; ++index;
		vertices[index] = Value; ++index;
		vertices[index] = rnadColR; ++index;
		vertices[index] = rnadColG; ++index;
		vertices[index] = rnadColB; ++index;
		vertices[index] = rnadColA; ++index;

		vertices[index] = randX - size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = LifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amplitude; ++index;
		vertices[index] = Value; ++index;
		vertices[index] = rnadColR; ++index;
		vertices[index] = rnadColG; ++index;
		vertices[index] = rnadColB; ++index;
		vertices[index] = rnadColA; ++index;
		
		vertices[index] = randX - size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = LifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amplitude; ++index;
		vertices[index] = Value; ++index;
		vertices[index] = rnadColR; ++index;
		vertices[index] = rnadColG; ++index;
		vertices[index] = rnadColB; ++index;
		vertices[index] = rnadColA; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY + size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = LifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amplitude; ++index;
		vertices[index] = Value; ++index;
		vertices[index] = rnadColR; ++index;
		vertices[index] = rnadColG; ++index;
		vertices[index] = rnadColB; ++index;
		vertices[index] = rnadColA; ++index;

		vertices[index] = randX + size; ++index;
		vertices[index] = randY - size; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVelX; ++index;
		vertices[index] = randVelY; ++index;
		vertices[index] = randVelZ; ++index;
		vertices[index] = StartTime; ++index;
		vertices[index] = LifeTime; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amplitude; ++index;
		vertices[index] = Value; ++index;
		vertices[index] = rnadColR; ++index;
		vertices[index] = rnadColG; ++index;
		vertices[index] = rnadColB; ++index;
		vertices[index] = rnadColA;
	}

	GLuint vboID = 0;

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);	//m_VBOQuads
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW);
	m_VBOQuads_VertexCount = count * verticesPerQuad;

	*id = vboID;
}

void Renderer::CreateProxyGeometry()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_Count_ProxyGeo = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_VBO_ProxyGeo);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ProxyGeo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}
