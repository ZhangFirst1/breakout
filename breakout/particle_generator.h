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
	// ���캯��
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	// ������������
	void Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// ��Ⱦ����
	void Draw();
private:
	// ״̬
	std::vector<Particle> particles;
	GLuint amount;
	// ��Ⱦ״̬
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	// ��ʼ��
	void init();
	// ���ص�һ���������ӵ��±�
	GLuint firstUnusedParticle();
	// ��������
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif