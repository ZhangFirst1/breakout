#include <iostream>
#include <sstream>
#include <fstream>

#include "resource_manager.h"
#include "stb_image.h"

// ʵ������̬����
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
	// 1.���ļ��м�����ɫ������
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try {
		// ���ļ�
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// ���뻺����
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// �ر��ļ�
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// תΪ�ַ���
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//std::cout << fragmentCode << std::endl;
		// ������ɫ��
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
	//2. �Ӵ��봴����ɫ��
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile == nullptr ? nullptr : gShaderCode);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar* file, GLboolean alpha) {
	// �����������
	Texture2D texture;
	if (alpha) {
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// ����ͼƬ
	int width, height;
	unsigned char* image = stbi_load(file, &width, &height, 0, 0);
	// ��������
	texture.Generate(width, height, image);
	stbi_image_free(image);
	return texture;
}