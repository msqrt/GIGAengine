#include "simpledraw.h"
#include "mesh.h"

void simple::tri(float x1, float y1, 
		float x2, float y2, 
		float x3, float y3
	)
{
	float vertices[6];// = {-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f};
	vertices[0] = x1;
	vertices[1] = y1;
	vertices[2] = x2;
	vertices[3] = y2;
	vertices[4] = x3;
	vertices[5] = y3;
	unsigned int indices[] = {0, 1, 2};

	mesh triangle(MESH_EMPTY);	
	triangle.data(3, 2*sizeof(float), vertices);
	triangle.indices(3, indices);
	triangle.pointer(0, 0, 2);
	triangle.draw(GL_TRIANGLES);

	//mesh quadi(MESH_QUAD);
	//quadi.draw(GL_TRIANGLES);
}