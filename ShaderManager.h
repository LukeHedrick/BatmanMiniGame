#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <FreeImage.h>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class ShaderManager
{
private:
	GLuint program;
public:
	GLuint getProgram() const;
	ShaderManager();
	~ShaderManager();
	bool loadShaders(const char* vertexFile, const char* fragmentFile);
	GLuint loadShader(const char* file, GLenum shaderFile);
};

