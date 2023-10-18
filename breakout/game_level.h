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
	// �ȼ�״̬
	std::vector<GameObject> Bricks;
	// ���캯��
	GameLevel() {}
	// ���ļ��м�����Ϸ
	void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	// ��Ⱦ
	void Draw(SpriteRenderer& renderer);
	// �����Ϸ״̬���Ƿ����пɴݻٵ�ש�鶼���ݻ٣�
	GLboolean IsCompleted();
private:
	// ��ʼ��
	void Init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif