
#ifndef TEXTURE3D_H
#define TEXTURE3D_H

class texture3D {
	private:
		GLuint id;
		bool mipmaps;
	public:
		int width, height, depth;
		texture3D(int w, int h, int d, int c = 1, GLint filter = GL_NEAREST, GLint repeat = GL_CLAMP_TO_BORDER);
		~texture3D();
		int addmiplevel(int w, int h, int d, int c = 1, int mip = 0, GLint filter = GL_NEAREST, GLint repeat = GL_CLAMP_TO_BORDER);
		int bind(int slot);
	
	friend class target;
};

#endif
