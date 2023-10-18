#ifndef __POST_PROCESSOR_H__
#define __POST_PROCESSOR_H__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"

class PostProcessor {
public:
	// ״̬
	Shader PostProcessingShader;
	Texture2D Texture;
	GLuint Width, Height;
	// ѡ��
	GLboolean Confuse, Chaos, Shake;
	// ���캯��
	PostProcessor(Shader shader, GLuint width, GLuint height);
	// ����Ⱦ��Ϸ֮ǰ׼�����ڴ�������֡�������
	void BeginRender();
	// ��Ⱦ��������� ����Ⱦ���ݴ����������
	void EndRender();
	// ��ȾPostProcessor�����ı���(��Ϊһ��������Ļ�Ĵ���)
	void Render(GLfloat time);
private:
	// ��Ⱦ״̬
	GLuint MSFBO, FBO;	
	GLuint RBO;
	GLuint VAO;
	// ��ʼ��
	void initRenderData();
};

#endif