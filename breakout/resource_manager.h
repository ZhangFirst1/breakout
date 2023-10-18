#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

/* 一个静态的资源管理器类，负责加载纹理和着色器。
所有资源都是静态的，没有定义公共构造函数*/

class ResourceManager {
public:
	// 资源存储
	static std::map<std::string, Shader>	Shaders;
	static std::map<std::string, Texture2D> Textures;
	// 从文件中加载片元、顶点（几何）着色器
	static Shader	 LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	// 检索存储的着色器
	static Shader&	 GetShader(std::string name);
	// 加载纹理
	static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, std::string name);
	// 检索存储的纹理
	static Texture2D& GetTexture(std::string name);
	// 删除所有资源
	static void		 Clear();
private:
	// 私有构造函数，不需要任何实际的资源管理器对象，他的成员和函数应该是公开可用的
	ResourceManager() {}
	// 实际的创建函数
	static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};


#endif