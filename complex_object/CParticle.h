
#pragma once

#ifndef CParticle_HEADER
#define CParticle_HEADER


//=============================================================================
// cparticle.h
//
//Author: http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=26
//-----------------------------------------------------
//

//=============================================================================


#include "GL/glew.h"
#include "GL/glut.h"

#include "Shader.h"

#include "camera.h"
#include "GL/nuss_math.h"
#include "GL/nuss_vector.h"



class CParticle
{
public:
	Vector3f vPosition;
	Vector3f vVelocity;
	Vector4f vColor;
	float fLifeTime;
	//float temperature;
	//float density;
};

#endif
