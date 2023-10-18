#ifndef __PARTICLE_GENERATOR_H__
#define __PARTICLE_GENERATOR_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "texture.h" 
#include "shader.h"
#include "game_object.h"

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

class ParticleGenerator {
public:
	// 构造函数
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	// 更新所有粒子
	void Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// 渲染粒子
	void Draw();
private:
	// 状态
	std::vector<Particle> particles;
	GLuint amount;
	// 渲染状态
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	// 初始化
	void init();
	// 返回第一个消亡粒子的下标
	GLuint firstUnusedParticle();
	// 粒子重生
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif