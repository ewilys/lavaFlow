// complex_object.cpp : Defines the entry point for the console application.
//


#include "Solution.h"


int main(int argc, char** argv)
{
	Solution sol;

	glutInit(&argc, argv);

	sol.initOpenGL();
	sol.initSolution();
	Solution::setSolution(&sol);

	glutMainLoop();



	return 0;
}

