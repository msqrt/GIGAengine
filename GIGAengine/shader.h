
#ifndef SHADER_H
#define SHADER_H

#define RESOURCE_SHADER 0x01
#define GEOMETRY_SHADER 0x02

class shader {
	private:
		GLuint p, v, f, g;
		std::string path;
		int flags;
	public:
		shader(std::string programPath, int pflags = 0);
		shader(int programName, int resourceType, int pflags = 0);
		~shader();
		//parameter is for the (con|de)structor, leave to default (0=delete+load,1=load,2=delete)
		int reload(int mode=0);
		GLint getLoc(std::string name);
		//use the program
		int use();
		int compile(char * source);
		int error;
};

#endif