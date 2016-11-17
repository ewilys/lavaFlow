#pragma once

#include "GL/glew.h"
#include "GL/glut.h"
#include "geometry_common.h"
#include "Shader.h"



class Texture
{
	
private:
	GLuint texId;		// the texture handle
	int textureUnit;	// the texture unit which will be used for the sampler
	GLuint texTarget;	// target type;


public:
	Texture();
	~Texture();

	int loadTextureImage(char *imageFileName, GLuint texTarget);
	int loadTextureImages(char ** texFileName);		// loads a cube texture images
	int loadColourTexture(void);					// loads a cube colour textures
	void bindToTextureUnit(GLuint textureType, int textureUnit);
	GLuint setTextureSampler(Shader shader, char *sampleName, GLuint textureUnit);

	GLuint getTexId();
};

