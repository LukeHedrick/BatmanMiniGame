#include "ShaderManager.h"

GLuint ShaderManager::getProgram() const
{
	return program;
}

ShaderManager::ShaderManager()
{
	program = 0;
}


ShaderManager::~ShaderManager()
{
}

bool ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile)
{
	GLuint vShader = loadShader(vertexFile, GL_VERTEX_SHADER);

	if (vShader == 0) {
		return 0;
	}

	GLuint fShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	if (fShader == 0) {
		return 0;
	}

	program = glCreateProgram();

	glAttachShader(program, vShader);

	glAttachShader(program, fShader);

	glLinkProgram(program);

	GLint results;
	glGetProgramiv(program, GL_LINK_STATUS, &results);

	if (results != 0) {
		return true;
	}

	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	GLchar* data = new GLchar[logLength];

	glGetProgramInfoLog(program, logLength, 0, data);

	cout << data;

	glDeleteProgram(program);

	delete[] data;

	return false;
}

GLuint ShaderManager::loadShader(const char * file, GLenum shaderFile)
{
	ifstream inFile;
	char* fileContents;

	inFile.open(file, ios::binary);

	if (inFile.is_open()) {
		inFile.seekg(0, ios::end);
		int length = (int)inFile.tellg();

		inFile.seekg(0, ios::beg);

		fileContents = new char[length + 1];

		inFile.read(fileContents, length);
		fileContents[length] = 0;

		inFile.close();
	}
	else {
		cout << "Error opening file. Closing file." << endl;
		return 0;
	}

	GLuint shader = glCreateShader(shaderFile);

	glShaderSource(shader, 1, &fileContents, 0);

	glCompileShader(shader);

	delete[] fileContents;

	GLint results;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &results);

	if (results != 0) {
		return shader;
	}

	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	GLchar* data = new GLchar[logLength];

	glGetShaderInfoLog(shader, logLength, 0, data);

	cout << data;

	glDeleteShader(shader);

	delete[] data;

	return 0;
}
