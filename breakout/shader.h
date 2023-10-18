/*
��ɫ����
	�������ɱ���������ɫ�� �Լ�����״̬������uniform����
*/
#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

// ������ɫ��
class Shader {
public:
	// state
	GLuint ID;
	// constructor 
	Shader() {}
	// ���õ�ǰΪ�
	Shader& Use();
	// ������ɫ��
	void Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr);
	// ���ߺ���
	void    SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
	void    SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = false);
private:
	// ������
	void checkCompileErrors(GLuint object, std::string type);
};


#endif

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
//#ifndef SHADER_H
//#define SHADER_H
//
//#include <string>
//
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//
//// General purpose shader object. Compiles from file, generates
//// compile/link-time error messages and hosts several utility 
//// functions for easy management.
//class Shader
//{
//public:
//    // state
//    unsigned int ID;
//    // constructor
//    Shader() { }
//    // sets the current shader as active
//    Shader& Use();
//    // compiles the shader from given source code
//    void    Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // note: geometry source code is optional 
//    // utility functions
//    void    SetFloat(const char* name, float value, bool useShader = false);
//    void    SetInteger(const char* name, int value, bool useShader = false);
//    void    SetVector2f(const char* name, float x, float y, bool useShader = false);
//    void    SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
//    void    SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
//    void    SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
//    void    SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
//    void    SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
//    void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
//private:
//    // checks if compilation or linking failed and if so, print the error logs
//    void    checkCompileErrors(unsigned int object, std::string type);
//};
//
//#endif