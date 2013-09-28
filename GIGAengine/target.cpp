
#include "main.h"

target::target() {
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxtex);
	attachments = new GLenum[maxtex];
	ZeroMemory(attachments, sizeof(GLenum)*(maxtex));
	maxtex--;
	glGenFramebuffers(1, &id);
}

target::~target() {
	glDeleteFramebuffers(1, &id);
	delete [] attachments;
}

int target::setTexture(GLenum attachment, texture * texture, bool tach, int mip) {
	int loc = attachment-GL_COLOR_ATTACHMENT0;
	if(attachment==GL_DEPTH_ATTACHMENT||attachment==GL_DEPTH_STENCIL_ATTACHMENT)
		loc = maxtex;

	if(texture != 0)
		attachments[loc] = attachment;
	else
		attachments[loc] = 0;

	if(tach)
		glBindFramebuffer(GL_FRAMEBUFFER, id);

	int texid = 0;
	if(texture!=0)
		texid = texture->id;

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texid, mip);
	if(tach)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	return 0;
}

int target::setTexture(GLenum attachment, texture3D * texture, bool tach, int mip) {
	int loc = attachment-GL_COLOR_ATTACHMENT0;
	if(attachment==GL_DEPTH_ATTACHMENT||attachment==GL_DEPTH_STENCIL_ATTACHMENT)
		loc = maxtex;

	if(texture != 0)
		attachments[loc] = attachment;
	else
		attachments[loc] = 0;

	if(tach)
		glBindFramebuffer(GL_FRAMEBUFFER, id);

	int texid = 0;
	if(texture!=0)
		texid = texture->id;

	glFramebufferTexture(GL_FRAMEBUFFER, attachment, texid, mip);
	if(tach)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return 0;
}

int target::attach(int width, int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, width, height);
	int i;
	for(i = 0; i<maxtex && attachments[i]!=0; i++);
	glDrawBuffers(i, attachments);

	return 0;
}

int target::getID() {
	return id;
}