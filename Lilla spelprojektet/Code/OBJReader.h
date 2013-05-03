#pragma once
#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
#include <fstream>
#include <iostream>

using namespace std;

class OBJReader
{

public:
	OBJReader(void);
	~OBJReader(void);

	MESH_PNUV* getOBJfromFile(string fileName, int &nrOfVerts);

private:
	bool ReadFileCounts(string fileName, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
	MESH_PNUV* LoadDataStructures(string fileName, int vertexCount, int textureCount, int normalCount, int faceCount);
};

