
#ifndef SHADER_H
#define SHADER_H

#define GEOMETRY_SHADER 0x01

class shader {
	private:
		GLuint p, v, f, g;
		std::string path;
		int flags;
	public:
		shader(std::string progpath, int pflags = 0);
		~shader();
		//parameter is for the (con|de)structor, leave to default (0=delete+load,1=load,2=delete)
		int reload(int mode=0);
		GLint getLoc(std::string name);
		//use the program
		int use();
		int error;
};

#endif