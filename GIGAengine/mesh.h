
#pragma once

enum meshInit { EMPTY, QUAD };

class mesh {
	private:
		int vertexCount, isIndexed, vertexSize;
		GLuint vertexArray, vertexBuffer, indexBuffer;
	public:
		mesh(meshInit init = EMPTY);
		~mesh();
		int data(int newVertexCount, int vertexSize, void * data, GLenum usage = GL_STATIC_DRAW);
		int pointer(int location, int floatOffset, int dimensions);
		int indices(int newVertexCount, unsigned int * data, GLenum usage = GL_STATIC_DRAW);
		int draw(GLenum mode, int instanceCount = 1);
};
