#include <bits/stdc++.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "gameManager.h"

#define X first
#define Y second
#define point pair<float, float>
#define mp make_pair
#define pb push_back
#define PI 3.14159265359

using namespace std;

GameManager gm;

void display();
void KB_handler(unsigned char key, int x, int y);
void resize_handler(int w, int h);
void update(int v);
void add(int v);
void enableInterrupt(int v);

int main(int argc, char** argv){
	glutInit(&argc, argv); // initialize glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500); //Set the window size

	//Create the window
	glutCreateWindow("ASTROIDS");

	gm = GameManager();

	// set handlers for time, keyboard, resize
	glutDisplayFunc(display);
	glutKeyboardFunc(KB_handler);
	glutReshapeFunc(resize_handler);

	glutTimerFunc(0, update, 1);
	glutTimerFunc(0, add, 1);

	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0;
}


void display(){
	gm.runGame();
}

void KB_handler(unsigned char key, int x, int y){
	bool f = gm.KB_handler(key, x, y);
	if(f)glutTimerFunc(1000, enableInterrupt, 1);
	glutPostRedisplay();
}

void resize_handler(int w, int h){
	gm.resize_handler(w, h);
}

void update(int v){
	gm.timer_update_astroids(v);
	glutTimerFunc(100, update, 1); // repeat this each 100ms
	glutPostRedisplay();
}

void add(int v){
	gm.timer_add_astroids(v);
	glutTimerFunc(1000, add, 1); // repeat this each 1000ms
	glutPostRedisplay();
}

void enableInterrupt(int v){
	gm.timer_enable_interrupt(v);
	glutPostRedisplay();
}