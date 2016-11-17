

//=============================================================================
// graphicsObject.c
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. generic class for graphics objects. 
//
//
// Description:
//--------------
//  sub classes of objects can be inherited.  The class does the main funcionality of setting and rendering objects.

//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================




#include "GraphicsObject.h"

/***************************************************************************/

GraphicsObject::GraphicsObject() : vtxVBO(-1), vao(-1), indVBO(-1)
, rollAngle(0)
, numIndices(0)
{
}

/***************************************************************************/



GraphicsObject::~GraphicsObject()
{
}


/***************************************************************************/



int GraphicsObject::createVAO(Shader shader)
{
	return 0;
}


/***************************************************************************/


int GraphicsObject::createVAO(Shader shader, Vertices vtx, Indices ind)
{
	int rc = 0;
	Vertex v;
	GLint location;		// location of the attributes in the shader;

	//create vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create vertex buffer object
	glGenBuffers(1, &vtxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vtxVBO);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(Vertex), vtx.data(), GL_STATIC_DRAW);

	//copy the vertex position
	location = glGetAttribLocation(shader.getProgId(), "vtxPos");
	if (location == -1) {
		rc = -1;
		goto err;
	}
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	//copy the vertex color
	location = glGetAttribLocation(shader.getProgId(), "vtxCol");
//	if (location == -1) {
	//	rc = -2;
		//goto err;
	//}
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

	/*
	//set the vertex normal
	location = glGetAttribLocation(shader.getProgId(), "vtxNormal");
	if (location == -1) {
		rc = -3;
		//DN	goto err;
	}
	else {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	}
	*/
	//set the vertex tex coordinates
	location = glGetAttribLocation(shader.getProgId(), "vtxCoord");
	if (location == -1) {
		rc = -3;
		//DN	goto err;
	}
	else {
		glEnableVertexAttribArray(location);
		int relAddress = (char *)v.texCoord - (char *)&v;

		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)relAddress);

		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	}

	//create index buffer
	glGenBuffers(1, &indVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(GLuint), ind.data(), GL_STATIC_DRAW);
	// store the number of indices
	numIndices = ind.size();

	//end creation
	glBindVertexArray(0);

	err:
	return(rc);
}



/*********************************************************************************/


int GraphicsObject::render(Shader shader)
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix

	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle,pitchAngle,yawAngle,1);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(position);
	modelMat = transMat * modelMat;

	// move matrix to shader
	shader.copyMatrixToShader(modelMat, "model");


	// bind the vao
	glBindVertexArray(vao);

	// draw using indices
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	// unbind the vao
	glBindVertexArray(0);

	return 0;
}


/*************************************************************/
// sets the initial orientation

void GraphicsObject::setInitialRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->rollAngle = rollAngle;
	this->pitchAngle = pitchAngle;
	this->yawAngle = yawAngle;

}

/*************************************************************************/

// set the initial position

void GraphicsObject::setInitialPosition(Vector3f position)
{
	this->position = position;
}

/*************************************************************************/

// set the initial position

void GraphicsObject::setInitialPosition(float x, float y, float z)
{
	this->position = Vector3f(x, y, z);
}

/*************************************************************/
// increment the courrent rotation by the given amounts

void GraphicsObject::incrementRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->rollAngle += rollAngle;
	this->pitchAngle += pitchAngle;
	this->yawAngle += yawAngle;

}

/*************************************************************************/

// increment positin by delta position

void GraphicsObject::incrementPosition(Vector3f deltaPosition)
{
	this->position += deltaPosition;
}

/*************************************************************************/

// increment position by delta

void GraphicsObject::incrementPosition(float deltaX, float deltaY, float deltaZ)
{
	this->position += Vector3f(deltaX, deltaY, deltaZ);
}


/*************************************************************************/

// set the initial position

void GraphicsObject::setScale(Vector3f scale)
{
	this->scale = scale;
}

/*************************************************************************/

// set the initial position

void GraphicsObject::setScale(float scaleX, float scaleY, float scaleZ)
{
	this->scale = Vector3f(scaleX, scaleY, scaleZ);
}
