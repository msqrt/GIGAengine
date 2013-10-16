#include <cmath>
#include "main.h"
#include "util.h"

#define M_PI (3.141592653589)

double degToRad(double degrees)
{
	return (degrees/360.0)*2.0*M_PI;
}


void util::perspectiveMatrix(shader & s, float fov, float aspect, float nearplane, float farplane)
{
	float mat[16] = {.0f};

	float zoomy = 1.0f/tan((float)degToRad(fov/2.0));

	float zoomx = zoomy/aspect;
	float frustrum_length = farplane - nearplane;
	float z_scale = -((farplane+nearplane)/frustrum_length);
	float z_translate = -((2*nearplane*farplane)/frustrum_length);

	mat[0] = zoomx;
	mat[5] = zoomy;
	mat[10] = z_scale;
	mat[14] = z_translate;
	mat[11] = -1;	// copy the (sign flipped) z coordinate to w
	mat[15] = 1.0f;

	glUniformMatrix4fv(s.getLoc("projection"), 1, false, mat); 
}