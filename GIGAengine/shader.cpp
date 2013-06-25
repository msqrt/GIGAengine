
#include "main.h"

shader::shader(std::string programPath, int pflags) {
	error = 0;
	flags = pflags;
	path = programPath;
	reload(1);
}

shader::~shader() {
	reload(2);
}

shader::shader(int programName, int resourceType, int pflags) {
	flags = pflags | RESOURCE_SHADER;
	HMODULE handle = GetModuleHandle(0);
	HRSRC resource = FindResource(handle, MAKEINTRESOURCE(programName), MAKEINTRESOURCE(resourceType));
	HGLOBAL resourceData = LoadResource(handle, resource);
	int size = (int)SizeofResource(handle, resource);
	char * tdata = (char*)LockResource(resourceData);
	char * data = new char[size+1];
	memcpy(data, tdata, size);
	data[size] = '\0';
	compile(data);
	delete [] data;
}

int shader::reload(int mode) {
	
	if(flags & RESOURCE_SHADER)
		return 1;

	if(mode!=1)printf("re");
	if(mode< 2)printf("loading");
	else       printf("leasing");
	printf(" %s\n", path.c_str());

	//delete the previous ones
	if(mode!=1) {
		glUseProgram(0);
		glDetachShader(p, v);
		glDetachShader(p, f);

		glDeleteShader(v);
		glDeleteShader(f);

		glDeleteProgram(p);
	}
	//load new
	if(mode<2) {
		//ugly piece of code that reads the two files
		FILE * sf = 0;
		fopen_s(&sf, path.c_str(), "r");
		int len = 0;
		while(fgetc(sf)!=EOF)len++;
		rewind(sf);
		char * source = new char[len+1];
		fread_s(source, sizeof(char)*len, sizeof(char), len, sf);
		source[len]='\0';
		fclose(sf);
		compile(source);
		delete [] source;
	}
	return 0;
}

GLint shader::getLoc(std::string name) {
	return glGetUniformLocation(p, name.c_str());
}

int shader::use() {
	glUseProgram(p);
	return 0;
}

int shader::compile(char * source) {
	
	int len;
	p = glCreateProgram();

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	if(flags & GEOMETRY_SHADER)
		g = glCreateShader(GL_GEOMETRY_SHADER);

	char version[] = "#version 330\n", disclaimer[] = "/* READING OTHER PEOPLES SHADER CODE IS SHAMEFUL */\n";
	char def1[] = "#define vertexcompile\n", def2[] = "#define fragmentcompile\n", def3[] = "#define geometrycompile\n";
	char * vert[] = {version, def1, disclaimer, source};
	glShaderSource(v, 4, vert, 0);
	glCompileShader(v);
	char * frag[] = {version, def2, disclaimer, source};
	glShaderSource(f, 4, frag, 0);
	glCompileShader(f);

	if(flags & GEOMETRY_SHADER) {
		char * geom[] = {version, def3, disclaimer, source};
		glShaderSource(g, 4, geom, 0);
		glCompileShader(g);
	}

	char err[1024] = {'\0'};
	glGetShaderInfoLog(v, 1024, &len, err);
	//if len>0 there was an error
	if(len>0)
		printf("\n%s(vertex):\n\n%s\n", path.c_str(), err);
	ZeroMemory(err, 1024*sizeof(char));

	glGetShaderInfoLog(f, 1024, &len, err);
	//if len>0 there was an error
	if(len>0)
		printf("\n%s(fragment):\n\n%s\n", path.c_str(), err);
	ZeroMemory(err, 1024*sizeof(char));

	if(flags & GEOMETRY_SHADER) {
		glGetShaderInfoLog(g, 1024, &len, err);
		//if len>0 there was an error
		if(len>0)
			printf("\n%s(geometry):\n\n%s\n", path.c_str(), err);
		ZeroMemory(err, 1024*sizeof(char));
	}

	glAttachShader(p, v);
	glAttachShader(p, f);
	if(flags & GEOMETRY_SHADER)
		glAttachShader(p, g);
	glLinkProgram(p);
		
	glGetProgramInfoLog(p, 1024, &len, err);
	//if len>0 there was an error
	if(len>0) {
		printf("\n%s(program):\n\n%s\n", path.c_str(), err);
		error = 1;
	}

	return 0;
}