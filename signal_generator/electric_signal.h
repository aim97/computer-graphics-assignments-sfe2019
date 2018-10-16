#ifndef ELECTRIC_SIGNAL_H
#define ELECTRIC_SIGNAL_H

#include <bits/stdc++.h>
#include "polygon.h"
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

#define pb push_back

#define SIN_SIGNAL 0

class Signal{
private:
	vector<float> y_points;
	float params[100];
	float lvl;
	float step = 0.0;
	int type = 0;

	// a * sin(b * (x - c)) + d
	void handle_sin();
public:
	Signal(float lvl,int type, ...);
	void forward();
	void reset();
	void display();
};

#include "electric_signal.cpp"

#endif