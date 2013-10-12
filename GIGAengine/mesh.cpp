
#include "main.h"

mesh::mesh(meshInit init) : vertexCount(-1), isIndexed(0) {
	
	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindVertexArray(0);

	if(init==MESH_QUAD) {
		float quadVertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};
		unsigned int quadIndices[] = {0, 1, 2, 0, 2, 3};
		data(4, 2*sizeof(float), quadVertices);
		pointer(0, 0, 2);
		indices(6, quadIndices);
	} else if(init==MESH_TRI) {
		float triVertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
		unsigned int triIndices[] = {0, 1, 2};
		data(3, 2*sizeof(float), triVertices);
		pointer(0, 0, 2);
		indices(3, triIndices);
	}
}

mesh::~mesh() {
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
}

int mesh::data(int newVertexCount, int vertexSize, void * data, GLenum usage) {
	this->vertexSize = vertexSize;

	glBindVertexArray(vertexArray);

	if(!isIndexed)
		vertexCount = newVertexCount;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, newVertexCount*vertexSize, data, usage);

	glBindVertexArray(0);
	return 0;
}

int mesh::pointer(int location, int floatOffset, int dimensions) {
	glBindVertexArray(vertexArray);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, dimensions, GL_FLOAT, 0, vertexSize, (void*)(floatOffset*sizeof(float)));
	glBindVertexArray(0);
	return 0;
}

int mesh::indices(int newVertexCount, unsigned int * data, GLenum usage) {
	glBindVertexArray(vertexArray);
	if(newVertexCount==0) {
		isIndexed = 0;
	} else {
		vertexCount = newVertexCount;
		if(!isIndexed) {
			isIndexed = 1;
		}
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
	glFinish();
	glFlush();
	return 0;
}
