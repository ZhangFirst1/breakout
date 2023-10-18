#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

class GameLevel {
public:
	// 等级状态
	std::vector<GameObject> Bricks;
	// 构造函数
	GameLevel() {}
	// 从文件中加载游戏
	void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	// 渲染
	void Draw(SpriteRenderer& renderer);
	// 检查游戏状态（是否所有可摧毁的砖块都被摧毁）
	GLboolean IsCompleted();
private:
	// 初始化
	void Init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif