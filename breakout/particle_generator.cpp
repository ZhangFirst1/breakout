#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
	: shader(shader), texture(texture), amount(amount) {
	this->init();
}

void ParticleGenerator::init() {
	GLuint VBO;
    GLfloat particle_quad[] = {
       0.0f, 1.0f, 0.0f, 1.0f,
       1.0f, 0.0f, 1.0f, 0.0f,
       0.0f, 0.0f, 0.0f, 0.0f,

       0.0f, 1.0f, 0.0f, 1.0f,
       1.0f, 1.0f, 1.0f, 1.0f,
       1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    // 创建粒子
    for (GLuint i = 0; i < this->amount; i++)
        this->particles.push_back(Particle());
}

void ParticleGenerator::Draw() {
    // 使用添加的混合给它一个“发光”的效果
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.Use();
    for (Particle &particle : this->particles) {
        if (particle.Life > 0.0f) {
            this->shader.SetVector2f("offset", particle.Position);
            this->shader.SetVector4f("color", particle.Color);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // 重置为默认模式
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle() {
    // 下一个消亡的粒子索引总是在上一个的右边
    for (GLuint i = lastUsedParticle; i < amount; ++i) {
        if (particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // 若没找到 则从头开始找
    for (GLuint i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // 再没找到 设为第一个粒子，此时说明粒子生命周期过长，需考虑减小，或是没有保留足够的粒子
    lastUsedParticle = 0;
    return 0;
}

// 随机生成粒子
void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
    // 位置和颜色随机，速度与小球速度成正比
    GLfloat random = ((rand() % 100) - 50) / 10.0f;
    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.Position = object.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}

void ParticleGenerator::Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset) {
    // 每帧添家newParticle个新粒子
    for (GLuint i = 0; i < newParticles; i++) {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, offset);
    }
    // 更新所有粒子
    for (GLuint i = 0; i < this->amount; i++) {
        Particle& p = this->particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f) {
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5;
        }
    }
}