#include "shader.h"
#include <string>

void setProjection(shader & s, std::string glslName, float fov, float aspect, float nearplane = .1f, float farplane = 100.0f);
void setCamera(shader & s, std::string glslName, float camx, float camy, float camz, float targetx, float targety, float targetz);