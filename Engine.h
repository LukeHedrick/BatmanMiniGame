#pragma once
#include "ShaderManager.h"
#include <map>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>
#include <math.h>
#define GLFW_KEY_ESCAPE 256
#define GLFW_KEY_SPACE 32
#define GLFW_KEY_W 87
#define GLFW_KEY_A 65
#define GLFW_KEY_D 68
#define GLFW_KEY_MOUSE_BUTTON_1 0


struct Vertex{
	glm::vec3 loc;
	glm::vec2 uv;
};

struct Transform {
	glm::vec3 loc;
	glm::vec3 rot;
	glm::vec3 size;
	glm::mat4 transMatrix;
};

struct RigidBody {
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
};

struct Object {
	Transform trans;
	const char* textFileName;
	RigidBody rBody;
	enum collider { COLLIDERLESS, AABB, SPHERE };
	int colliderType;
};

class Engine
{
public:
	Engine();
	~Engine();
	bool init();
	bool bufferModel();
	bool gameLoop();
	bool useShaders();
	bool loadTextures(const char* fileName);
	void applyForce(Object& o, glm::vec3 f);
	void changeVel(Object& o);
	void changeLoc(Object& o);
	bool colliding(Object& o1, Object& o2);
private:
	GLuint vertArr, vertBuff;
	unsigned int vertCount;
	GLFWwindow* GLFWwindowPtr;
	ShaderManager sManager;
	unsigned int texID;
	map <const char*, unsigned int> textures;
	map <const char*, Object> obj;
	float tDelta, tCurr, tPrev;
	glm::vec3 nForce;
};