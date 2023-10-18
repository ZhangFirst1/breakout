#ifndef __POWER_UP_H__
#define __POWER_UP_H__

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"

// 提升后的挡板尺寸
const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
// 加速！
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject {
public:
	// powerup 状态
	std::string Type;
	GLfloat		Duration;
	GLboolean	Activated;
	// 构造函数
	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
		: GameObject(position, POWERUP_SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() { }
};

#endif