/** 
 * A shader storage object.
 * You shouldn't try to access the map before you've got a valid OpenGL 
 * context.
 */

#pragma once

#include <string>
#include <map>
#include "shader.h"

class ShaderMap {
        public:
        ShaderMap();    
        ~ShaderMap();

        /// Loads a shader from the /shaders data directory.
        /// Filename should be e.g. "shiny.glsl"
        shader* loadFile(std::string filename);
        shader* getShader(std::string basename);

        private:
        std::map<std::string, shader*> shaders;

};