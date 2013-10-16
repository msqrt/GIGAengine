
#include "main.h"
#include "effect.h"
#include "postprocess.h"

PostProcess::PostProcess(int width, int height) :
	width(width),
	height(height),
	quad(MESH_QUAD),
	post("assets/postprocess.shader"),
	gaussian("assets/gaussian.shader"),
	redux("assets/redux.shader"),
	color(width, height, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER),
	additional(width, height, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER),
	depth(width, height, true, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER),
	bloom(width, height, false, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER),
	tmpblur(width, height, false, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER) {
	
	bindUniforms();
	bloom.bind(0);
	glGenerateMipmap(GL_TEXTURE_2D);
	tmpblur.bind(0);
	glGenerateMipmap(GL_TEXTURE_2D);

	for(int i = 0; i<BLOOMSTAGES; i++) {
		renderTarget[i].attach(width, height);
		renderTarget[i].setTexture(GL_COLOR_ATTACHMENT0, &bloom, false, i);
		if(i==0) {
			renderTarget[i].setTexture(GL_COLOR_ATTACHMENT0, &color, false, i);
			renderTarget[i].setTexture(GL_COLOR_ATTACHMENT1, &additional, false, i);
			renderTarget[i].setTexture(GL_DEPTH_ATTACHMENT, &depth, false, i);
		}
	}

}

void PostProcess::bind() {
	renderTarget[0].attach(width, height);
	renderTarget[0].setTexture(GL_COLOR_ATTACHMENT0, &color, false, 0);
	renderTarget[0].setTexture(GL_DEPTH_ATTACHMENT, &depth, false, 0);
}

void PostProcess::render(float t) {
	
	renderTarget[0].attach(width, height);
	
	int curwidth = width, curheight = height;

	glDisable(GL_DEPTH_TEST);

	color.bind(0);
	
	renderTarget[0].attach(curwidth, curheight);
	for(int i = 0; i<BLOOMSTAGES; i++) {
		renderTarget[i].setTexture(GL_COLOR_ATTACHMENT0, &tmpblur, false, i);
		gaussian.use();
		glUniform1f(gaussian.getLoc("i"), float(i));
		glUniform2f(gaussian.getLoc("screen"), float(curwidth), float(curheight));
		glUniform1f(gaussian.getLoc("dir"), .0f);
		quad.draw(GL_TRIANGLES);
		
		renderTarget[i].setTexture(GL_COLOR_ATTACHMENT0, &bloom, false, i);
		tmpblur.bind(0);

		glUniform1f(gaussian.getLoc("dir"), 1.0f);
		quad.draw(GL_TRIANGLES);
		
		if(i+1<BLOOMSTAGES) {
			curwidth  /= 2;
			curheight /= 2;

			redux.use();
			glUniform1f(redux.getLoc("i"), float(i));
			glUniform2f(redux.getLoc("screen"), float(curwidth), float(curheight));
			
			bloom.bind(0);
			renderTarget[i+1].attach(curwidth, curheight);
			renderTarget[i+1].setTexture(GL_COLOR_ATTACHMENT0, &bloom, false, i+1);
			quad.draw(GL_TRIANGLES);
		}
		quad.draw(GL_TRIANGLES);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	color.bind(0);
	bloom.bind(1);
	additional.bind(2);
	post.use();
	glUniform1f(post.getLoc("t"), t);
	quad.draw(GL_TRIANGLES);
	bind();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void PostProcess::bindUniforms()
{
	gaussian.use();
	glUniform1i(gaussian.getLoc("source"), 0);
	redux.use();
	glUniform1i(redux.getLoc("source"), 0);
	post.use();
	glUniform1i(post.getLoc("color"), 0);
	glUniform1i(post.getLoc("bloom"), 1);
	glUniform1i(post.getLoc("additional"), 2);
}