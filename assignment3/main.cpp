#include <GL/glew.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
#include <GLFW/glfw3.h>
using namespace std;

#define point pair<float, float>
#define X first
#define Y second
#define mp make_pair
#define pb push_back

// GAME STATES
#define RUNNING 0
#define STOPED 1

// astroids motion rates
#define ASTROID_STEP_TIME 5
#define ASTROID_ADD_TIME 15

// creating the window pointer
int game_state = RUNNING;
GLFWwindow *window;

float screenWidth = 700;
float screenHieght = 700;

// astroids related data
vector<point> astroids;
int astroid_step = ASTROID_STEP_TIME, astroid_new = ASTROID_ADD_TIME;

// player related data
point player_location = mp(0.0, -0.8);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void drawCircle(float);
void error_callback(int error, const char* description);
void draw_astroid();
void game_screen();
void stop_screen();
void renderbitmap(float x, float y, float z, void *font, string s);
void positions_update(int v = 1);
void add_astroids(int v = 1);
void reset_game();

int main(int argc, char** argv) {
	srand( (unsigned)time( NULL ) );
	//Initialize GLUT  
	glutInit(&argc, argv);  // argc and argv are command line arguments  

	//initilize library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR : glfw couldn't be initilized\nprogram terminated\n");
		return -1;
	}

	// setting call error back handler for glfw
	glfwSetErrorCallback(error_callback);

	// --------------------------------------------------------------------------------------
	// setting the necessary versions of openGL used when compiling and executing the program
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// ---------------------------------------------------------------------------------------

	// ---------------------------------------------------------------------------------------
	//Create Window
	window = glfwCreateWindow(screenWidth, screenHieght, "GLFW demo", NULL, NULL);
	if (!window){
		fprintf(stderr, "ERROR : program window couldn't be rendered\nprogram terminated\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback); // set the key press handler for created window
	// ---------------------------------------------------------------------------------------
	

	glViewport(0.0f, 0.0f, screenWidth, screenHieght); //draw openGl in pixels 
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		switch(game_state){
			case RUNNING:
				game_screen();
				break;
			case STOPED:
				stop_screen();
				break;
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

void drawCircle(float r) {
	float a;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++){
		a = i * 3.142 / 180;
		glVertex3f(r*cos(a), r*sin(a), 0.0);
	}
	glEnd();
}

void draw_astroid(){
	glColor3f(0.5f, 0.5f, 0.0f);
	drawCircle(0.08);
	glColor3f(0.5f, 0.0f, 0.0f);
	drawCircle(0.05);
}

void draw_player(){
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 0.5);
	glVertex3f(-0.07, 0, 0);
	glVertex3f(0.07, 0, 0);
	glVertex3f(0, 0.03, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-0.02, 0, 0);
	glVertex3f(0.02, 0, 0);
	glVertex3f(0, 0.1, 0);
	glEnd();
}


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    	printf("closing window");
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }else if(key == GLFW_KEY_RIGHT){
    	player_location.X = min(player_location.X + 0.02, 0.9);
    }else if(key == GLFW_KEY_LEFT){
    	player_location.X = max(player_location.X - 0.02, -0.9);
    }else if((key == GLFW_KEY_KP_ENTER || key == GLFW_KEY_ENTER)  && game_state == STOPED){
    	reset_game();
    }
}

void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

void renderbitmap(float x, float y, float z, void *font, string s) {
	glRasterPos3f(x, y, z);
	for (int i = 0; i < s.size(); i++) {
		glutBitmapCharacter(font, (int)s[i]);
	}
}


void game_screen(){
	if(astroid_new > 0)astroid_new--;
	else{
		add_astroids();
		astroid_new = ASTROID_ADD_TIME;
	}

	if(astroid_step > 0) astroid_step--;
	else{
		positions_update();
		astroid_step = ASTROID_STEP_TIME;
	}

	// display astroids
	for(auto i : astroids){
		glPushMatrix();
		glTranslatef(i.X, i.Y, 0.0);
		draw_astroid();
		glPopMatrix();
	}
	// display player vehicle
	glPushMatrix();
	glTranslatef(player_location.X, player_location.Y, 0.0);
	draw_player();
	glPopMatrix();
}

void stop_screen(){
	glColor3f(1.0, 1.0, 1.0);
	renderbitmap(-0.2,0,0, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
	glColor3f(0.0, 1.0, 0.0);
	renderbitmap(-0.5,-0.1,0, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS ENTER TO TRY AGAIN");
	renderbitmap(-0.5,-0.2,0, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS ESC TO EXIT");
}

float distance(point a, point b){
	return sqrt((a.X-b.X)*(a.X-b.X) + (a.Y-b.Y)*(a.Y-b.Y));
}

void positions_update(int v){
	// update astroids positions
	switch(game_state){
		case RUNNING:
			for(auto i = astroids.begin();i != astroids.end();i++){
				i->Y -= 0.07;
				if(distance(player_location, *i) < 0.1){
					game_state = STOPED;
					break;
				}
				if(i->Y < -0.9)astroids.erase(i);
			}
			break;
		case STOPED:
			break;
	}
}

void add_astroids(int v){
	point ast = mp(rand() * 2.0 / RAND_MAX - 1.0, 1.0);
	astroids.pb(ast);
}


void reset_game(){
	astroid_step = ASTROID_STEP_TIME;
	astroid_new = ASTROID_ADD_TIME;
	astroids.clear();
	player_location = mp(0.0, -0.8);
	game_state = RUNNING;
}