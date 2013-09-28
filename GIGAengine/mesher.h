
#ifndef MESHER_H
#define MESHER_H

class mesher {
	private:
		int size, maxPower;
		mesh quad, indmesh;
		texture3D histogram, *currentField;
		texture quadPositions, vertexPositions;
		target * targs, positionTarget;
		shader countVertices, histoBuild, positionInsert, positionSmooth;
		unsigned int quadCount, quadWidth, quadHeight;
	public:
		mesher(int maxPower);
		~mesher();
		int update(texture3D * distanceField, int power = -1);
		unsigned int getQuadCount();
		unsigned int getQuadWidth();
		unsigned int getQuadHeight();
		texture3D * getHistogram();
		texture * getPositionTexture();
		int draw();
};

#endif
