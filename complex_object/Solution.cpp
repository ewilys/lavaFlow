

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

	// create the shader object
	rc = shader.createShaderProgram("complexObjects1.vert", "complexObjects1.frag");
	if (rc != 0) {
		fprintf(stderr, "Error in generating shader (solution)\n");
		rc = -1; 
		goto err;
	}

	cam.setCamera(viewerPosition, lookAtPoint, upVector);
	cam.setPerspectiveView(110, 1, 0.01f, 1000);

	Surface::createSurface(30, 30,0,1,0,1,vtx, ind);
	testSurface.createVAO(shader, vtx, ind);
	testSurface.setInitialPosition(volcanoCenter.x, volcanoCenter.y, volcanoCenter.z);
	testSurface.setInitialRotations(0, 0, 0);
	testSurface.setScale(25, 1, 25);


	//set volcano texture
	volcanoTex.loadTextureImage("volcan.jpeg", GL_TEXTURE_2D);

	//skybox initialization
	skybox.init("skybox.vert", "skybox.frag");
	skybox.loadTextureImages(sbTextureNameSunnyDay);


	factor = 1;
	err:
	return 0;
}


/**********************************************************************/

void Solution::setSolution(Solution * _sol)
{
	Solution::sol = _sol;
}



/************************************************************/

// render function.  


void Solution::render()
{

	
	Matrix4f viewMat, projMat;
	


	// use the created shader
	shader.useProgram(1);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	skybox.render(cam);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexHandle());

	// set the camera matrix
	viewMat = cam.getViewMatrix(NULL);
	// move matrix to shader
	shader.copyMatrixToShader(viewMat, "view");


	// set the projection matrix
	projMat = cam.getProjectionMatrix(NULL);
	// move matrix to shader
	shader.copyMatrixToShader(projMat, "projection");

	shader.copyFloatVectorToShader((float*)&volcanoCenter, 1, 3, "center");
	volcanoTex.setTextureSampler(shader, "texSampler", GL_TEXTURE1);

	

	// render the objects

	testSurface.render(shader);
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
	// recall that this will be carried out in the model space
	

	glutPostRedisplay();
	return 0;
}
