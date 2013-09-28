
#include "main.h"

texture3D::texture3D(int w, int h, int d, int c, GLint filter, GLint repeat) {
	glGenTextures(1, &id);
	mipmaps = false;
	addmiplevel(w, h, d, c, 0, filter, repeat);
}

int texture3D::addmiplevel(int w, int h, int d, int c, int mip, GLint filter, GLint repeat) {
	if(mip!=0)
		mipmaps = true;

	width = w;
	height = h;
	depth = d;

	glBindTexture(GL_TEXTURE_3D, id);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, repeat);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, repeat);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, repeat);
	if(mipmaps) {
		if(filter == GL_LINEAR)
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
	
	if(c==0)
		glTexImage3D(GL_TEXTURE_3D, mip, GL_R16F, width, height, depth, 0, GL_RED, GL_FLOAT, 0);
	else if(c==1)
		glTexImage3D(GL_TEXTURE_3D, mip, GL_R32UI, width, height, depth, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
	else if(c==2)
		glTexImage3D(GL_TEXTURE_3D, mip, GL_R8UI, width, height, depth, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
	else
		glTexImage3D(GL_TEXTURE_3D, mip, GL_RGB16F, width, height, depth, 0, GL_RGB, GL_FLOAT, 0);
	
	glBindTexture(GL_TEXTURE_3D, 0);

	return 0;
}

texture3D::~texture3D() {
	glDeleteTextures(1, &id);
}

int texture3D::bind(int slot) {
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_3D, id);
	return 0;
}