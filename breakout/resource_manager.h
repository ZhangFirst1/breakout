#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

/* һ����̬����Դ�������࣬��������������ɫ����
������Դ���Ǿ�̬�ģ�û�ж��幫�����캯��*/

class ResourceManager {
public:
	// ��Դ�洢
	static std::map<std::string, Shader>	Shaders;
	static std::map<std::string, Texture2D> Textures;
	// ���ļ��м���ƬԪ�����㣨���Σ���ɫ��
	static Shader	 LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	// �����洢����ɫ��
	static Shader&	 GetShader(std::string name);
	// ��������
	static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, std::string name);
	// �����洢������
	static Texture2D& GetTexture(std::string name);
	// ɾ��������Դ
	static void		 Clear();
private:
	// ˽�й��캯��������Ҫ�κ�ʵ�ʵ���Դ�������������ĳ�Ա�ͺ���Ӧ���ǹ������õ�
	ResourceManager() {}
	// ʵ�ʵĴ�������
	static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);
};


#endif