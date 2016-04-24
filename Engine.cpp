#include "Engine.h"
namespace {
	std::map<int, bool> inputIsDown;
	std::map<int, bool> inputWasDown;
}

void mouseClick(GLFWwindow* windowPtr, int button, int action, int mods) {
	inputIsDown[button] = action;
}

void keyCallback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
	inputIsDown[key] = action;
}

Engine::Engine()
{
	sManager = ShaderManager();
	texID = 1;
}


Engine::~Engine()
{
	glDeleteTextures(1, &texID);
}

bool Engine::init()
{

	if (glfwInit() == GL_FALSE)
		return false;

	GLFWwindowPtr = glfwCreateWindow(800, 600, "Luke Hedrick DSA1 Engine", NULL, NULL);

	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else {
		glfwTerminate();
		return false;
	}

	//Engine Initializer
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	obj["dirt"].textFileName = "textures/dirt.jpg";
	obj["dirt"].trans.loc = { 0, -.5, 0 };
	obj["dirt"].trans.rot = { 0, 0, 0 };
	obj["dirt"].trans.size = { 1, .5, .5 };
	obj["dirt"].rBody.force = { 0, 0, 0 };
	obj["dirt"].rBody.mass = 0;
	obj["dirt"].rBody.velocity = { 0, 0, 0 };
	obj["dirt"].colliderType = obj["dirt"].COLLIDERLESS;

	obj["test"].textFileName = "textures/TestTexture.png";
	obj["test"].trans.loc = { 0, 0, 0 };
	obj["test"].trans.rot = { 0, 0, 0 };
	obj["test"].trans.size = { .5, .5, .5 };
	obj["test"].rBody.force = { 0, 0, 0 };
	obj["test"].rBody.mass = 0;
	obj["test"].rBody.velocity = { 0, 0, 0 };
	obj["test"].colliderType = obj["test"].AABB;

	obj["bat"].textFileName = "textures/batman.png";
	obj["bat"].trans.loc = { .4, -.2, 0 };
	obj["bat"].trans.rot = { 0, 0, 0 };
	obj["bat"].trans.size = { .15, .15, .15 };
	obj["bat"].rBody.force = { 0, 0, 0 };
	obj["bat"].rBody.mass = .15;
	obj["bat"].rBody.velocity = { 0, 0, 0 };
	obj["bat"].colliderType = obj["bat"].AABB;

	return true;
}

bool Engine::bufferModel()
{
	std::vector <glm::vec2> uv = { { 1, 1 }, { 0, 1 } , { 0, 0 }, { 1, 0 } };
	std::vector <glm::vec3> locs = { {1, 1, 0} , { -1, 1, 0 }, {-1, -1, 0},  {1, -1, 0 } };
	
	std::vector <unsigned int> locInds = { 0, 1, 2, 0, 2, 3 };

	std::vector <unsigned int> uvInds = { 0, 1, 2, 0, 2, 3 };

	vertCount = locInds.size();

	std::vector<Vertex> vertBuffData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++) {
		vertBuffData[i].loc = locs[locInds[i]];
		vertBuffData[i].uv = uv[uvInds[i]];
	}
		

	//tell OpenGL to get indices of stored data
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuff);

	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuff);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount, &vertBuffData[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	
	return true;
}

bool Engine::gameLoop()
{	
	int count = 0;
	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyCallback);

	//Game Loop
	while (!glfwWindowShouldClose(GLFWwindowPtr)) {
		tPrev = tCurr;
		tCurr = glfwGetTime();
		tDelta = tCurr - tPrev;
		
		glClear(GL_COLOR_BUFFER_BIT);

		float epsilon = .0001;

		if (obj["bat"].rBody.velocity.x > 0) {
			applyForce(obj["bat"], obj["bat"].rBody.velocity * -1.0f);
		} else if(obj["bat"].rBody.velocity.x < 0) {
			applyForce(obj["bat"], obj["bat"].rBody.velocity * -1.0f);
		}

		if (obj["bat"].trans.loc.y > -.2 + epsilon) {
			applyForce(obj["bat"], glm::vec3({ 0, -.5, 0 }));
		}

		if (obj["bat"].trans.loc.y < -.2 - epsilon) {
			obj["bat"].trans.loc.y = -.2;
			obj["bat"].rBody.velocity.y = 0;
		}

		changeVel(obj["bat"]);
		changeLoc(obj["bat"]);
		/*
		Here is where the scale, rotation, and translate happen I commented out where the scale should go, and left in the one where it actually works
		*/

		obj["dirt"].trans.transMatrix = glm::mat4();

		obj["dirt"].trans.transMatrix *= glm::translate(obj["dirt"].trans.transMatrix, obj["dirt"].trans.loc);
		obj["dirt"].trans.transMatrix *= glm::yawPitchRoll(obj["dirt"].trans.rot.y, obj["dirt"].trans.rot.x, obj["dirt"].trans.rot.z);
		obj["dirt"].trans.transMatrix *= glm::scale(obj["dirt"].trans.transMatrix, obj["dirt"].trans.size);

		glUniformMatrix4fv(2, 1, GL_FALSE, &obj["dirt"].trans.transMatrix[0][0]);

		glBindVertexArray(vertArr);
		glBindTexture(GL_TEXTURE_2D, textures["textures/dirt.jpg"]);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);

		obj["test"].trans.transMatrix = glm::mat4();

		obj["test"].trans.transMatrix *= glm::translate(obj["test"].trans.transMatrix, obj["test"].trans.loc);
		obj["test"].trans.transMatrix *= glm::yawPitchRoll(obj["test"].trans.rot.y, obj["test"].trans.rot.x, obj["test"].trans.rot.z);
		obj["test"].trans.transMatrix *= glm::scale(obj["test"].trans.transMatrix, obj["test"].trans.size);

		glUniformMatrix4fv(2, 1, GL_FALSE, &obj["test"].trans.transMatrix[0][0]);

		glBindTexture(GL_TEXTURE_2D, textures["textures/TestTexture.png"]);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);

		obj["bat"].trans.transMatrix = glm::mat4();


		obj["bat"].trans.transMatrix *= glm::translate(obj["bat"].trans.transMatrix, obj["bat"].trans.loc);
		obj["bat"].trans.transMatrix *= glm::yawPitchRoll(obj["bat"].trans.rot.y, obj["bat"].trans.rot.x, obj["bat"].trans.rot.z);
		obj["bat"].trans.transMatrix *= glm::scale(obj["bat"].trans.transMatrix, obj["bat"].trans.size);

		glUniformMatrix4fv(2, 1, GL_FALSE, &obj["bat"].trans.transMatrix[0][0]);

		glBindTexture(GL_TEXTURE_2D, textures["textures/batman.png"]);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);

		glfwSwapBuffers(GLFWwindowPtr);

		inputWasDown = inputIsDown;

		glfwPollEvents();

		if (inputWasDown != inputIsDown) {
			if (inputIsDown[GLFW_KEY_ESCAPE]) {
				glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
			}

			if (inputIsDown[GLFW_KEY_W] && !inputWasDown[GLFW_KEY_W] && obj["bat"].trans.loc.y < .2) {
				//applyForce(obj["bat"], glm::vec3(0, 1, 0));
				obj["bat"].rBody.velocity.y = 1;
			}

			if (inputIsDown[GLFW_KEY_SPACE]) {
				glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
			}

			//changed texture when mouse clicked
			/*if (inputIsDown[GLFW_KEY_MOUSE_BUTTON_1]) {
				texID++;
				if (texID > 2) {
					texID = 1;
				}
			}*/
		}

		if (inputIsDown[GLFW_KEY_A]) {
			obj["bat"].rBody.velocity.x = -.1;
			if (obj["bat"].trans.size.x = -.15) {
				obj["bat"].trans.size.x *= -1;
			}
		}

		if (inputIsDown[GLFW_KEY_D]) {
			obj["bat"].rBody.velocity.x = 1;
			if (obj["bat"].trans.size.x = .15) {
				obj["bat"].trans.size.x *= -1;
			}
		}
		
		if (colliding(obj["test"], obj["bat"])) {
			std::cout << "obj[test] is colliding with obj[bat] for " <<  count << " frames" << std::endl;
			count++;
		}
	}

	glfwTerminate();

	return true;
}

bool Engine::useShaders()
{
	if (sManager.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl") == true) {
		glUseProgram(sManager.getProgram());
		return true;
	}

	return false;
}

bool Engine::loadTextures(const char * fileName)
{
	textures[fileName] = texID;

	FIBITMAP* imagePtr = FreeImage_ConvertTo32Bits(FreeImage_Load(FreeImage_GetFileType(fileName, 0), fileName));

	glGenTextures(textures[fileName], &textures[fileName]);
	glBindTexture(GL_TEXTURE_2D, textures[fileName]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(imagePtr), FreeImage_GetHeight(imagePtr), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(imagePtr));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	FreeImage_Unload(imagePtr);

	glBindTexture(GL_TEXTURE_2D, 0);

	texID++;

	return true;
}

void Engine::applyForce(Object & o, glm::vec3 f)
{
	o.rBody.force += f;
}

void Engine::changeVel(Object & o)
{
	o.rBody.velocity += o.rBody.force * (1 / o.rBody.mass) * tDelta;
	o.rBody.force = glm::vec3{};
}

void Engine::changeLoc(Object & o)
{
	o.trans.loc += o.rBody.velocity * tDelta;
}

bool Engine::colliding(Object & o1, Object & o2)
{
	//AABB and SPHERE
	if (o1.colliderType == o1.AABB && o2.colliderType == o2.SPHERE) {
		glm::vec3 posChecker = o1.trans.loc + o2.trans.loc;
		glm::vec3 dist = { 0, 0, 0 };
		if (posChecker.x > 0) {
			dist += 0;
		}

		dist = (o2.trans.loc * o2.trans.loc) + (o1.trans.loc * o1.trans.loc);

		if (dist.x -  (o1.trans.size.x / 2) <= (o1.trans.size.x / 2)) {
			return true;
		}
	}
	//AABB only
	if (o1.colliderType == o1.AABB && o2.colliderType == o2.AABB) {
		if ((o1.trans.loc.x + o1.trans.size.x / 2) - (o2.trans.loc.x - o2.trans.size.x /2) >= 0) {
			return true;
		}
	}

	//Sphere only

	return false;
}


