#ifndef __GAME_H__
#define __GAME_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"
#include "power_up.h"

// ��Ϸ״̬
enum GameState{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// ����
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

// ��ʼ����������
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);

// ��ʼ��������
const glm::vec2 BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;

// ���������л�����Ϸ�г��ֵĶ���
class Game {
public:
	// game state
	GameState	State;
	GLboolean	Keys[1024];
	GLuint		Width, Height;
	std::vector<GameLevel>  Levels;
	std::vector<PowerUp>	PowerUps;
	GLuint		Level;
	GLuint		Lives;

	// constructor / Destructor
	Game(GLuint width, GLuint height);
	~Game();

	// initialize all game states (load all shader / texture/ levels)
	void Init();

	// reset
	void ResetLevel();
	void ResetPlayer();

	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	void Docollisions();

	// PowerUp
	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(GLfloat dt);

	GLboolean KeysProcessed[1024];
};

#endif	// __GAME_H_
