#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer {
public:
	// 构造析构函数
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();
	// 用给定的精灵渲染一个四边形纹理
	void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotato = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;
	GLuint quadVAO;
	// 初始化缓冲区和顶点属性
	void initRenderData();
};


#endif