#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <bits/stdc++.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "sceneManager.h"

#define X first
#define Y second
#define point pair<float, float>
#define mp make_pair
#define pb push_back
#define PI 3.14159265359
#define ull unsigned long long

// game states
#define RUNNING 0
#define STOPPED 1
#define PAUSED 2

// game boundaries
#define X_LIM 4
#define Y_LIM 4
#define BACKGROUND_LVL -10.0
#define GAME_LVL BACKGROUND_LVL + 0.4

#define DEATH_RADIUS 0.4
#define ASTROID_STEP 0.14
#define SHIP_STEP    0.07

using namespace std;

struct Astroid{
	point location;

	void set(point loc){location = loc;}
};

struct Player{
	point location;
	ull score;

	void set(point loc, ull s = 0){
		location = loc;score = s;
	}
};

class GameManager{
private:
	SceneManager SM;

	vector<Astroid> astroids;
	Player player;

	int game_state;
	int interruptable;

	void game_screen();
	void pause_screen();
	void stop_screen();
	void reset_game();

	float distance(point a, point b);

public:
	GameManager();

	void initALL();
	void runGame();

	void timer_update_astroids(int v);
	void timer_add_astroids(int v);
	void timer_enable_interrupt(int v);

	bool KB_handler(unsigned char key, int x, int y);

	void resize_handler(int w, int h);
};

#endif
