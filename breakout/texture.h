/*
texture类
	2Dtexture 负责存储和配置纹理
*/

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>

class Texture2D {
public:
	// 纹理ID
	GLuint ID;
	// 纹理尺寸
	GLuint Width, Height;
	// 纹理格式
	GLuint Internal_Format;
	GLuint Image_Format;
	// 纹理配置
	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;	 // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max;	 // Filtering mode if texture pixels > screen pixels
	// 构造函数
	Texture2D();
	// 从图片创建纹理
	void Generate(GLuint width, GLuint height, unsigned char* data);
	// 将纹理绑定为当前活动的GL_TEXTURE_2D纹理对象
	void Bind() const;
};

#endif