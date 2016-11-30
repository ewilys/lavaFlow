

//=============================================================================
// solution.c
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



#define NORMAL_SPEED 0.5
#define MAX_SPEED 2.0




#include "Solution.h"

Solution *Solution::sol = NULL;

/****************************************************************************/

Solution::Solution() : numFrames(0)

{
}

/*************************************************************************/


Solution::~Solution()
{
	printf("\n exiting the progam gracefully\n");

}

/******************************************************************************/

#include "GL/nuss_math.h"
// initializing the opengl functions and windows
int Solution::initOpenGL()
{
	//initialize OpenGL
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 400);
	glutCreateWindow("Lava Flow bis");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	glutDisplayFunc(Solution::renderCB);
	glutReshapeFunc(Solution::winResizeCB);
	glutKeyboardFunc(Solution::keyboardCB);
	glutSpecialFunc(Solution::specialKeyboardCB);
	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);

	glewInit();

	gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);
	GLfloat model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, model);
	Matrix4f m;
	m = Matrix4f::cameraMatrix(Vector3f(0, 0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0));

	return 0;
}

/************************************************************/

// render callback function.  This is a static funcion


void Solution::renderCB()
{

	sol->render();
	
}


/************************************************************/

// keyboard callback function.  This is a static funcion


void Solution::keyboardCB(unsigned char key, int x, int y)
{
	sol->keyboard(key, x, y);
}


/************************************************************/

// special keyboard callback function.  This is a static funcion



void Solution::specialKeyboardCB(int key, int x, int y)
{
	sol->specialKeyboard(key, x, y);
}


/************************************************************/

// window resize callback function.  This is a static funcion



void Solution::winResizeCB(int width, int height)
{
	sol->winResize(width, height);
}

/************************************************************/

// timer  callback function.  This is a static funcion


void Solution::timerCB(int operation)
{

	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);	
	sol->timer(operation);

}


/************************************************************/

// timrt  function.  


int Solution::timer(int operation)
{
	numFrames++;
	switch (operation) {
	case UPDATE_RENDERRED_OBJECTS:
		updateObjects(numFrames);
		break;
	default:
		break;
	}
	return(0);
}




/******************************************************************************/
// initialization of the solution
int Solution::initSolution()
{
	int rc;
	Vertices vtx;
	Indices ind;

	char *sbTextureNameSunnyDay[6] = {
		"TropicalSunnyDayLeft2048.png",
		"TropicalSunnyDayRight2048.png",
		"TropicalSunnyDayUp2048.png",
		"TropicalSunnyDayDown2048.png",
		"TropicalSunnyDayFront2048.png",
		"TropicalSunnyDayBack2048.png" };


	volcanoCenter = Vector3f(100, 0, 100);
	Vector3f viewerPosition = Vector3f(140, 50, 140);
	Vector3f lookAtPoint = volcanoCenter;
	Vector3f upVector = Vector3f(0, 1, 0);

	GenPosition = volcanoCenter;

	rc = particleShader.createShaderProgram("particle.vert", "particle.frag");
	if (rc != 0) {
		fprintf(stderr, "Error in generating particle shader (solution)\n");
		rc = -1;
		goto err;
	}
	// create the shader object
	rc = shader.createShaderProgram("complexObjects1.vert", "complexObjects1.frag");
	if (rc != 0) {
		fprintf(stderr, "Error in generating shader (solution)\n");
		rc = -1; 
		goto err;
	}

	

	cam.setCamera(viewerPosition, lookAtPoint, upVector);
	//cam.setPerspectiveView(110, 1, 0.01f, 1000);

	Surface::createSurface(30, 30,0,1,0,1,vtx, ind);
	testSurface.createVAO(shader, vtx, ind);
	testSurface.setInitialPosition(volcanoCenter.x, volcanoCenter.y, volcanoCenter.z);
	testSurface.setInitialRotations(0, 0, 0);
	testSurface.setScale(25, 1, 25);


	//set volcano texture
	volcanoTex.loadTextureImage("volcan.jpeg", GL_TEXTURE_2D);

	//skybox initialization
	//skybox.init("skybox.vert", "skybox.frag");
	//skybox.loadTextureImages(sbTextureNameSunnyDay);


	factor = 1;
	err:
	return 0;
}


/**********************************************************************/

void Solution::setSolution(Solution * _sol)
{
	Solution::sol = _sol;
}

/********************************************************************/

bool Solution::InitalizeParticleSystem()
{

	CParticle v;
	ParticlesCount = 1;

	v.vPosition = GenPosition;
	v.vVelocity = GenVelocity;
	v.vColor = GenColor;
	v.fLifeTime = lifeTime;

	ParticlesContainer[0] = v;
	gpuParticleContainer[0] = v;

	// obtain a handle to the VAO 
	glGenVertexArrays(1, &triVAO);

	// bind the VAO in order to indicate which VAO is currently used
	glBindVertexArray(triVAO);

	// next we follow the same process of generating a VBO.  All the steps are stord in the VAO

	// get a handle to the vbo
	glGenBuffers(1, &vbo);

	// indicate that which vbo to be used
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// provide the vertex array to be stored in the VBO 
	glBufferData(GL_ARRAY_BUFFER, sizeof(CParticle)*MAX_PARTICLES_ON_SCENE, gpuParticleContainer, GL_STATIC_DRAW);

	// bind the vertx attributes with the shader
	GLuint positionLoc = glGetAttribLocation(particleShader.getProgId(), "vtxPos");
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CParticle), (void *)((char *)&v.vPosition - (char *)&v));

	GLuint colourLoc = glGetAttribLocation(particleShader.getProgId(), "vtxColour");
	glEnableVertexAttribArray(colourLoc);
	glVertexAttribPointer(colourLoc, 4, GL_FLOAT, GL_FALSE, sizeof(CParticle), (void *)((char *)&v.vColor - (char *)&v));

	// disable the vertex array from being current (stop recording in this case)
	glEnableVertexAttribArray(0);

	// disable the vbo from being current
	glBindVertexArray(0);

	return(0);
}
/***********************************************************
//generate a float in range min-max 
float Solution::gRandF(float min, float max){
	//std::mt19937 generator;
	//std::uniform_real_distribution<float> uniform_distribution(min, max);
	//auto my_rand = std::bind(uniform_distribution, generator);

	//return my_rand();
}
*/

int Solution::FindUnusedParticle(){

	nextParticleToUse = (nextParticleToUse + 1) % MAX_PARTICLES_ON_SCENE;
	if (nextParticleToUse == firstUsedParticle && ParticlesContainer[firstUsedParticle].fLifeTime > 0.0){
		ParticlesContainer[firstUsedParticle].fLifeTime = 0.0;
		return firstUsedParticle;
	}


	return nextParticleToUse;
}

int Solution::FindFirstUsedParticle(){
	if (firstUsedParticle == MAX_PARTICLES_ON_SCENE - 1 && ParticlesContainer[firstUsedParticle].fLifeTime <= 0.0)firstUsedParticle = 0;
	for (int i = firstUsedParticle; i<MAX_PARTICLES_ON_SCENE; i++){
		if (ParticlesContainer[i].fLifeTime > 0){
			firstUsedParticle = i;
			return i;
		}
	}


	return 0; // All particles are taken, override the first one
}


/***********************************************************/

void Solution::UpdateParticles()
{
	//update last particle used
	nextParticleToUse = FindUnusedParticle();
	//printf("nextParticleToUse %i \n", nextParticleToUse);

	//update 1st particle used
	firstUsedParticle = FindFirstUsedParticle();
	//printf("firstUsedParticle %i \n", firstUsedParticle);

	//nb particles for previous 
	int nbParticle;
	if (nextParticleToUse>firstUsedParticle)nbParticle = nextParticleToUse - firstUsedParticle;
	else{ nbParticle = MAX_PARTICLES_ON_SCENE - firstUsedParticle + nextParticleToUse; }


	//nb particles to render for this iteration
	ParticlesCount = 0;


	float delta = 0.1;
	for (int i = firstUsedParticle; i<firstUsedParticle + nbParticle; i++){

		CParticle& p = ParticlesContainer[i%MAX_PARTICLES_ON_SCENE]; // shortcut

		if (p.fLifeTime - delta > 0.0f){

			// Decrease life
			p.fLifeTime -= delta;


			// Simulate simple physics : gravity only, no collisions
			p.vVelocity += Gravity * (float)delta * 0.005f;
			p.vPosition += p.vVelocity * (float)delta;


			// Fill the GPU buffer
			gpuParticleContainer[ParticlesCount] = p;
			ParticlesCount += 1;


		}
		else{
			//particle dies
			p.fLifeTime = 0.0;

		}
	}
	printf("time %i\n", time);
	if (time == GENERATE_NEW_PARTICLE){
		time = 0;
		//create new particle
		for (int i = 0; i < NEW_PARTICLES; i++){
			CParticle v;


			v.vPosition = Vector3f(GenPosition.x + i*PARTICLE_SIZE, GenPosition.y, GenPosition.z);
			v.vVelocity = GenVelocity;
			v.vColor = GenColor;
			v.fLifeTime = lifeTime;

			ParticlesContainer[nextParticleToUse] = v;

			ParticlesCount += 1;

			gpuParticleContainer[ParticlesCount] = v;
			nextParticleToUse = FindUnusedParticle();
			printf("nextParticleToUse %i\n", nextParticleToUse);

		}
	}
	printf("particle count %i\n", ParticlesCount);


	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//update the data in the VBO

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(CParticle)*ParticlesCount, gpuParticleContainer);

	// disable the vbo from being current
	//glBindVertexArray(0);

}
/************************************************************/
/********************************************************************/
// render function.  


void Solution::render()
{

	
	Matrix4f viewMat, projMat;
	



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/*
	skybox.render(cam);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexHandle());
	*/
	// use the created shader
	shader.useProgram(1);
	// set the camera matrix
	viewMat = cam.getViewMatrix(NULL);
	// move matrix to shader
	shader.copyMatrixToShader(viewMat, "view");


	// set the projection matrix
	projMat = cam.getProjectionMatrix(NULL);
	// move matrix to shader
	shader.copyMatrixToShader(projMat, "projection");

	shader.copyFloatVectorToShader((float*)&volcanoCenter, 1, 3, "center");
	glActiveTexture(GL_TEXTURE1);
	volcanoTex.setTextureSampler(shader, "texSampler", GL_TEXTURE1);

	// render volcano

	testSurface.render(shader);

	//render particles
	
	particleShader.useProgram(1);
	particleShader.copyMatrixToShader(viewMat, "view");
	particleShader.copyMatrixToShader(projMat, "projection");
	
	// bind the VAO buffer
	glBindVertexArray(triVAO);

	glPointSize(PARTICLE_SIZE);
	glDrawArrays(GL_POINTS, 0, ParticlesCount);

	glBindVertexArray(0);
	
	
	glutSwapBuffers();
}

/************************************************************/

// keyboard handling function. 


void Solution::keyboard(unsigned char key, int x, int y)
{
	static int nc = 0;
	float dx, dy;
	nc++;
	//printf("speed :%f ", cam.getSpeed);
	switch (key) {
	case 033:
	case 'Q':
	case 'q':
		this->~Solution();
		exit(1);
		break;
	case 'w':
		nc++;
		cam.moveForward(NORMAL_SPEED*factor);
		break;
	case 's':
		cam.moveBackward(NORMAL_SPEED*factor);
		break;
	case 'u':
		nc++;
		cam.moveUp(NORMAL_SPEED*factor);
		break;
	case 'U':
		cam.moveDown(NORMAL_SPEED*factor);
		break;
	case 'a':
		cam.roll(1);
		//printf("I'm here \n");
		break;
	case 'd':
		cam.roll(-1);
		break;
	case 'g':
		cam.moveRight(NORMAL_SPEED*factor);
		break;
	case 'G':
		cam.moveLeft(NORMAL_SPEED*factor);
		break;
	case 'z':
		cam.zoomIn();
		break;
	case 'Z':
		cam.zoomOut();
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		factor = key - '0';
		break;
	
	}
	//glutPostRedisplay();

}


/************************************************************/

// special keyboard handling  function.  



void Solution::specialKeyboard(int key, int x, int y)
{
	switch (key) {
	case 033:
	case 'Q':
	case 'q':
		exit(1);
		break;
	case GLUT_KEY_LEFT:
		cam.yaw(1);
		break;
	case GLUT_KEY_UP:
		cam.pitch(1);
		break;
	case GLUT_KEY_RIGHT:
		cam.yaw(-1);
		break;
	case GLUT_KEY_DOWN:
		cam.pitch(-1);
		break;

	}
}

/************************************************************/

// window resize handling function.  



void Solution::winResize(int width, int height)
{

	glViewport(0, 0, width, height);

}

/***********************************************************/
// update the state of the objects

int Solution::updateObjects(int numFrames)
{
	time++;
	UpdateParticles();

	glutPostRedisplay();
	return 0;
}
