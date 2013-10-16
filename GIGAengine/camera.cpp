
#include "main.h"


void setProjection(shader & s, std::string glslName, float fov, float aspect, float nearplane = .5f, float farplane = 100.0f) {
	float projection[16] = {.0f};
	projection[0] = 2.0f*nearplane*fov*aspect;
	projection[5] = 2.0f*nearplane*fov;
	projection[10] = (farplane+nearplane)/(farplane-nearplane);
	projection[14] = -2.0f*farplane*nearplane/(farplane-nearplane);
	projection[11] = 1.0f;
	glUniformMatrix4fv(s.getLoc(glslName), 1, false, projection);
}

void setCamera(shader & s, std::string glslName, float camx, float camy, float camz, float targetx, float targety, float targetz) {
	float camera[16] = {.0f};
	camera[0] = camera[5] = camera[10] = camera[15] = 1.0f;
	float horiz = atan2(targetx-camx, targetz-camz),
		  vert  = atan2(targety-camy, sqrt((targetz-camz)*(targetz-camz)+(targetx-camx)*(targetx-camx)));
	float cosa = cos(horiz), sina = sin(horiz), cosb = cos(vert), sinb = sin(vert);
	camera[0] = cosa;
	camera[1] = -sina*sinb;
	camera[2] = sina*cosb;
	camera[5] = cosb;
	camera[6] = sinb;
	camera[8] = -sina;
	camera[9] = -cosa*sinb;
	camera[10] = cosa*cosb;
	camera[12] = -(camx*camera[0]+camy*camera[4]+camz*camera[8]);
	camera[13] = -(camx*camera[1]+camy*camera[5]+camz*camera[9]);
	camera[14] = -(camx*camera[2]+camy*camera[6]+camz*camera[10]);
	glUniformMatrix4fv(s.getLoc(glslName), 1, false, camera);
}
