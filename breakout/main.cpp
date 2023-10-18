#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "game.h"
#include "resource_manager.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	
	// ��׽���
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ����OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OpenGL����
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);								// ���ݺ���glCullFaceҪ����������ͼ�β��ϵ���
	glEnable(GL_BLEND);									// ������ɫ��ϡ�����ʵ�ְ�͸��Ч��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Դ��ɫ���������alpha ֵ��Ŀ����ɫ����1.0��ȥԴ��ɫ��alphaֵ

	// ��ʼ��game
	Breakout.Init();

	// Deltaʱ�����
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	//// ������Ϸ״̬
	//Breakout.State = GAME_ACTIVE;

	while (!glfwWindowShouldClose(window)){
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// �����û�����
		Breakout.ProcessInput(deltaTime);

		// ������Ϸ״̬
		Breakout.Update(deltaTime);
		
		// Render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Breakout.Render();
		//std::cout << Breakout.Keys[GLFW_KEY_A] << " " << Breakout.Keys[GLFW_KEY_LEFT] << std::endl;


		glfwSwapBuffers(window);
	}

	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//ESC�رճ���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			Breakout.Keys[key] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE) {
			Breakout.Keys[key] = GL_FALSE;
			Breakout.KeysProcessed[key] = GL_FALSE;
		}
	}

}