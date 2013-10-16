#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <array>
#include "objloader.h"

// from http://stackoverflow.com/a/236803
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

struct ObjFace {
	unsigned int vert_index[3];
	unsigned int texcoord_index[3];
	unsigned int normal_index[3];
};

struct ObjVert {
	float x;
	float y;
	float z;
};

// used to serialize vertices for opengl
struct OglVertex {
	float pos[3];
	float normal[3];
};

/// If a field has no index, writes 0 since Wavefront Obj format begins
/// indexing from 1 instead of 0.
void parseVertexIndices(std::string str, std::vector<unsigned int> &indices)
{

	std::vector<std::string> values;
	split(str.substr(0, std::string::npos), '/', values);

	if (values.size() < 3) {
		fprintf(stderr, "ObjLoader: face vertex has only %d indices", values.size());
		return;
	}

	for (std::string s : values) {
		if (s.length() == 0) {
			indices.push_back(0);
			continue;
		}

		indices.push_back(atoi(s.c_str()));
	}
}

void parseFaceIndices(std::string str, std::vector<ObjFace> &faces)
{
	std::vector<std::string> values;
	split(str.substr(1, std::string::npos), ' ', values);

	if (values.size() < 3) {
		fprintf(stderr, "ObjLoader: face listing has only %d vertices", values.size());
		return;
	}

	std::vector<unsigned int> indices;

	for (std::string s : values) {
		if (s.length() == 0)
			continue;

		parseVertexIndices(s, indices);
	}

	ObjFace face = { 
		{indices[0], indices[3 + 0], indices[2*3 + 0]},
		{indices[1], indices[3 + 1], indices[2*3 + 1]},
		{indices[2], indices[3 + 2], indices[2*3 + 2]},
	};

	faces.push_back(face);
}

char* readFileToString(const char* path)
{
	char *file_contents;
	long input_file_size;
	FILE *input_file;
	fopen_s(&input_file, path, "rb");
	fseek(input_file, 0, SEEK_END);
	input_file_size = ftell(input_file);
	rewind(input_file);
	//file_contents = (char*)malloc(input_file_size * (sizeof(char)));
	file_contents = new char[input_file_size];
	fread(file_contents, sizeof(char), input_file_size, input_file);
	fclose(input_file);

	return file_contents;
}


mesh* util::loadWavefrontObj(std::string path, bool verbose)
{
	char* text = readFileToString(path.c_str());

	std::stringstream stream(text);
	std::string to;

	int line = 0;

	if (text == NULL) {
		fprintf(stderr, "Couldn't read obj text from %s", path.c_str());
		return NULL;
	}

	std::vector<ObjVert> vertices;
	std::vector<ObjVert> normals;
	std::vector<ObjFace> faces;

	while(std::getline(stream, to, '\n')) {
		line++;
		// skip empty lines
		if (to.length() == 0) 
			continue;
		char first = to[0];

		if (first == '#')
			continue;

		ObjVert vert;

		switch (first) {
		case 'v':

			if (to[1] == 'n')  {
				sscanf_s(to.c_str(),"vn %f %f %f", &vert.x, &vert.y, &vert.z);
				normals.push_back(vert);

				if (verbose)
					printf("normal: %f %f %f\n", vert.x, vert.y, vert.z);
			} else {
				sscanf_s(to.c_str(),"v %f %f %f", &vert.x, &vert.y, &vert.z);
				vertices.push_back(vert);

				if (verbose)
					printf("vertex: %f %f %f\n", vert.x, vert.y, vert.z);
			}
			break;
		case 'f':
			parseFaceIndices(to, faces);
			break;
		default:
			break;
		}
	}

	int vertex_count = faces.size() * 3; // triangular faces
	OglVertex* vertex_array = new OglVertex[vertex_count];

	for (std::vector<ObjFace>::size_type i=0;i!=faces.size();i++) {
		ObjFace& face = faces[i];
		
		// loop through face vertices
		for (int u=0;u<3;u++) {
			OglVertex vert;	
			ObjVert objvert, objnormal;

			int vert_index = face.vert_index[u];
			int normal_index = face.normal_index[u];

			// if vertex index is 0, it means the respective index was 
			// not listed in the obj face row, so we fall back to 
			// default values
			if (vert_index > 0) {
				objvert = vertices.at(vert_index - 1);
			} else {
				objvert.x = objvert.y = objvert.z = 0.0f;
			}

			if (normal_index > 0) {
				objnormal = normals.at(normal_index - 1);
			} else {
				objnormal.x = objnormal.y = objnormal.z = 0.0f;
			}

			vert.pos[0] = objvert.x;
			vert.pos[1] = objvert.y;
			vert.pos[2] = objvert.z;
			vert.pos[3] = 1.0f;

			vert.normal[0] = objnormal.x;
			vert.normal[1] = objnormal.y;
			vert.normal[2] = objnormal.z;

			vertex_array[i*3 + u] = vert;
		}
	}

	mesh* newmesh = new mesh(MESH_EMPTY);
	newmesh->data(vertex_count, sizeof(OglVertex), vertex_array);
	newmesh->indices(0, nullptr);
	newmesh->pointer(0, 0, 3); // position data
	newmesh->pointer(1, 3, 3); // normals

	delete[] vertex_array;
	delete[] text;

	return newmesh;
}
