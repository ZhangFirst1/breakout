#include <iostream>
#include <sstream>
#include <fstream>

#include "resource_manager.h"
#include "stb_image.h"

// 实例化静态变量
std::map<std::string, Shader>	 ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name) {
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name) {
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear() {
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile) {
	// 1.从文件中检索着色器代码
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try {
		// 打开文件
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// 读入缓冲流
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// 关闭文件
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// 转为字符串
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//std::cout << fragmentCode << std::endl;
		// 几何着色器
		if (gShaderFile != nullptr) {
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();
	//2. 从代码创建着色器
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile == nullptr ? nullptr : gShaderCode);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar* file, GLboolean alpha) {
	// 创建纹理对象
	Texture2D texture;
	if (alpha) {
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// 加载图片
	int width, height;
	unsigned char* image = stbi_load(file, &width, &height, 0, 0);
	// 生成纹理
	texture.Generate(width, height, image);
	stbi_image_free(image);
	return texture;
}