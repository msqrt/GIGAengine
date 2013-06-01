
#include "main.h"

mesh::mesh(meshInit init) {
	vertexCount = -1;
	isIndexed = 0;

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributeCount);

	vertexBuffer  = new GLuint[attributeCount];
	ZeroMemory(vertexBuffer, sizeof(GLuint)*attributeCount);

	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(attributeCount, vertexBuffer);

	if(init==QUAD) {
		float quadVertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};
		unsigned int quadIndices[] = {0, 1, 2, 0, 2, 3};
		data(0, 4, 2, quadVertices);
		indices(6, quadIndices);
	}
}

mesh::~mesh() {
	if(isIndexed)
		glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(attributeCount, vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);

	delete [] vertexBuffer;
}

int mesh::data(int location, int newVertexCount, int dimensions, float * data, GLenum usage) {
	glBindVertexArray(vertexArray);

	if(newVertexCount==0)
		glDisableVertexAttribArray(location);
	else {
		if(!isIndexed)
			vertexCount = newVertexCount;
		glEnableVertexAttribArray(location);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[location]);
		glBufferData(GL_ARRAY_BUFFER, vertexCount*dimensions*sizeof(float), data, usage);
		glVertexAttribPointer(location, dimensions, GL_FLOAT, 0, 0, 0);
	}

	glBindVertexArray(0);
	return 0;
}

int mesh::indices(int newVertexCount, unsigned int * data, GLenum usage) {
	glBindVertexArray(vertexArray);
	if(newVertexCount==0 && isIndexed) {
		isIndexed = 0;
		glDeleteBuffers(1, &indexBuffer);
	}
	else if(newVertexCount>0) {
		vertexCount = newVertexCount;
		if(!isIndexed) {
			isIndexed = 1;
			glGenBuffers(1, &indexBuffer);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount*sizeof(unsigned int), data, usage);
	}
	glBindVertexArray(0);
	return 0;
}

int mesh::draw(GLenum mode, int instanceCount) {
	if(vertexCount>=0) {
		glBindVertexArray(vertexArray);
		if(instanceCount==1) {
			if(isIndexed)
				glDrawElements(mode, vertexCount, GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(mode, 0, vertexCount);
		}
		else {
			if(isIndexed)
				glDrawElementsInstanced(mode, vertexCount, GL_UNSIGNED_INT, 0, instanceCount);
			else
				glDrawArraysInstanced(mode, 0, vertexCount, instanceCount);
		}
		glBindVertexArray(0);
	}
	return 0;
}
