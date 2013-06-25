
#ifndef TEXTURE_H
#define TEXTURE_H

class texture {
	private:
		GLuint id;
	public:
		int width, height; bool depth;
		texture(std::wstring path, GLint filter = GL_LINEAR);
		texture(int width, int height, bool depth = false, GLint filter = GL_NEAREST, GLint repeat = GL_CLAMP_TO_EDGE);
		~texture();
		int bind(int slot);
		int resize(int width, int height);
	friend class target;
};

#endif
