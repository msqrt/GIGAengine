#include <cstdlib>
#include <cstdio>
#include <map>
#include "shadermap.h"

ShaderMap::ShaderMap() 
{

}

ShaderMap::~ShaderMap() 
{
		/*
        for (std::map<std::string, shader*>::iterator it=shaders.begin();it!=shaders.end();++it) {
                fprintf(stdout, "Freeing shader %s", it->first.c_str());
                delete it->second;
        }
		*/
}

shader* ShaderMap::loadFile(std::string filename)
{
        //filename = "shaders/" + filename;

        shader* current_shader = new shader(filename);

		addShader(current_shader);


        return current_shader;
}

shader* ShaderMap::getShader(std::string basename)
{
        return shaders.at(basename);
}

void ShaderMap::reloadAll()
{
	fprintf(stdout, "reloading all shaders\n");
	for (std::map<std::string, shader*>::iterator it=shaders.begin();it!=shaders.end();++it) {
		it->second->reload();	
	}
}

shader* ShaderMap::addShader(shader* shaderprogram)
{
        char drive[_MAX_DRIVE]; 
        char dir[_MAX_DIR];
        char basename[_MAX_FNAME];
        char extension[_MAX_EXT];
        _splitpath_s(shaderprogram->getPath().c_str(), drive, dir, basename, extension);

        shaders[std::string(basename)] = shaderprogram;

        fprintf(stdout, "Added shader: %s", basename);

		return shaderprogram;
}