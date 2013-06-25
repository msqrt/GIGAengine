
#include "main.h"

texture::texture(std::wstring path, GLint filter) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	float max;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
	
	Gdiplus::Bitmap img(path.c_str(), 1);

	width = img.GetWidth();
	height = img.GetHeight();

	Gdiplus::Rect area;
	area.X = 0; area.Y = 0; area.Width = width; area.Height = height;

	Gdiplus::BitmapData data;

	img.LockBits(&area, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data.Scan0);
	img.UnlockBits(&data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

texture::texture(int width, int height, bool depth, GLint filter, GLint repeat): width(width), height(height), depth(depth) {

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	
	if(depth)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

texture::~texture() {
	glDeleteTextures(1, &id);
}

int texture::resize(int w, int h) {
	bind(0);
	width = w;
	height = h;

	if(depth)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

	return 0;
}

int texture::bind(int slot) {
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_2D, id);
	return 0;
}