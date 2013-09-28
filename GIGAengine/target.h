
#ifndef TARGET_H
#define TARGET_H

class target {
	private:
		GLuint id;
		GLenum * attachments;
		int maxtex;
	public:
		target();
		~target();
		int getID();
		int setTexture(GLenum attachment, texture * texture, bool tach = false, int mip = 0);
		int setTexture(GLenum attachment, texture3D * texture, bool tach = false, int mip = 0);
		int attach(int width, int height);
};

#endif
