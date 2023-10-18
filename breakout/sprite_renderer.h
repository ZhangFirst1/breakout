#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer {
public:
	// ������������
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();
	// �ø����ľ�����Ⱦһ���ı�������
	void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotato = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;
	GLuint quadVAO;
	// ��ʼ���������Ͷ�������
	void initRenderData();
};


#endif