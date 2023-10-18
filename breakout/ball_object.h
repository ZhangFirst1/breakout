#ifndef __BALL_OBJECT_H__
#define __BALL_OBJECT_H___

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"
#include "sprite_renderer.h"

class BallObject : public GameObject {
public:
	// 小球状态
	GLfloat Radius;
	GLboolean Stuck;
	GLboolean Sticky, PassThrough;
	// 构造函数
	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	// 移动小球
	glm::vec2 Move(GLfloat dt, GLuint window_width);
	// 重置状态
	void Reset(glm::vec2 positon, glm::vec2 velocity);
};

#endif