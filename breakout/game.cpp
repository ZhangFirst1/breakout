#include <iostream>
#include <irrKlang.h>
#include <sstream>

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "text_renderer.h"

using namespace irrklang;

SpriteRenderer*		Renderer;
GameObject*			Player;
BallObject*			Ball;
ParticleGenerator*	Particles;
PostProcessor*		Effects;
ISoundEngine*		SoundEngine = createIrrKlangDevice();
GLfloat				ShakeTime = 0.0f;
TextRenderer*       Text;

Game::Game(GLuint width, GLuint height)
	: State(GAME_MENU), Keys(), Width(width), Height(height), Level(0), Lives(3)
{

}

Game::~Game() {
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	SoundEngine->drop();
}

void Game::Init() {
	// 加载音乐
	SoundEngine->play2D("music/breakout.mp3", GL_TRUE);
	// 加载着色器
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
	ResourceManager::LoadShader("shaders/post_processing.vs", "shaders/post_processing.fs", nullptr, "postprocessing");
	// 配置着色器
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	// 加载纹理
	ResourceManager::LoadTexture("resources/textures/basketball.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("resources/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("resources/textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("resources/textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("resources/textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("resources/textures/particle.png", GL_TRUE, "particle");

	ResourceManager::LoadTexture("resources/textures/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceManager::LoadTexture("resources/textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
	ResourceManager::LoadTexture("resources/textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceManager::LoadTexture("resources/textures/powerup_increase.png", GL_TRUE, "powerup_increase");
	ResourceManager::LoadTexture("resources/textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");
	ResourceManager::LoadTexture("resources/textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
	// 设置用于渲染的控制
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	// 加载关卡
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	// 设置挡板属性
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	// 设置小球属性
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, BALL_VELOCITY, ResourceManager::GetTexture("face"));
	// 设置字体
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("fonts/OCRAEXT.TTF", 24);

	//Effects->Shake = GL_TRUE;
	//Effects->Chaos = GL_TRUE;
	//Effects->Confuse = GL_TRUE;
}

void Game::Update(GLfloat dt) {
	// 更新小球
	Ball->Move(dt, this->Width);
	// 碰撞检测
	this->Docollisions();
	// 更新粒子
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
	// 更新道具状态
	this->UpdatePowerUps(dt);
	// 减少震动时间
	if (ShakeTime > 0.0f) {
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = GL_FALSE;
	}
	// 游戏结束
	if (Ball->Position.y >= this->Height) {
		this->Lives--;
		if (this->Lives == 0) {
			this->ResetLevel();
			this->State = GAME_MENU;
		}
		this->ResetPlayer();
	}
	// 获胜
	if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Chaos = GL_TRUE;
		this->State = GAME_WIN;
	}

}

void Game::ProcessInput(GLfloat dt) {
	// 选择关卡
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;
			this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
		}
	}

	if (this->State == GAME_ACTIVE) {
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// 移动挡板
		if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT]) {
			if (Player->Position.x >= 0) {
				Player->Position.x -= velocity;
				if (Ball->Stuck)	// 球跟随挡板移动
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT]) {
			if (Player->Position.x <= this->Width - Player->Size.x) {
				Player->Position.x += velocity;
				if (Ball->Stuck)	// 球跟随挡板移动
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
		
	}

	if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			Effects->Chaos = GL_FALSE;
			this->State = GAME_MENU;
		}
	}
}

void Game::ResetLevel() {
	if (this->Level == 0)
		this->Levels[0].Load("levels/one.lvl", this->Width, this->Height / 2);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", this->Width, this->Height / 2);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", this->Width, this->Height / 2);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.lvl", this->Width, this->Height / 2);
	
	this->Lives = 3;
}

void Game::ResetPlayer() {
	// 重置游戏状态
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2.0f - Player->Size.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), BALL_VELOCITY);

	Player->Color = glm::vec3(1.0f);
	Ball->Color = glm::vec3(1.0f);
}

void Game::Render() {
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN)
	{
		// 向后期处理缓冲区渲染
		Effects->BeginRender();
			// 绘制背景
			Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
			// 绘制关卡
			this->Levels[this->Level].Draw(*Renderer);
			// 绘制挡板
			Player->Draw(*Renderer);
			// 绘制粒子(在球之前 其他物体之后)
			Particles->Draw();	
			// 绘制道具
			for (PowerUp& powerUp : this->PowerUps) {
				if (!powerUp.Destroyed)
					powerUp.Draw(*Renderer);
			}
			// 绘制小球
			Ball->Draw(*Renderer);
		// 结束向后期处理缓冲区渲染
		Effects->EndRender();
		// 渲染后期处理四边形
		Effects->Render(glfwGetTime());

		std::stringstream ss; ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	if (this->State == GAME_MENU)
	{
		Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
	}
	if (this->State == GAME_WIN)
	{
		Text->RenderText(
			"You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
		);
		Text->RenderText(
			"Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
		);
	}

}

// powerups
GLboolean ShouldSpawn(GLuint chance) {
	GLuint random = rand() % chance;
	return random == 0;
}
void Game::SpawnPowerUps(GameObject& block)
{
	if (ShouldSpawn(60)) // 概率
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
	if (ShouldSpawn(60))
		this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
	if (ShouldSpawn(60)) 
		this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
	if (ShouldSpawn(60))
		this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
	if (ShouldSpawn(40)) // 负面道具被更频繁地生成
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
	if (ShouldSpawn(40))
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void ActivatePowerUp(PowerUp& powerUp)
{
	// 根据道具类型发动道具
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = GL_TRUE;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->PassThrough = GL_TRUE;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = GL_TRUE; // 只在chaos未激活时生效，chaos同理
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = GL_TRUE;
	}
}

// 是否有其他道具正在使用
GLboolean isOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
	for (const PowerUp& powerUp : powerUps)
	{
		if (powerUp.Activated)
			if (powerUp.Type == type)
				return GL_TRUE;
	}
	return GL_FALSE;
}
// 更新道具状态
void Game::UpdatePowerUps(GLfloat dt) {
	for (PowerUp& powerUp : this->PowerUps) {
		powerUp.Position += powerUp.Velocity * dt;
		// std::cout << powerUp.Velocity.y << std::endl;
		if (powerUp.Activated) {
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f) {
				// 持续时间结束 道具失效
				powerUp.Activated = GL_FALSE;
				// 停用效果
				if (powerUp.Type == "sticky") {
					if (!isOtherPowerUpActive(this->PowerUps, "sticky")) {
						Ball->Sticky = GL_FALSE;
						Player->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "pass-through")
				{
					if (!isOtherPowerUpActive(this->PowerUps, "pass-through"))
					{
						Ball->PassThrough = GL_FALSE;
						Ball->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!isOtherPowerUpActive(this->PowerUps, "confuse"))
					{
						Effects->Confuse = GL_FALSE;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!isOtherPowerUpActive(this->PowerUps, "chaos"))
					{
						Effects->Chaos = GL_FALSE;
					}
				}
			}
		}
	}
	// 移除所有失效的道具
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp& powerUp) {return powerUp.Destroyed && !powerUp.Activated; }), this->PowerUps.end());
}


// 碰撞检测
GLboolean CheckCollision(GameObject& one, GameObject& two);
Collision CheckCollision(BallObject& one, GameObject& two);
Direction VectorDirection(glm::vec2 target);

void Game::Docollisions() {
	// 检查球与砖块的碰撞
	for (GameObject& box : this->Levels[this->Level].Bricks) {
		if (!box.Destroyed) {
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision)) {	// 发生碰撞
				// 不是实心砖快就销毁
				if (!box.IsSolid) {
					box.Destroyed = GL_TRUE;
					this->SpawnPowerUps(box);	// 掉道具了
					SoundEngine->play2D("music/bleep.mp3", GL_FALSE);
				}
				else {
					// 如果是实心砖快激活shake特效
					ShakeTime = 0.05f;
					Effects->Shake = GL_TRUE;
					SoundEngine->play2D("music/solid.wav", GL_FALSE);
				}
				// 碰撞处理
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (!(Ball->PassThrough && !box.IsSolid)) {		// 当passthrough生效时不执行碰撞检测
					if (dir == LEFT || dir == RIGHT) {			// 水平碰撞
						Ball->Velocity.x = -Ball->Velocity.x;	// 反转水平速度
						 // 重定位
						GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);	// 球进入AABB程度
						if (dir == LEFT)
							Ball->Position.x += penetration; // 将球右移
						else
							Ball->Position.x -= penetration; // 将球左移
					}
					else										// 垂直方向碰撞
					{
						Ball->Velocity.y = -Ball->Velocity.y; // 反转垂直速度
						// 重定位
						GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
						if (dir == UP)
							Ball->Position.y -= penetration; // 将球上移
						else
							Ball->Position.y += penetration; // 将球下移
					}
				}
			}
		}
	}
	// 检测道具
	for (PowerUp& powerUp : this->PowerUps) {
		if (!powerUp.Destroyed) {
			if (powerUp.Position.y >= this->Height)	// 道具掉出屏幕 销毁
				powerUp.Destroyed = GL_TRUE;
			if (CheckCollision(*Player, powerUp)) {
				// 道具与挡板接触 激活道具
				ActivatePowerUp(powerUp);
				powerUp.Activated = GL_TRUE;
				powerUp.Destroyed = GL_TRUE;
				SoundEngine->play2D("music/powerup.wav", GL_FALSE);
			}
		}
	}

	// 检查球与挡板碰撞
	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result)) {
		// 检查与哪个位置碰撞，并改变速度
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2.0f;
		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		GLfloat percentage = distance / (Player->Size.x / 2.0f);
		// 移动
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);	// 保持速度矢量大小不变

		Ball->Stuck = Ball->Sticky;

		SoundEngine->play2D("music/bleep.wav", GL_FALSE);
		//Ball->Stuck = GL_TRUE;
		//std::cout << Ball->Stuck << std::endl;
	}
}

// AA-BB collision
GLboolean CheckCollision(GameObject& one, GameObject& two) {
	// x轴方向碰撞
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// y轴方向碰撞
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// 只有两个轴向都有碰撞时才碰撞
	return collisionX && collisionY;
}

// AA-BB - cirle collision 
Collision CheckCollision(BallObject& one, GameObject& two) {
	// 获取圆心
	glm::vec2 center(one.Position + one.Radius);
	// 计算AABB的信息（中心、半边长）
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	// 获取两个中心的差矢量
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
	glm::vec2 closest = aabb_center + clamped;
	// 获得圆心center和最近点closest的矢量并判断是否 length <= radius
	difference = closest - center;
	if (glm::length(difference) < one.Radius)
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f, 0.0f));
}

// 返回最接近的上下左右矢量方向
Direction VectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),  // 上
		glm::vec2(1.0f, 0.0f),  // 右
		glm::vec2(0.0f, -1.0f), // 下
		glm::vec2(-1.0f, 0.0f)  // 左
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}