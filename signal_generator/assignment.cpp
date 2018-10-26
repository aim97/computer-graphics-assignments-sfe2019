#include <bits/stdc++.h>
#include "polygon.h"
#include "electric_signal.h"

//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define window_width 800
#define window_length 500

using namespace std;
Signal* s = new Signal(0, 1.0, 0.0);

struct RGB{
	float red, green, blue;

	RGB(float r, float g, float b){
		red = r;green = g;blue = b;
	}
};

// helper funtions
float distance(point a, point b);

// interrupt functions
void handleKeypress(unsigned char key, int x, int y);
void handleKeypress(int key, int x, int y);
void handleResize(int w, int h);

// drawing functions
void drawGrid(int x_limit, int y_limit, RGB* color);
void initRendering();
void tryThis();

void timer(int);

int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_length); //Set the window size

	//Create the window
	glutCreateWindow("Basic Shapes");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(tryThis);
	//glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(0, timer, 0);

	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}

void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
}

void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		1.0,                   //The near z clipping coordinate
		200.0);                //The far z clipping coordinate
}


void handleKeypress(unsigned char key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates
	glutPostRedisplay();
	switch (key){
		case GLUT_KEY_UP:
			printf("UP\n");
			s->moveUp();
			printf("dfghg\n");
			exit(0);
			break;
		case GLUT_KEY_DOWN:
			printf("Down\n");
			s->moveDown();
			break;
		case GLUT_KEY_LEFT:
			printf("Left\n");
			s->decreaseAmplification();
			break;
		case GLUT_KEY_RIGHT:
			printf("Right\n");
			s->increaseAmplification();
			break;
		case 27: //Escape key
			printf("escape\n");
			exit(0); //Exit the program
			break;
	}
}

void handleKeypress(int key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates
	glutPostRedisplay();
	switch (key){
		case GLUT_KEY_UP:
			printf("UP\n");
			s->moveUp();
			break;
		case GLUT_KEY_DOWN:
			printf("Down\n");
			s->moveDown();
			break;
		case GLUT_KEY_LEFT:
			printf("Left\n");
			s->decreaseAmplification();
			break;
		case GLUT_KEY_RIGHT:
			printf("Right\n");
			s->increaseAmplification();
			break;
		case 27: //Escape key
			printf("escape\n");
			exit(0); //Exit the program
			break;
	}
}

void timer(int){
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
	s->forward();
	//printf("______________________________________________\n");
}

void tryThis(){
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	// draw signal
	s->display();
	// draw grid
	drawGrid(6, 4, new RGB(0.5, 0.5, 0.0));
	
	glutSwapBuffers(); //Send the 3D scene to the screen
}

void drawGrid(int x_limit, int y_limit, RGB* color){
	// rule : draws a grid for visual and testing purposes only.
	// in   : x_limit --> max value to be displayed for x, e.g. -x_limit <= x <= x_limit
	//        x_limit --> max value to be displayed for y, e.g. -y_limit <= y <= y_limit
	//        color   --> color of lines in the grid
	// out  : void
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(color->red, color->green, color->blue);
	glLineWidth(0.1);
	for(int i = -x_limit;i <= x_limit;i++){
		glVertex3f(i, y_limit, -10.0);
		glVertex3f(i, -y_limit, -10.0);
	}

	for(int i = -y_limit;i <= y_limit;i++){
		glVertex3f(x_limit, i, -10.0);
		glVertex3f(-x_limit, i, -10.0);
	}
	glEnd();
	glPopMatrix();
}
