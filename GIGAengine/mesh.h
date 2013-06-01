
#ifndef MESH_H
#define MESH_H

enum meshInit { EMPTY, QUAD };

class mesh {
	private:
		int attributeCount, vertexCount, isIndexed;
		GLuint vertexArray, * vertexBuffer, indexBuffer;
	public:
		mesh(meshInit init = EMPTY);
		~mesh();
		//dimensions is amount of floats per vertex (must be in [1,4])
		//to disable an attribute or index array, set vertexCount to 0
		int data(int location, int newVertexCount, int dimensions, float * data, GLenum usage = GL_STATIC_DRAW);
		int indices(int newVertexCount, unsigned int * data, GLenum usage = GL_STATIC_DRAW);
		int draw(GLenum mode, int instanceCount = 1);
};

#endif
