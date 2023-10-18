/*
texture��
	2Dtexture ����洢����������
*/

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>

class Texture2D {
public:
	// ����ID
	GLuint ID;
	// ����ߴ�
	GLuint Width, Height;
	// �����ʽ
	GLuint Internal_Format;
	GLuint Image_Format;
	// ��������
	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;	 // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max;	 // Filtering mode if texture pixels > screen pixels
	// ���캯��
	Texture2D();
	// ��ͼƬ��������
	void Generate(GLuint width, GLuint height, unsigned char* data);
	// �������Ϊ��ǰ���GL_TEXTURE_2D�������
	void Bind() const;
};

#endif