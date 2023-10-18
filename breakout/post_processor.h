#ifndef __POST_PROCESSOR_H__
#define __POST_PROCESSOR_H__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"

class PostProcessor {
public:
	// 状态
	Shader PostProcessingShader;
	Texture2D Texture;
	GLuint Width, Height;
	// 选项
	GLboolean Confuse, Chaos, Shake;
	// 构造函数
	PostProcessor(Shader shader, GLuint width, GLuint height);
	// 在渲染游戏之前准备后期处理器的帧缓冲操作
	void BeginRender();
	// 渲染场景后调用 将渲染数据存入纹理对象
	void EndRender();
	// 渲染PostProcessor纹理四边形(作为一个覆盖屏幕的大精灵)
	void Render(GLfloat time);
private:
	// 渲染状态
	GLuint MSFBO, FBO;	
	GLuint RBO;
	GLuint VAO;
	// 初始化
	void initRenderData();
};

#endif