#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <bits/stdc++.h>
#include "imageloader.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define X first
#define Y second
#define point pair<float, float>
#define mp make_pair
#define pb push_back
#define PI 3.14159265359

#define BACKGROUND_TEX 1
#define ASTROID_TEX 2
#define SHIP_TEX 3

using namespace std;

class SceneManager{
private:
	map<int, GLuint> tex;

	void draw_circle(float r);
	GLuint loadTexture(Image* image);

public:
	void draw_grid(int xlim, int ylim);
	void glut_init();
	void draw_astroid();
	void draw_space_ship();
	void draw_back_ground(float lvl);
	void add_lighting(float li);
	void render_bit_map(float x, float y, float z, void *font, string s);
};

#endif

/**
this class is responsible for drawing objects in the scene
it should implement methods responsible fot drawing objects from the scene
like the astroids, space ship, background, lighting, .. etc
it simply breaks the scene into a set of components that can be manipulated by the
controller class.

*/
