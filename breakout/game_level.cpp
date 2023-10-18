#include "game_level.h"

#include <fstream>
#include <sstream>

void GameLevel::Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight) {
	// ���������
	this->Bricks.clear();
	// ���ļ��м���
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;
	if (fstream) {
		while (std::getline(fstream, line)) {	// ��ȡÿ��
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode) 		// ��ȡÿ��
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->Init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& renderer) {
	for (GameObject& tile : this->Bricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
}
// �ж���Ϸ�Ƿ��ʤ
GLboolean GameLevel::IsCompleted() {
	for (GameObject& tile : this->Bricks)
		if (!tile.IsSolid && !tile.Destroyed)
			return GL_FALSE;
	return GL_TRUE;
}

/*
	0 ��ש��
	1 ���ɴݻٵ�ש��
	>1 �ɴݻ� ���ִ�����ɫ
*/

void GameLevel::Init(std::vector<std::vector<GLuint>> tileData, GLuint levelwidth, GLuint levelheight) {
	// ������
	GLuint heigth = tileData.size();
	GLuint width = tileData[0].size();
	GLfloat unit_width = levelwidth / static_cast<GLfloat>(width), unit_height = levelheight / static_cast<GLfloat>(heigth);

	for (GLuint x = 0; x < heigth; x++) {
		for (GLuint y = 0; y < width; y++) {
			if (tileData[x][y] == 1) {
				glm::vec2 pos(y * unit_width, x * unit_height);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = GL_TRUE;
				this->Bricks.push_back(obj);
			}
			else if (tileData[x][y] > 1) {
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[x][y] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[x][y] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[x][y] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[x][y] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(y * unit_width, x * unit_height);
				glm::vec2 size(unit_width, unit_height);
				this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
			}
		}
	}
	
}