#ifndef GEOMETRY_COMMON
#define GEOMETRY_COMMON 1


#include <vector>

#include "GL/nuss_math.h"


struct Vertex {
	Vertex(){}
	Vertex(const Vector4f& _pos);
	Vertex(const Vector4f _pos, const Vector4f _col){
		memcpy(pos, (void *)&_pos, sizeof(pos));
		memcpy(col, (void *)&_col, sizeof(col));
	}

	float pos[4];
	float col[4];
};

typedef struct Vertex Vertex;



typedef std::vector<Vertex> Vertices;
typedef std::vector<unsigned int>	Indices;

#endif