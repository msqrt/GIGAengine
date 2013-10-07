
#include "main.h"

shader::shader(std::string programPath, int pflags) {
	error = 0;
	flags = pflags;
	path = programPath;
	uniformCount = 0;
	reload(1);
	uniforms = 0;
	use();
}

shader::~shader() {
	reload(2);
	delete [] uniforms;
}

shader::shader(int programName, int resourceType, int pflags) {
	uniformCount = 0;
	uniforms = 0;
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
	use();
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

		use();
		for(int i = 0; i<uniformCount; i++)
			uniforms[i].location = getLoc(uniforms[i].name);
	}
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

	int status;

	glGetShaderiv(v, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE) {
		glGetShaderInfoLog(v, 1024, &len, err);
		if(len>0)
			printf("\n%s(vertex):\n\n%s\n", path.c_str(), err);
		ZeroMemory(err, 1024*sizeof(char));
	}
	
	glGetShaderiv(f, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE) {
		glGetShaderInfoLog(f, 1024, &len, err);
		if(len>0)
			printf("\n%s(fragment):\n\n%s\n", path.c_str(), err);
		ZeroMemory(err, 1024*sizeof(char));
	}
	if(flags & GEOMETRY_SHADER) {
		glGetShaderiv(g, GL_COMPILE_STATUS, &status);
		if(status==GL_FALSE) {
			glGetShaderInfoLog(g, 1024, &len, err);
			if(len>0)
				printf("\n%s(geometry):\n\n%s\n", path.c_str(), err);
			ZeroMemory(err, 1024*sizeof(char));
		}
	}

	glAttachShader(p, v);
	glAttachShader(p, f);
	if(flags & GEOMETRY_SHADER)
		glAttachShader(p, g);
	glLinkProgram(p);
	
	glGetProgramiv(p, GL_LINK_STATUS, &status);
	if(status==GL_FALSE && err[0] == '/0') {
		glGetProgramInfoLog(p, 1024, &len, err);
		if(len>0) {
			printf("\n%s(program):\n\n%s\n", path.c_str(), err);
			//error = 1;
		}
	}
	return 0;
}

GLint shader::getLoc(std::string name) {
	return glGetUniformLocation(p, name.c_str());
}

int shader::use() {
	glUseProgram(p);
	updateUniforms();
	return 0;
}

int shader::addUniform(std::string name, void* data, uniformType type) {
	uniformData * tmp = new uniformData[uniformCount+1];
	memcpy(tmp, uniforms, sizeof(uniformData)*(uniformCount));
	tmp[uniformCount].data = data;
	tmp[uniformCount].name = name;
	tmp[uniformCount].type = type;
	tmp[uniformCount].location = getLoc(name);
	delete [] uniforms;
	uniforms = tmp;
	uniformCount++;
	return 0;
}

int shader::setUniform(std::string name, void* data, uniformType type) {
	switch(type) {
		case GFLOAT:   glUniform1fv      (getLoc(name), 1,    (float *)data); break;
		case GVEC2:    glUniform2fv      (getLoc(name), 1,    (float *)data); break;
		case GVEC3:    glUniform3fv      (getLoc(name), 1,    (float *)data); break;
		case GVEC4:    glUniform4fv      (getLoc(name), 1,    (float *)data); break;
		case GUINT:    glUniform1uiv     (getLoc(name), 1,    (GLuint*)data); break;
		case GINT:     glUniform1iv      (getLoc(name), 1,    (int   *)data); break;
		case GMATRIX2: glUniformMatrix2fv(getLoc(name), 1, 0, (float *)data); break;
		case GMATRIX3: glUniformMatrix3fv(getLoc(name), 1, 0, (float *)data); break;
		case GMATRIX4: glUniformMatrix4fv(getLoc(name), 1, 0, (float *)data); break;
	}
	return 0;
}

//enum uniformType { FLOAT, MATRIX4, MATRIX3, UINT, INT, VEC2, VEC3, VEC4 };

int shader::updateUniforms() {
	for(int i = 0; i<uniformCount; i++)
	switch(uniforms[i].type) {
		case GFLOAT:   glUniform1fv      (uniforms[i].location, 1,    (float *)uniforms[i].data); break;
		case GVEC2:    glUniform2fv      (uniforms[i].location, 1,    (float *)uniforms[i].data); break;
		case GVEC3:    glUniform3fv      (uniforms[i].location, 1,    (float *)uniforms[i].data); break;
		case GVEC4:    glUniform4fv      (uniforms[i].location, 1,    (float *)uniforms[i].data); break;
		case GUINT:    glUniform1uiv     (uniforms[i].location, 1,    (GLuint*)uniforms[i].data); break;
		case GINT:     glUniform1iv      (uniforms[i].location, 1,    (int   *)uniforms[i].data); break;
		case GMATRIX2: glUniformMatrix2fv(uniforms[i].location, 1, 0, (float *)uniforms[i].data); break;
		case GMATRIX3: glUniformMatrix3fv(uniforms[i].location, 1, 0, (float *)uniforms[i].data); break;
		case GMATRIX4: glUniformMatrix4fv(uniforms[i].location, 1, 0, (float *)uniforms[i].data); break;
	}
	return 0;
}