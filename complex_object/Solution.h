#pragma once
#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER



//=============================================================================
// solution.h
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. solution framework for assignments
//
//
// Description:
//--------------
// A simple framework for drawing objecs 
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




#include "GL/glew.h"
#include "GL/glut.h"

#include "Shader.h"
#include "sphere.h"
#include "surface.h"

#include "camera.h"
#include "Texture.h"
#include "SkyBox.h"
#include "CParticle.h"
#include <random>
#include <functional>


#define UPDATE_RENDERRED_OBJECTS 1000
#define FRAME_TIME 30

#define NUM_PARTICLE_ATTRIBUTES 4
#define GENERATE_NEW_PARTICLE 3
#define MAX_PARTICLES_ON_SCENE 2500
#define NEW_PARTICLES 5
#define PARTICLE_SIZE 7


class Solution
{
public:
	Solution();
	~Solution();
	// initializing the opengl functions and windows
	int initOpenGL();


	// static callback function 
	static void renderCB();
	static void keyboardCB(unsigned char key, int x, int y);
	static void specialKeyboardCB(int key, int x, int y);
	static void winResizeCB(int width, int height);
	static void timerCB(int operation);

	static void setSolution(Solution * _sol);

	// generaL shader that will be used by all objects
	// initialization of the solution
	int initSolution();
	bool InitalizeParticleSystem();

	void UpdateParticles();
	Vector3f ApplyBoundaries(Vector3f pos);
	Vector3f randVelocity();
	float updateTemp(Vector3f pos);

	Shader shader;
	Shader particleShader;
	Camera cam;
	SkyBox skybox;
	Surface testSurface;
	Vector3f volcanoCenter;
	int numFrames;
	int time=0;
	int factor;
	static Solution *sol;
	Texture volcanoTex;
	Texture lavaTex;
	
	Matrix4f matProjection, matView;

	CParticle ParticlesContainer[MAX_PARTICLES_ON_SCENE];
	CParticle gpuParticleContainer[MAX_PARTICLES_ON_SCENE];
	const Vector3f Gravity = Vector3f(0, -9.81, 0);
	Vector3f GenPosition ;
	const Vector3f GenVelocity = Vector3f(0, 0, 0);
	const Vector4f GenColor = Vector4f(1.0, 0, 0, 1.0);
	const float lifeTime = 250.0f;
	const float tempGen = 0.5f;
	

	GLuint triVAO = -1;
	GLuint vbo;
	int ParticlesCount;
	int nextParticleToUse = 100;
	int firstUsedParticle = 0;



	void render();
	void keyboard(unsigned char key, int x, int y);
	void specialKeyboard(int key, int x, int y);
	void winResize(int width, int height);
	int timer(int operation);
	int FindUnusedParticle();
	int FindFirstUsedParticle();

	int updateObjects(int numFrames);
	float gRandF(float min, float max);
	
};



#endif