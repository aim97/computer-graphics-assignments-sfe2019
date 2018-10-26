// shapes.cpp : This file contains the 'main' function. Program execution begins and ends there.

//#include "pch.h"
#include <bits/stdc++.h>
#include "polygon.h"

//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

// electron motion states
#define up    1
#define down  2
#define right 3
#define left  4
#define circuit_length (float)3.0
#define circuit_width  (float)1.0
int electron_state = 3;
float electron_charge = 1.0;
float electron_radius = 0.1;
float electron_speed = 0.1;
point electron_poition = mp(0.0, circuit_width);
polygon* a = new polygon(15, mp(0,0), electron_radius);

float x_position = 0;
float dx = 0.02;

//Called when a key is pressed
void handleKeypress(unsigned char key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates
	glutPostRedisplay();
	switch (key) {
		case GLUT_KEY_UP:
			printf("UP\n");fflush(stdout);
			//glscalef(electron_charge, electron_charge, electron_charge)
			electron_charge = min(2.0, electron_charge + 0.02);
			electron_radius = min(0.3, electron_radius + 0.02);
			electron_speed = min(0.3, electron_speed + 0.05);
			break;
		case GLUT_KEY_DOWN:
			printf("DOWN\n");fflush(stdout);
			electron_charge = max(0.5, electron_charge - 0.02);
			electron_radius = max(0.2, electron_radius - 0.02);
			electron_speed = max(0.05, electron_speed - 0.05);
			break;
		case 27: //Escape key
			printf("escape\n");
			exit(0); //Exit the program
			break;
	}
}

//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
}

//Called when the window is resized
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

void drawPolygon(point start, float l, int n){
    point tmp;        // create a tmp point
    vector<point> v;  // create a vector to be used to hold the points of the polygon
    v.pb(start);      // add the start point to the vector
	// angle : this is the slope angle of currently drawn line (created by last point and next one) with x-axis
	// theta : the angle between two consecutive lines in the same polygon
	// for n=4 --> theta=90, n=5 --> theta = 72, ...
    float angle, theta = 180 - ((n - 2) * 180) / n; 
    for(int i = 0;i < n - 1;i++){
        angle = i * theta; // get value of angle
        tmp.X = start.X + l * cos(angle * PI / 180); // compute X value of next point
        tmp.Y = start.Y + l * sin(angle * PI / 180); // compute Y value of next point
        v.pb(tmp);                                   // add computed point to vector v
        start = tmp;                                 // set start to hold the new computed point
    }
    
	// draw the polygon
    glBegin(GL_POLYGON);
        for(auto cur: v)
            glVertex3f(cur.X, cur.Y, -10.0);
    glEnd();
}

void drawScene() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	// draw scene objects
	float colnum = 1.8,
	      first_row = 2.2,
	      second_row = 0.0,
	      third_row = -2.2;
	point shift = mp(-0.3, -0.5);
	
	// hexagon
	drawPolygon(mp(colnum + shift.X,first_row + shift.Y), 1.0, 6);
	// octagon
	drawPolygon(mp(colnum + shift.X,second_row + shift.Y), 0.8, 8);
	// decagon
	drawPolygon(mp(colnum + shift.X,third_row + shift.Y), 0.6, 10);
	
	// pentagon
	drawPolygon(mp(-colnum + shift.X,first_row + shift.Y), 1.0, 5);
	// heptagon
	drawPolygon(mp(-colnum + shift.X,second_row + shift.Y), 0.8, 7);
	// nonagon
	drawPolygon(mp(-colnum + shift.X,third_row + shift.Y), 0.6, 9);


	glutSwapBuffers(); //Send the 3D scene to the screen
}

void tryThis(){
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	
	// draw circuit
	glPushMatrix();
	glBegin(GL_LINES);
	glLineWidth(3.5);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(-circuit_length, circuit_width, -10.0);glVertex3f(circuit_length, circuit_width, -10.0);
	glVertex3f(-circuit_length, -circuit_width, -10.0);glVertex3f(circuit_length, -circuit_width, -10.0);
	glVertex3f(-circuit_length, circuit_width, -10.0);glVertex3f(-circuit_length, -circuit_width, -10.0);
	glVertex3f(circuit_length, -circuit_width, -10.0);glVertex3f(circuit_length, circuit_width, -10.0);
	glEnd();
	glPopMatrix();


	// draw electron
	glPushMatrix();
	glTranslatef(electron_poition.X, electron_poition.Y, 0.0);
	glColor3f(1.0,1.0,0.0);
	glScalef(electron_charge, electron_charge, electron_charge);
	a->draw();
	glPopMatrix();
	
	// draw grid
	glPushMatrix();
	glBegin(GL_LINES);
	glLineWidth(0.1);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(-4.0, 0.0, -10.0);glVertex3f(4.0, 0, -10.0);
	glVertex3f(-4.0, -1.0, -10.0);glVertex3f(4.0, -1.0, -10.0);
	glVertex3f(-4.0, 1.0, -10.0);glVertex3f(4.0, 1.0, -10.0);
	glVertex3f(-4.0, -2.0, -10.0);glVertex3f(4.0, -2.0, -10.0);
	glVertex3f(-4.0, 2.0, -10.0);glVertex3f(4.0, 2.0, -10.0);
	glVertex3f(-4.0, -3.0, -10.0);glVertex3f(4.0, -3.0, -10.0);
	glVertex3f(-4.0, 3.0, -10.0);glVertex3f(4.0, 3.0, -10.0);

	glVertex3f(0.0, -4.0, -10.0);glVertex3f(0.0, 4.0, -10.0);
	glVertex3f(1.0, -4.0, -10.0);glVertex3f(1.0, 4.0, -10.0);
	glVertex3f(-1.0, -4.0, -10.0);glVertex3f(-1.0, 4.0, -10.0);
	glVertex3f(2.0, -4.0, -10.0);glVertex3f(2.0, 4.0, -10.0);
	glVertex3f(-2.0, -4.0, -10.0);glVertex3f(-2.0, 4.0, -10.0);
	glVertex3f(3.0, -4.0, -10.0);glVertex3f(3.0, 4.0, -10.0);
	glVertex3f(-3.0, -4.0, -10.0);glVertex3f(-3.0, 4.0, -10.0);
	glEnd();
	glPopMatrix();


	glutSwapBuffers(); //Send the 3D scene to the screen
}

void timer(int){
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);

	switch(electron_state){
		case left:
			electron_poition.X = max(electron_poition.X - electron_speed, -circuit_length);
			if(fabs(electron_poition.X + circuit_length) <= 0.01)electron_state = up;
			break;
		case up:
			electron_poition.Y = min(electron_poition.Y + electron_speed, circuit_width);
			if(fabs(electron_poition.Y - circuit_width) <= 0.01)electron_state = right;
			break;
		case right:
			electron_poition.X = min(electron_poition.X + electron_speed, circuit_length);
			if(fabs(electron_poition.X - circuit_length) <= 0.01)electron_state = down;
			break;
		case down:
			electron_poition.Y = max(electron_poition.Y - electron_speed, -circuit_width);
			if(fabs(electron_poition.Y + circuit_width) <= 0.01)electron_state = left;
			break;
	}
	//x_position += dx;
	//if(x_position > 3.0 || x_position < -3.0)dx = -dx;
}


int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400); //Set the window size

	//Create the window
	glutCreateWindow("Basic Shapes");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(tryThis);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(0, timer, 0);

	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}








void drawGrid(int x_limit, int y_limit, RGB* color){
	// rule : draws a grid for visual and testing purposes only.
	// in   : x_limit --> max value to be displayed for x, e.g. -x_limit <= x <= x_limit
	//        x_limit --> max value to be displayed for y, e.g. -y_limit <= y <= y_limit
	//        color   --> color of lines in the grid
	// out  : void
	glPushMatrix();
	glBegin(GL_LINES);
	glLineWidth(0.1);
	for(int i = -x_limit;i <= x_limit;i++){
		glVertex3f(i, y_limit, -10.0);
		glVertex3f(i, -y_limit, -10.0);
	}

	for(int i = -y_limit;i <= y_limit;i++){
		glVertex3f(x_limit, i, -10.0);
		glVertex3f(-x_limit, i, -10.0);
	}
	glPopMatrix();
}