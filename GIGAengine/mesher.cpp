
#include "main.h"

mesher::mesher(int maxPower) : maxPower(maxPower), quad(QUAD), histogram(2, 2, 2, 1, GL_NEAREST), countVertices(VCOUNT, SHADER, GEOMETRY_SHADER), histoBuild(HISTO, SHADER, GEOMETRY_SHADER), positionInsert(POSITION, SHADER), positionSmooth(SMOOTHPOS, SHADER), quadPositions(1,1,false,GL_NEAREST), vertexPositions(1,1,false,GL_NEAREST) {
	size = 1;
	for(int i = 0; i<maxPower; i++)
		size *= 2;
	
	targs = new target[maxPower+1];

	int k = size;
	for(int i = 0; i<maxPower+1; i++) {
		histogram.addmiplevel(k, k, k, 1, i, GL_NEAREST);
		targs[i].setTexture(GL_COLOR_ATTACHMENT0, &histogram, true, i);
		k /= 2;
	}

	int zero = 0, one = 1;
	countVertices.use();
	countVertices.setUniform("tex", &one, GINT);
	histoBuild.use();
	histoBuild.setUniform("tex", &zero, GINT);
	quadCount = 0;

	float data[] = {.1f, .3f, .6f, .8f};
	unsigned int inds[] = {0,1,2,0,2,3};
	indmesh.data(4, sizeof(float), data);
	indmesh.pointer(0, 0, 1);
	indmesh.indices(6, inds);
}

mesher::~mesher() {
	delete [] targs;
}

int mesher::update(texture3D * distanceField, int power) {
	if(power<0)
		power = maxPower;
	
	int one = 1, zero = 0; float fk = float(size);
	currentField = distanceField;
		
	targs[0].attach(size, size);
	countVertices.use();
	distanceField->bind(1);
	countVertices.setUniform("size", &fk, GFLOAT);
	quad.draw(GL_TRIANGLES, size);
	glFinish();
	
	histoBuild.use();
	histogram.bind(0);
	histoBuild.setUniform("first", &one, GINT);
	int k = size;

	for(int i = maxPower-power; i<power; i++) {
		k /= 2;
		fk = float(k);
		targs[i+1].attach(k,k);
		histoBuild.setUniform("size", &fk, GFLOAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, i);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL , i);

		quad.draw(GL_TRIANGLES, k);
		if(!i)
			histoBuild.setUniform("first", &zero, GINT);
		glFinish();
	}
	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, power);
	
	glGetTexImage(GL_TEXTURE_3D, power, GL_RED_INTEGER, GL_UNSIGNED_INT, &quadCount);
	glFinish();
	
	quadWidth = 512;
	quadHeight = quadCount/512+1;

	if(quadCount<=512) {
		quadWidth = quadCount;
		quadHeight = 1;
	}

	quadPositions.resize(quadWidth,quadHeight);
	vertexPositions.resize(quadWidth,quadHeight*4);

	positionTarget.attach(quadWidth,quadHeight);
	positionTarget.setTexture(GL_COLOR_ATTACHMENT0, &quadPositions, false);

	positionInsert.use();
	glUniform1f(positionInsert.getLoc("power"), float(power-1));
	glUniform1i(positionInsert.getLoc("histo"), 0);
	glUniform1i(positionInsert.getLoc("sdf"), 1);
	quad.draw(GL_TRIANGLES);

	positionTarget.attach(quadWidth,quadHeight*4);
	positionTarget.setTexture(GL_COLOR_ATTACHMENT0, &vertexPositions, false);
	
	quadPositions.bind(2);

	positionSmooth.use();
	glUniform1i(positionSmooth.getLoc("sdf"), 1);
	glUniform1i(positionSmooth.getLoc("quadpos"), 2);
	glUniform2f(positionSmooth.getLoc("count"), float(quadWidth), float(quadHeight));
	glUniform1f(positionSmooth.getLoc("power"), float(power-1));
	quad.draw(GL_TRIANGLES);
	glFinish();

	return 0;
}

unsigned int mesher::getQuadCount() {
	return quadCount;
}

unsigned int mesher::getQuadWidth() {
	return quadWidth;
}

unsigned int mesher::getQuadHeight() {
	return quadHeight;
}

texture3D * mesher::getHistogram() {
	return &histogram;
}

texture * mesher::getPositionTexture() {
	return &vertexPositions;
}

int mesher::draw() {
	indmesh.draw(GL_TRIANGLES, quadCount);
	return 0;
}