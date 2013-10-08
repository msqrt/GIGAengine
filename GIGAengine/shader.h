
#pragma once

#include "main.h"

#define RESOURCE_SHADER 0x01
#define GEOMETRY_SHADER 0x02

enum uniformType { GFLOAT, GVEC2, GVEC3, GVEC4, GUINT, GINT, GMATRIX2, GMATRIX3, GMATRIX4 };

struct uniformData {
	std::string name;
	void * data;
	GLuint location;
	uniformType type;
};

class shader {
	private:
		GLuint p, v, f, g;
		std::string path;
		int flags, uniformCount;
		uniformData * uniforms;
	public:
		shader(std::string programPath, int pflags = 0);
		shader(int programName, int resourceType, int pflags = 0);
		~shader();
		//parameter is for the (con|de)structor, leave to default (0=delete+load,1=load,2=delete)
		int reload(int mode=0);
		GLint getLoc(std::string name);
		int addUniform(std::string name, void* data, uniformType type);
		int setUniform(std::string name, void* data, uniformType type);
		//use the program
		int use();
		int updateUniforms();
		int compile(char * source);
		int error;
};
