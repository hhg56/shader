#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

#include <time.h>

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void Lecture2();
	void Lecture04();
	void Lecture05();
	void Lecture06();
	void Lecture07();
	void Lecture08();	// 190403
	void Lecture09();	// 190408
	void Lecture10();	// 190410
	void DrawTextureRect();	// 190417
	void DrawNumber(int* number);		// 190515
	void FillAll();
	void GenQuadsVBO(int count, bool what, GLuint* x, GLuint y);
	void GenQuads();
	void CreateProxyGeometry();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectColor = 0;
	GLuint m_VBOCircle = 0;
	GLuint m_VBOTri = 0;
	GLuint m_VBOQuads = 0;
	GLuint m_VBO_ProxyGeo = 0;
	GLuint m_Count_ProxyGeo = 0;
	
	float test_count = 0;
	
	unsigned int m_VBOQuads_VertexCount;

	//Shaders
	GLuint m_SolidRectShader = 0;
	GLuint m_solidSampleShader = 0;
	GLuint m_solidSinShader = 0;
	GLuint m_solidCircleShader = 0;
	GLuint m_solidFillAllShader = 0;
	GLuint m_solidTextureShader = 0;
	GLuint m_solidDrawNumberShader = 0;
	
	//Texture
	GLuint m_Texture_1 = 0;
	GLuint m_Texture_2 = 0;
	GLuint m_Texture_3 = 0;
	GLuint m_Texture_test[1];

	GLuint gTextureID = 0;
};

