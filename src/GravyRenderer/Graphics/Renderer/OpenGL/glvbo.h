#pragma once

#include "Core/Base.h"

#include <GL/glew.h>

struct Vertex
{
	float posX, posY, posZ;
	float texCoordX, texCoordY;
	float normalX, normalY, normalZ;

	Vertex(float _posX, float _posY, float _posZ, float _texCoordX, float _texCoordY, float _normalX, float _normalY, float _normalZ)
	{
		posX = _posX;
		posY = _posY;
		posZ = _posZ;

		texCoordX = _texCoordX;
		texCoordY = _texCoordY;

		normalX = _normalX;
		normalY = _normalY;
		normalZ = _normalZ;
	}

	Vertex(std::vector<float> _vertices)
	{
		posX = _vertices[0];
		posY = _vertices[1];
		posZ = _vertices[2];

		texCoordX = _vertices[3];
		texCoordY = _vertices[4];

		normalX = _vertices[5];
		normalY = _vertices[6];
		normalZ = _vertices[7];
	}
};

namespace Renderer
{
	class glvbo
	{
	private:
		uint32_t BufferSize;
	public:
		glvbo();
        ~glvbo() {}

		GLuint Create(uint32_t bufferSize, GLenum usage = GL_STATIC_DRAW);
		GLuint Create(std::vector<float> *vertices, GLenum usage = GL_STATIC_DRAW);
		GLuint Create(float *vertices, uint32_t bufferSize, GLenum usage = GL_STATIC_DRAW);
		GLuint Create(std::vector<Vertex> *vertices, GLenum usage = GL_STATIC_DRAW);

		void Bind();
		void UnBind();

		void Delete();

		void SendData(const void *data, uint32_t bufferSize);
	
	public:
		GLuint VBO;
	};
}